/*
-------------------------------------------------------------------------------
This file is part of nsg-library.
http://github.com/woodjazz/nsg-library

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
#pragma once
#include "Types.h"
#include "GUI.h"

struct ImDrawList;
namespace NSG
{
	class Editor
	{
	public:
		Editor();
		~Editor();
		void SetCamera(PCamera camera);
		void SetWindow(PWindow window);
		void SetScene(PScene scene);
		void SetNode(PNode node);
		Node* GetNode() const;
		PTexture GetMaterialPreview(PMaterial material);
		PCamera GetEditorCamera() const { return editorCamera_; }
		void Render();
	private:
		void SetControl();
		void ShowWindows();
		void ShowGame();
		void RenderGame();
		void ShowScene();
		void ShowHierachy();
		void OnMouseDown(int button, float x, float y);
		void ShowInspector();
		PTexture GetScenePreview(Scene* scene, Camera* camera);
		PTexture GetGamePreview(Scene* scene, Camera* camera);
		PCameraControl control_;
		PFrameBuffer previewFrameBuffer_;
		PFrameBuffer sceneFrameBuffer_;
		PFrameBuffer gameFrameBuffer_;
		PWeakCamera camera_;
		PWeakWindow window_;
		PWeakScene scene_;
		PWeakNode node_;
		SignalMouseButton::PSlot slotMouseDown_;
		PScene scenePreview_;
		PSceneNode previewNode_;
		bool isSceneHovered_;
		PCamera editorCamera_;
		PTexture gTexture_;
		GUI editorGUI_;
		GUI gameGUI_;
	};
}