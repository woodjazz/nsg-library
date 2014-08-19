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
#pragma once
#include <memory>

namespace NSG 
{
	class App;
	class SceneNode;
	class Behavior
	{
	public:
		Behavior();
		virtual ~Behavior();
		virtual void Start() {}
		virtual void Update() {}
		virtual void Render() {};
        virtual void OnMouseMove(float x, float y) {}
        virtual void OnMouseDown(float x, float y) {}
        virtual void OnMouseUp() {}
        virtual void OnKey(int key, int action, int modifier) {}
        virtual void OnChar(unsigned int character) {}
		void SetSceneNode(SceneNode* pSceneNode);
		SceneNode* GetSceneNode() const { return pSceneNode_; }

		static void StartAll();
		static void UpdateAll();
		static void RenderAll();
		static void RenderAllBut(const Behavior* behavior);
		static void Render2SelectAll();
        static void OnMouseMoveAll(float x, float y);
        static void OnMouseDownAll(float x, float y);
        static void OnMouseUpAll();
        static void OnKeyAll(int key, int action, int modifier);
        static void OnCharAll(unsigned int character);
    protected:
		SceneNode* pSceneNode_;
	};
}
