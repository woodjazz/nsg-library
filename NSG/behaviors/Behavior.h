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

namespace NSG 
{
	class App;
	class SceneNode;
	class Scene;
	struct ContactPoint;
	struct Behavior
	{
		App& app_;
		SceneNode* sceneNode_;

		virtual ~Behavior();
		virtual void Start() {}
		virtual void Update() {}
        virtual void ViewChanged(int width, int height) {}
        virtual void OnMouseMove(float x, float y) {}
		virtual void OnMouseDown(int button, float x, float y) {}
        virtual void OnMouseWheel(float x, float y) {}
		virtual void OnMouseUp(int button, float x, float y) {}
		virtual void OnMultiGesture(int timestamp, float x, float y, float dTheta, float dDist, int numFingers) {}
        virtual void OnKey(int key, int action, int modifier) {}
        virtual void OnChar(unsigned int character) {}
        virtual void OnCollision(const ContactPoint& contactInfo) {}

	protected:
		Behavior();
	};
}
