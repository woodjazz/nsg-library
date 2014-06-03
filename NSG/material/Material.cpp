#include "Material.h"
#include "Mesh.h"
#include "Check.h"
#include "Camera.h"
#include "Context.h"
#include "Texture.h"
#include "Scene.h"
#include <assert.h>
#include <sstream>
#include <algorithm>


namespace NSG
{
	Material::Material() 
	: ambient_(1,1,1,1),
	diffuse_(1,1,1,1),
	specular_(1,1,1,1),
	shininess_(1),
    blendMode_(ALPHA),
    enableDepthTest_(true),
    enableCullFace_(false),
    enableStencilTest_(false),
    stencilMask_(0xFF),
	sfailStencilOp_(GL_KEEP),
	dpfailStencilOp_(GL_KEEP),
	dppassStencilOp_(GL_REPLACE),
	stencilFunc_(GL_ALWAYS),
	stencilRefValue_(0x00),
	stencilMaskValue_(0xFF),
    enableColorBuffer_(true),
    enableDepthBuffer_(true)
	{
	}

	Material::~Material()
	{
		Context::this_->Remove(this);
	}

	void Material::SetBlendMode(BLEND_MODE mode)
	{
		blendMode_ = mode;
	}

	void Material::EnableColorBuffer(bool enable)
	{
		enableColorBuffer_ = enable;
	}

	void Material::EnableDepthBuffer(bool enable)
	{
		enableDepthBuffer_ = enable;
	}

	void Material::EnableDepthTest(bool enable)
	{
		enableDepthTest_ = enable;
	}

	void Material::EnableCullFace(bool enable)
	{
		enableCullFace_ = enable;
	}

	void Material::EnableStencilTest(bool enable)
	{
		enableStencilTest_ = enable;	
	}

	void Material::SetStencilMask(GLuint mask)
	{
		stencilMask_ = mask;
	}

	void Material::SetStencilOp(GLenum sfail, GLenum dpfail, GLenum dppass)
	{
        sfailStencilOp_ = sfail;
        dpfailStencilOp_ = dpfail;
        dppassStencilOp_ = dppass;
	}

	void Material::SetStencilFunc(GLenum func, GLint ref, GLuint mask)
	{
        stencilFunc_ = func;
        stencilRefValue_ = ref;
        stencilMaskValue_ = mask;
    }

	void Material::SetProgram(PProgram pProgram)
	{
		if(pProgram_ != pProgram)
		{
			pProgram_ = pProgram;
            Invalidate();

		}
	}

	void Material::SetTexture0(PTexture pTexture)
	{
        if(pTexture0_ != pTexture)
        {
	    	pTexture0_ = pTexture;
            Invalidate();
        }
	}

	void Material::SetTexture1(PTexture pTexture)
	{
        if(pTexture1_ != pTexture)
        {
		    pTexture1_ = pTexture;
            Invalidate();
        }
	}

	bool Material::IsValid()
	{
        bool isReady = pProgram_ && pProgram_->IsReady();
        if(isReady)
        {
            if(pTexture0_)
            {
                isReady = pTexture0_->IsReady();
            }
            else
            {
                pTexture0_ = Context::this_->GetWhiteTexture();
                isReady = pTexture0_->IsReady();
            }

            if(isReady && pTexture1_)
            {
                isReady = pTexture1_->IsReady();
            }
        }

        return isReady;
	}

	void Material::AllocateResources()
	{
	}

	void Material::ReleaseResources()
	{
	}

	void Material::Use()
	{
		if(enableColorBuffer_)
		{
			glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		}
		else
		{
			glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		}

		if(enableDepthBuffer_)
		{
			glDepthMask(GL_TRUE);
		}
		else
		{
			glDepthMask(GL_FALSE);
		}

		if(enableStencilTest_)
		{
			glEnable(GL_STENCIL_TEST);
			glStencilMaskSeparate(GL_FRONT_AND_BACK, stencilMask_); 
			glStencilOp(sfailStencilOp_, dpfailStencilOp_, dppassStencilOp_);
			glStencilFunc(stencilFunc_, stencilRefValue_, stencilMaskValue_); 
		}
		else
		{
			glDisable(GL_STENCIL_TEST);
		}

		if(enableCullFace_)
		{
			glEnable(GL_CULL_FACE);
			//glCullFace(GL_FRONT);
		}
		else
		{
			glDisable(GL_CULL_FACE);
		}

        switch(blendMode_)
       	{
        	case NONE:
	        	glDisable(GL_BLEND);
	        	break;

	        case ALPHA:
	        	glEnable(GL_BLEND);
	        	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	        	break;

	        default:
	        	CHECK_ASSERT(false && "Undefined blend mode", __FILE__, __LINE__);
	        	break;
        }
        
        if(enableDepthTest_)
        {
        	glEnable(GL_DEPTH_TEST);
        }
        else
        {
        	glDisable(GL_DEPTH_TEST);
        }
	}

	void Material::Render(bool solid, Node* pNode, Mesh* pMesh)
	{
		if(IsReady() && pMesh->IsReady())
		{
			Use();

			UseProgram useProgram(*pProgram_);
			pProgram_->Use(this);

			pProgram_->Use(pNode);
			GLuint positionLoc = pProgram_->GetPositionLoc();
			GLuint texcoordLoc = pProgram_->GetTextCoordLoc();
			GLuint normalLoc = pProgram_->GetNormalLoc();
			GLuint colorLoc = pProgram_->GetColorLoc();

			pMesh->Render(solid, positionLoc, texcoordLoc, normalLoc, colorLoc);
	    }
	}

	void Material::Render(bool solid, const std::vector<MeshNode>& meshNodes)
	{
		if(IsReady())
		{
			Use();

			UseProgram useProgram(*pProgram_);
			pProgram_->Use(this);

			GLuint positionLoc = pProgram_->GetPositionLoc();
			GLuint texcoordLoc = pProgram_->GetTextCoordLoc();
			GLuint normalLoc = pProgram_->GetNormalLoc();
			GLuint colorLoc = pProgram_->GetColorLoc();

			auto it = meshNodes.begin();
			while(it != meshNodes.end())
			{
                pProgram_->Use(it->first.get());

                if(it->second->IsReady())
				{
					it->second->Render(solid, positionLoc, texcoordLoc, normalLoc, colorLoc);
				}
				++it;
			}
	    }
	}

	
}