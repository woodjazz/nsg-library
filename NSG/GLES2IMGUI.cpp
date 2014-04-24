/*
-------------------------------------------------------------------------------
This file is part of nsg-library.
http://nsg-library.googlecode.com/

Copyright (c) 2014-2015 Néstor Silveira Gorski

-------------------------------------------------------------------------------
This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
claim that you wrote the original software. If you use this software
in a product, an acknowledgment in the product documentation would be
appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
-------------------------------------------------------------------------------
*/
#include "GLES2IMGUI.h"
#include "GLES2RectangleMesh.h"
#include "GLES2CircleMesh.h"
#include "GLES2EllipseMesh.h"
#include "GLES2RoundedRectangleMesh.h"
#include "Node.h"
#include "Constants.h"
#include "Keys.h"
#include "Log.h"
#include "GLES2FrameColorSelection.h"
#include "GLES2Text.h"
#include "GLES2StencilMask.h"
#include "GLES2Camera.h"
#include "App.h"
#include <map>
#include <list>
#include <set>
#include <cassert>

using namespace NSG;

#define STRINGIFY(S) #S

static const char* vShader = STRINGIFY(
	uniform mat4 u_mvp;
	attribute vec4 a_position;
	attribute vec2 a_texcoord;
	varying vec4 v_position;
	varying vec2 v_texcoord;
	
	void main()
	{
		gl_Position = u_mvp * vec4(a_position.xyz, 1);
		v_position = a_position;
		v_texcoord = a_texcoord;
	}
);

static const char* fShader = STRINGIFY(
	struct Material
	{
		vec4 diffuse;
		float shininess;
	};
	uniform Material u_material;
	uniform sampler2D u_texture;
	varying vec4 v_position;
	varying vec2 v_texcoord;

	void main()
	{
		float factor = u_material.shininess - abs(v_position.y);

        if(u_material.shininess < 0.0)
            factor = abs(v_position.y) + u_material.shininess;

		gl_FragColor = texture2D(u_texture, v_texcoord) * u_material.diffuse * vec4(factor, factor, factor, 1.0);
	}
);

static const char* fShaderBorder = STRINGIFY(
	struct Material
	{
		vec4 diffuse;
	};
	uniform Material u_material;

	void main()
	{
		gl_FragColor = u_material.diffuse;
	}
);

namespace NSG 
{
	namespace IMGUI
	{
		Skin::Skin() 
		: alphaFactor(1),
		fontFile("font/FreeSans.ttf"),
		fontSize(18),
		textMaxLength(std::numeric_limits<int>::max()),
		fillEnabled(true),
		drawBorder(true),
        pActiveMaterial(new GLES2Material()),
        pNormalMaterial(new GLES2Material()),
        pHotMaterial(new GLES2Material()),
        pBorderMaterial(new GLES2Material()),
		pMesh(new GLES2RoundedRectangleMesh(0.5f, 2, 2, 64, GL_STATIC_DRAW))
		{
			pActiveMaterial->SetBlendMode(ALPHA);
			pActiveMaterial->EnableDepthTest(false);
			pNormalMaterial->SetBlendMode(ALPHA);
			pNormalMaterial->EnableDepthTest(false);
			pHotMaterial->SetBlendMode(ALPHA);
			pHotMaterial->EnableDepthTest(false);
			pBorderMaterial->SetBlendMode(ALPHA);
			pBorderMaterial->EnableDepthTest(false);

            PGLES2Program pProgram(new GLES2Program(vShader, fShader));
            pActiveMaterial->SetProgram(pProgram);
			pActiveMaterial->SetDiffuseColor(Color(0,1,0,0.7f));
			//pActiveMaterial->SetDiffuseColor(Color(1,1,1,0.7f));
			pActiveMaterial->SetShininess(0);

            pNormalMaterial->SetProgram(pProgram);
			pNormalMaterial->SetDiffuseColor(Color(0.5f,0.5f,0.5f,0.5f));
			pNormalMaterial->SetShininess(0);

			pHotMaterial->SetProgram(pProgram);
			pHotMaterial->SetDiffuseColor(Color(1,0.5f,0.5f,0.7f));
			pHotMaterial->SetShininess(0);

			PGLES2Program pBorderProgram(new GLES2Program(vShader, fShaderBorder));
			pBorderMaterial->SetProgram(pBorderProgram);
			pBorderMaterial->SetDiffuseColor(Color(1,1,1,1));
		}

		Skin::Skin(const Skin& obj)
		: alphaFactor(obj.alphaFactor),
		fontFile(obj.fontFile),
		fontSize(obj.fontSize),
		textMaxLength(obj.textMaxLength),
		fillEnabled(obj.fillEnabled),
		drawBorder(obj.drawBorder),
		pActiveMaterial(obj.pActiveMaterial),
		pNormalMaterial(obj.pNormalMaterial),
		pHotMaterial(obj.pHotMaterial),
		pBorderMaterial(obj.pBorderMaterial),
		pMesh(obj.pMesh)
		{
		}

		PSkin pSkin;
		PGLES2Camera pCamera;
		PNode pCurrentNode;
		PGLES2FrameColorSelection pFrameColorSelection;		
        PNode pRootNode;
        PGLES2StencilMask pStencilMask;
		int tick = 0;


		class TextManager
		{
		public:
			TextManager(const std::string& fontFile, int fontSize)
			: fontFile_(fontFile),
			fontSize_(fontSize)
			{
			}

			PGLES2Text GetTextMesh(GLushort item)
			{
				Key k(item);

				auto it = textMap_.find(k);

				if(it == textMap_.end())
				{
					PGLES2Text pTextMesh(new GLES2Text(fontFile_.c_str(), fontSize_, GL_STATIC_DRAW));

					textMap_.insert(TextMap::value_type(k, pTextMesh));

					return pTextMesh;
				}

				return it->second;
			}
		private:
			typedef GLushort Key;
			typedef std::map<Key, PGLES2Text> TextMap;
			TextMap textMap_;
			std::string fontFile_;
			int fontSize_;
		};

		typedef std::shared_ptr<TextManager> PTextManager;
		typedef std::pair<std::string, int> TextManagerKey;
		typedef std::map<TextManagerKey, PTextManager> TextManagerMap;
		typedef std::shared_ptr<TextManagerMap> PTextManagerMap;
		PTextManagerMap pTextManagers;

		PGLES2Text GetCurrentTextMesh(GLushort item)
		{
			TextManagerKey k(pSkin->fontFile, pSkin->fontSize);
			
			auto it = pTextManagers->find(k);

			if(it != pTextManagers->end())
			{
				return it->second->GetTextMesh(item);
			}
			else
			{
				PTextManager pTextManager(new TextManager(pSkin->fontFile, pSkin->fontSize));
				pTextManagers->insert(TextManagerMap::value_type(k, pTextManager));
				return pTextManager->GetTextMesh(item);
			}
		}

		class LayoutManager
		{
        public:
			struct LayoutArea;
			typedef std::shared_ptr<LayoutArea> PLayoutArea;
			struct LayoutArea
			{
				GLushort id_;
				int percentage_;
				PNode pNode_;
				enum Type {Vertical, Horizontal, Control, Spacer};
				Type type_;
				float textOffsetX_;
				unsigned int cursor_character_position_; 

                struct Sorting : public std::binary_function<PLayoutArea, PLayoutArea, bool>
                {
                    bool operator()(const PLayoutArea& a, const PLayoutArea& b) const
                    {
                        return a->id_ < b->id_;
                    }
                };

				std::set<PLayoutArea, Sorting> children_; // ordered by id_ (line number)

				LayoutArea(GLushort id, PNode pNode, Type type, int percentage) 
				: id_(id), percentage_(percentage), pNode_(pNode), type_(type), textOffsetX_(0), cursor_character_position_(0)
				{
				}
			};

			LayoutManager() 
			: layoutChanged_(false),
            visibleAreas_(0),
            newControlAdded_(false),
            isStable_(true)
			{
			}

			PLayoutArea InsertNewArea(GLushort id, LayoutArea::Type type, int percentage)
			{
                PLayoutArea pCurrentArea;

				PLayoutArea pArea;

                if(!nestedAreas_.empty())
                {
                	pCurrentArea = nestedAreas_.back();
                    PNode pNode(new Node());
                    pNode->SetParent(pCurrentArea->pNode_);
                	pArea = PLayoutArea(new LayoutArea(id, pNode, type, percentage));
				    pCurrentArea->children_.insert(pArea);
                }
                else
                {
                	pArea = PLayoutArea(new LayoutArea(id, pRootNode, type, percentage));
                }

				auto it = areas_.insert(AREAS::value_type(id, pArea));

                assert(it.second);

                if(++(it.first) != areas_.end())
                {
                    layoutChanged_ = true;
                }

                if(!nestedAreas_.empty())
                {
                	pCurrentArea->pNode_->EnableUpdate(false);
				    RecalculateLayout(pCurrentArea);
				}

				return pArea;
			}

			void Reset()
			{
				nestedAreas_.clear();
				areas_.clear();
				layoutChanged_ = false;
                visibleAreas_ = 0;
			}

			void Begin()
			{
			}

			void End()
			{
				if(layoutChanged_ || visibleAreas_ < areas_.size())
				{
					Reset();
					isStable_ = false;
				}
				else if(newControlAdded_)
				{
					pRootNode->EnableUpdate(true);
					pRootNode->Update(false);
					newControlAdded_ = false;
					isStable_ = false;
				}
				else
				{
					isStable_ = true;
				}

                visibleAreas_ = 0;
			}

			PLayoutArea GetAreaForControl(GLushort id, LayoutArea::Type type = LayoutArea::Control, int percentage = 0)
			{
				PLayoutArea pArea;
				auto it = areas_.find(id);
				if(it != areas_.end())
				{
					pArea = it->second;
				}
				else if(nestedAreas_.size() || type != LayoutArea::Control)
				{
					pArea = InsertNewArea(id, type, percentage);
					newControlAdded_ = true;
				}

                ++visibleAreas_;

                pCurrentNode = pArea->pNode_;
                
				return pArea;
			}

			void BeginHorizontal(GLushort id)
			{
				nestedAreas_.push_back(GetAreaForControl(id, LayoutArea::Horizontal));
			}

			void EndHorizontal()
			{
				nestedAreas_.pop_back();
			}

			void BeginVertical(GLushort id)
			{
				nestedAreas_.push_back(GetAreaForControl(id, LayoutArea::Vertical));
			}

			void EndVertical()
			{
				nestedAreas_.pop_back();
			}

			void Spacer(GLushort id, int percentage)
			{
				GetAreaForControl(id, LayoutArea::Control, percentage);
			}

			void RecalculateLayout(PLayoutArea pCurrentArea)
			{
				size_t n = pCurrentArea->children_.size();

				if(n < 2)
					return;

				int nControls = n;
				float remainingPercentage = 100;
				{
					auto it = pCurrentArea->children_.begin();
					while(it != pCurrentArea->children_.end())
					{
						PLayoutArea pArea = *it;
						
						if(pArea->percentage_)
						{
							remainingPercentage -= pArea->percentage_;
							--nControls;
						}

						++it;
					}
				}

				remainingPercentage /= 100.0f;

				assert(remainingPercentage >= 0 && "Total percentage for spacer exceeds 100%");

				if(nControls > 0)
				{
					auto it = pCurrentArea->children_.begin();
					if(pCurrentArea->type_ == LayoutArea::Vertical)
					{
                        float yPosition = 1; //position to the top of current area
						while(it != pCurrentArea->children_.end())
						{
							float scaleY(remainingPercentage/nControls);
							PLayoutArea pArea = *it;
							if(pArea->percentage_)
							{
								scaleY = pArea->percentage_ / 100.0f;
							}
							float step = scaleY;
                            pArea->pNode_->SetPosition(Vertex3(0, yPosition - step, 0));
							pArea->pNode_->SetScale(Vertex3(1, scaleY, 1));
                            yPosition -= 2*step;
							++it;
						}
					}
					else
					{
						assert(pCurrentArea->type_ == LayoutArea::Horizontal);
                        float xPosition = -1; //position to the left of current area
						while(it != pCurrentArea->children_.end())
						{
							float scaleX(remainingPercentage/nControls);
							PLayoutArea pArea = *it;
							if(pArea->percentage_)
							{
								scaleX = pArea->percentage_ / 100.0f;
							}
                            float step = scaleX;
                            pArea->pNode_->SetPosition(Vertex3(xPosition + step, 0, 0));
							pArea->pNode_->SetScale(Vertex3(scaleX, 1, 1));
                            xPosition += 2*step;
							++it;
						}
					}
				}
			}

			bool IsStable() const 
			{
				return isStable_;
			}

		private:
			std::list<PLayoutArea> nestedAreas_;
			typedef std::map<GLushort, PLayoutArea> AREAS;
			AREAS areas_;
			bool layoutChanged_;
            size_t visibleAreas_;
            bool newControlAdded_;
            bool isStable_;
		};

		typedef std::shared_ptr<LayoutManager> PLayoutManager;

		PLayoutManager pLayoutManager_;

		struct UIState
		{
			float mousex;
			float mousey;
			bool mousedown;

			GLushort hotitem;
			GLushort activeitem;

			GLushort kbditem;
  			int keyentered;
  			int keymod;
  			int keyaction;
  			unsigned int character;
  			GLushort lastwidget;	
  			bool activeitem_is_a_text_field;
		};

		UIState uistate = {0,0,false,0,0, 0,0,0,0,0,0, false};

		void AllocateResources()
		{
			TRACE_LOG("IMGUI Allocating resources");
			pSkin = PSkin(new Skin());
			pCamera = PGLES2Camera(new GLES2Camera());
			pCamera->EnableOrtho();
            pCamera->SetFarClip(1000000);
            pCamera->SetNearClip(-1000000);
            pLayoutManager_ = PLayoutManager(new LayoutManager());
        	pRootNode = PNode(new Node());
        	pTextManagers = PTextManagerMap(new TextManagerMap());
        	pStencilMask = PGLES2StencilMask(new GLES2StencilMask());
		}

		void ReleaseResources()
		{
			TRACE_LOG("IMGUI Releasing resources");
			pStencilMask = nullptr;
			pTextManagers = nullptr;
			pRootNode = nullptr;
            pLayoutManager_ = nullptr;
			pSkin = nullptr;
		}

		GLES2Camera* pActiveCamera = nullptr;
		void Begin()
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			pActiveCamera = GLES2Camera::GetActiveCamera();
			pCamera->Activate();
			uistate.hotitem = 0;

			pCurrentNode = pRootNode;
			pLayoutManager_->Begin();
			pLayoutManager_->BeginVertical(0);
		}

		void End()
		{
			pLayoutManager_->EndVertical();
			pLayoutManager_->End();
			
			if(pActiveCamera)
				pActiveCamera->Activate();

			if(!uistate.activeitem_is_a_text_field)
			{
				if(App::GetPtrInstance()->IsKeyboardVisible())
				{
					App::GetPtrInstance()->HideKeyboard();
					pCamera->SetPosition(Vertex3(0,0,0));
				}
			}

			if(!uistate.mousedown)
			{
				uistate.activeitem = 0;
			}
			else
			{
				if(uistate.activeitem == 0)
				{
			  		uistate.activeitem = -1;
			  		uistate.activeitem_is_a_text_field = false;
			  	}
			}

			// If no widget grabbed tab, clear focus
			if (uistate.keyentered == NSG_KEY_TAB || !pLayoutManager_->IsStable())
				uistate.kbditem = 0;
	
			// Clear the entered key
			uistate.keyentered = 0;	
			uistate.character = 0;	
		}

		void DrawButton(PGLES2Material pMaterial, PNode pNode, bool hasFocus)
		{
            if(pMaterial->IsReady())
            {
            	Color diffuse = pMaterial->GetDiffuseColor();
            	float shininess = pMaterial->GetShininess();

                pMaterial->SetDiffuseColor(diffuse * Color(1,1,1, pSkin->alphaFactor));
                pMaterial->SetShininess(hasFocus ? 1 : -0.25f);
                pMaterial->Render(pSkin->fillEnabled, pNode.get(), pSkin->pMesh.get());
                
                pMaterial->SetShininess(shininess);
                pMaterial->SetDiffuseColor(diffuse);
            }
		}

		bool Hit(GLushort id, PNode pNode)
		{
			if(!pFrameColorSelection)
				return false;

			pFrameColorSelection->Begin(uistate.mousex, uistate.mousey);
	    	pFrameColorSelection->Render(id, pSkin->pMesh.get(), pNode.get());
		    pFrameColorSelection->End();

		    return id == pFrameColorSelection->GetSelected();
		}

		void InternalBeginHorizontal(GLushort id)
		{
			pLayoutManager_->BeginHorizontal(id);
		}

		void InternalEndHorizontal()
		{
			pLayoutManager_->EndHorizontal();
		}

		void InternalBeginVertical(GLushort id)
		{
			pLayoutManager_->BeginVertical(id);
		}

		void InternalEndVertical()
		{
			pLayoutManager_->EndVertical();
		}

		void InternalSpacer(GLushort id, int percentage)
		{
			pLayoutManager_->Spacer(id, percentage);
		}

		bool InternalButton(GLushort id, const std::string& text)
		{
            PNode pNode = pLayoutManager_->GetAreaForControl(id)->pNode_;
            assert(pNode);

			if(!pLayoutManager_->IsStable())
				return false;

			// Check whether the button should be hot
			if (Hit(id, pNode))
			{
				uistate.hotitem = id;

				if (uistate.activeitem == 0 && uistate.mousedown)
				{
					uistate.activeitem_is_a_text_field = false;
			  		uistate.activeitem = id;
			  		uistate.kbditem = id;
			  	}
			}
			// If no widget has keyboard focus, take it
			if (uistate.kbditem == 0)
			{
				uistate.kbditem = id;
				uistate.activeitem_is_a_text_field = false;
			}

			// If we have keyboard focus, show it
			bool hasFocus = uistate.kbditem == id;

			if(uistate.hotitem == id)
			{
				if(uistate.activeitem == id)
				{
			  		// Button is both 'hot' and 'active'
			  		DrawButton(pSkin->pActiveMaterial, pNode, hasFocus);
			  	}
				else
				{
					// Button is merely 'hot'
					DrawButton(pSkin->pHotMaterial, pNode, hasFocus);
				}
			}
			else
			{
				// button is not hot, but it may be active    
				DrawButton(pSkin->pNormalMaterial, pNode, hasFocus);
			}

			if(pSkin->drawBorder)
			{
				bool fillEnabled = pSkin->fillEnabled;
				pSkin->fillEnabled = false;
				DrawButton(pSkin->pBorderMaterial, pNode, hasFocus);
				pSkin->fillEnabled = fillEnabled;
			}

			{
				pStencilMask->Begin();
				pStencilMask->Render(pNode.get(), pSkin->pMesh.get());
				pStencilMask->End();
				PGLES2Text pTextMesh = GetCurrentTextMesh(id);
	            pTextMesh->SetText(text);
				
				{
	                static PNode pTextNode(new Node());
	                pTextNode->EnableUpdate(false);
	                pTextNode->SetParent(pNode);
	                pTextNode->SetInheritScale(false);
                    pTextNode->SetScale(pRootNode->GetGlobalScale());
                    static PSceneNode pTextNode1(new SceneNode());
                    pTextNode1->SetParent(pTextNode);
                    pTextNode1->SetPosition(Vertex3(-pTextMesh->GetWidth()/2, -pTextMesh->GetHeight()/2, 0));
                    pTextNode->EnableUpdate(true);
                    pTextNode->Update(false);

                    pTextNode1->SetMesh(pTextMesh);
                    static PGLES2Material pTextMaterial(new GLES2Material());
                    pTextMaterial->SetDiffuseTexture(pTextMesh->GetAtlas());
                    pTextMaterial->SetProgram(pTextMesh->GetProgram());
                    pTextNode1->SetMaterial(pTextMaterial);
                    pTextMaterial->SetDiffuseColor(Color(1,1,1,pSkin->alphaFactor));
                    pTextNode1->Render(true);
				}
			}

			bool enterKey = false;

			// If we have keyboard focus, we'll need to process the keys
			if (hasFocus)
			{
				switch (uistate.keyentered)
				{
				case NSG_KEY_TAB:
					// If tab is pressed, lose keyboard focus.
					// Next widget will grab the focus.
					uistate.kbditem = 0;
					// If shift was also pressed, we want to move focus
					// to the previous widget instead.
					if (uistate.keymod & NSG_KEY_MOD_SHIFT)
						uistate.kbditem = uistate.lastwidget;
					// Also clear the key so that next widget
					// won't process it
					uistate.keyentered = 0;
					break;

				case NSG_KEY_ENTER:
					// Had keyboard focus, received return,
					// so we'll act as if we were clicked.
					enterKey = true;
				  	break;
				}
			}

			uistate.lastwidget = id;

			// If button is hot and active, but mouse button is not
			// down, the user must have clicked the button.
			return enterKey || (uistate.mousedown == 0 && uistate.hotitem == id &&  uistate.activeitem == id);
		}		

		std::string InternalTextField(GLushort id, const std::string& text, std::regex* pRegex)
		{
            LayoutManager::PLayoutArea pArea = pLayoutManager_->GetAreaForControl(id);
			PNode pNode = pArea->pNode_;

			if(!pLayoutManager_->IsStable())
				return text;

            Vertex4 areaSize = pCamera->GetModelViewProjection(pNode.get()) * Vertex4(2,0,0,0);

			std::string currentText = text;

            PGLES2Text pTextMesh = GetCurrentTextMesh(id);
			PGLES2Text pCursorMesh = GetCurrentTextMesh(-1);
			pCursorMesh->SetText("_");

			// Check whether the button should be hot
			if (Hit(id, pNode))
			{
				uistate.hotitem = id;

				if (uistate.activeitem == 0 && uistate.mousedown)
				{
			  		uistate.activeitem = id;
			  		uistate.kbditem = id;
			  		uistate.activeitem_is_a_text_field = true;

			  		///////////////////////////////////////////////
                    //Calculates cursor's position after click
                    Vertex4 worldPos = pNode->GetGlobalModelMatrix() * Vertex4(-1,0,0,1); //left border in world coords
                    Vertex3 screenPos = pCamera->WorldToScreen(Vertex3(worldPos)); //left border in screen coords
                    float mouseRelPosX(uistate.mousex - screenPos.x);
                    float textEndRelPosX = pTextMesh->GetWidth();
                    float mouseTotalX = mouseRelPosX  + pArea->textOffsetX_;
                    if(mouseTotalX > textEndRelPosX)
                    {
                    	// mouse exceeded text length
                        pArea->cursor_character_position_ = currentText.length();
                    }
                    else
                    {
                         pArea->cursor_character_position_ = pTextMesh->GetCharacterPositionForWidth(mouseTotalX)-1;
                    }
                    ///////////////////////////////////////////////

					if(!App::GetPtrInstance()->IsKeyboardVisible())
					{
						if(App::GetPtrInstance()->ShowKeyboard())
						{
							Vertex3 position(0, screenPos.y, 0);
				  			pCamera->SetPosition(position);
				  		}
				  	}
			  	}
			}

			// If no widget has keyboard focus, take it
			if (uistate.kbditem == 0)
			{
				uistate.kbditem = id;
				pArea->cursor_character_position_ = currentText.length();
			}

			// If we have keyboard focus, show it
			bool hasFocus = uistate.kbditem == id;

			if(uistate.hotitem == id)
			{
				if(uistate.activeitem == id)
				{
			  		// Button is both 'hot' and 'active'
                    DrawButton(pSkin->pActiveMaterial, pNode, hasFocus);
			  	}
				else
				{
					// Button is merely 'hot'
					DrawButton(pSkin->pHotMaterial, pNode, hasFocus);
				}
			}
			else
			{
				// button is not hot, but it may be active    
				DrawButton(pSkin->pNormalMaterial, pNode, hasFocus);
			}

			if(pSkin->drawBorder)
			{
				bool fillEnabled = pSkin->fillEnabled;
				pSkin->fillEnabled = false;
				DrawButton(pSkin->pBorderMaterial, pNode, hasFocus);
				pSkin->fillEnabled = fillEnabled;
			}

			{
				pStencilMask->Begin();
				pStencilMask->Render(pNode.get(), pSkin->pMesh.get());
				pStencilMask->End();

	            pTextMesh->SetText(currentText);
				
                float cursorPositionInText = pTextMesh->GetWidthForCharacterPosition(pArea->cursor_character_position_);

                if(pArea->textOffsetX_ + areaSize.x < cursorPositionInText)
				{
                    //If cursor moves to the right of the area then scroll
 					pArea->textOffsetX_ = pCursorMesh->GetWidth() + cursorPositionInText - areaSize.x;  
				}
                else if(cursorPositionInText - pArea->textOffsetX_ < 0) 
                {
                    // if cursor moves to the left of the area then scroll
                    pArea->textOffsetX_ -= areaSize.x /4;

                    if(pArea->textOffsetX_ < 0)
                        pArea->textOffsetX_ = 0;
                }

                static PNode pTextNode(new Node());
                pTextNode->EnableUpdate(false);
	            pTextNode->SetParent(pNode);
	            pTextNode->SetPosition(Vertex3(-1, 0, 0)); //move text to the beginning of the current area
                static PNode pTextNode0(new Node());
                pTextNode0->SetParent(pTextNode);
	            pTextNode0->SetInheritScale(false);
                pTextNode0->SetScale(pRootNode->GetGlobalScale());
                static PSceneNode pTextNode1(new SceneNode());
                pTextNode1->SetParent(pTextNode0);
                pTextNode1->SetPosition(Vertex3(-pArea->textOffsetX_, 0, 0));
                pTextNode->EnableUpdate(true);
                pTextNode->Update(false);
                pTextNode1->SetMesh(pTextMesh);
                static PGLES2Material pTextMaterial(new GLES2Material());
                pTextMaterial->SetDiffuseTexture(pTextMesh->GetAtlas());
                pTextMaterial->SetProgram(pTextMesh->GetProgram());
                pTextNode1->SetMaterial(pTextMaterial);
                pTextMaterial->SetDiffuseColor(Color(1,1,1,pSkin->alphaFactor));
                pTextNode1->Render(true);
                
				// Render cursor if we have keyboard focus
				if(hasFocus && (tick < 15))
                {
                    static PSceneNode pCursorNode(new SceneNode());
                    pCursorNode->EnableUpdate(false);
                    pCursorNode->SetParent(pTextNode1);
                    pCursorNode->SetPosition(Vertex3(cursorPositionInText, 0, 0));
                    pCursorNode->EnableUpdate(true);
                    pCursorNode->Update(false);

                    pCursorNode->SetMesh(pCursorMesh);
                    static PGLES2Material pTextMaterial(new GLES2Material());
                    pTextMaterial->SetDiffuseTexture(pTextMesh->GetAtlas());
                    pTextMaterial->SetProgram(pTextMesh->GetProgram());
                    pCursorNode->SetMaterial(pTextMaterial);
                    pTextMaterial->SetDiffuseColor(Color(1,0,0,pSkin->alphaFactor));
                    pCursorNode->Render(true);
                }
			}

			// If we have keyboard focus, we'll need to process the keys
			if (hasFocus)
			{
				switch (uistate.keyentered)
				{
				case NSG_KEY_TAB:
					// If tab is pressed, lose keyboard focus.
					// Next widget will grab the focus.
					uistate.kbditem = 0;
					// If shift was also pressed, we want to move focus
					// to the previous widget instead.
					if (uistate.keymod & NSG_KEY_MOD_SHIFT)
						uistate.kbditem = uistate.lastwidget;
					// Also clear the key so that next widget
					// won't process it
					uistate.keyentered = 0;
					break;
				case NSG_KEY_BACKSPACE:
					if(pArea->cursor_character_position_ > 0)
					{
                        std::string::iterator it = currentText.begin() + pArea->cursor_character_position_ - 1;
                        currentText.erase(it);
                        --pArea->cursor_character_position_;
					}
					break;   

                case NSG_KEY_DELETE:
					if(pArea->cursor_character_position_ < currentText.length())
					{
                        std::string::iterator it = currentText.begin() + pArea->cursor_character_position_;
                        currentText.erase(it);
					}
                    break;

                case NSG_KEY_RIGHT:
                    if(pArea->cursor_character_position_ < currentText.length())
                        ++pArea->cursor_character_position_;
                    break;

                case NSG_KEY_LEFT:
                    if(pArea->cursor_character_position_ > 0)
                        --pArea->cursor_character_position_;
                    break;

                case NSG_KEY_HOME:
                    pArea->cursor_character_position_ = 0;
                    break;

                case NSG_KEY_END:
                    pArea->cursor_character_position_ = currentText.length();
                    break;

                case NSG_KEY_ENTER:
					if(App::GetPtrInstance()->IsKeyboardVisible())
					{
						App::GetPtrInstance()->HideKeyboard();
						pCamera->SetPosition(Vertex3(0,0,0));
					}
					break;
				}

	            if (uistate.character >= 32 && uistate.character < 256 && currentText.size() < pSkin->textMaxLength)
	            {
	            	std::string textCopy = currentText;

                    std::string::iterator it = currentText.begin() + pArea->cursor_character_position_;
	                currentText.insert(it, (char)uistate.character);
                    ++pArea->cursor_character_position_;

	            	if(pRegex)
	            	{
	            		if(!regex_match(currentText, *pRegex))
	            		{
	            			currentText = textCopy;
	            			--pArea->cursor_character_position_;
	            		}
	            	}
	            }
			}

			// If button is hot and active, but mouse button is not
			// down, the user must have clicked the widget; give it 
			// keyboard focus.
			if (uistate.mousedown == 0 && uistate.hotitem == id && uistate.activeitem == id)
				uistate.kbditem = id;

			uistate.lastwidget = id;

			return currentText;
		}		


		void ViewChanged(int32_t width, int32_t height)
		{
			if(!pFrameColorSelection)
        		pFrameColorSelection = PGLES2FrameColorSelection(new GLES2FrameColorSelection(false));

        	pFrameColorSelection->ViewChanged(width, height);
		}

        void OnMouseMove(float x, float y)
        {
        	uistate.mousex = x;
        	uistate.mousey = y;
        }

        void OnMouseDown(float x, float y)
        {
        	uistate.mousex = x;
        	uistate.mousey = y;
        	uistate.mousedown = true;
        }

        void OnMouseUp()
        {
        	uistate.mousedown = false;
        }

        void OnKey(int key, int action, int modifier)
        {
            if(action == NSG_KEY_PRESS)
            {
                uistate.keyentered = key;
                uistate.keyaction = action;
                uistate.keymod = modifier;
            }
        }

        void OnChar(unsigned int character)
        {
        	uistate.character = character;
        }


        void DoTick()
        {
        	++tick;
            if(tick > 30)
                tick = 0;
        }
	}
}

