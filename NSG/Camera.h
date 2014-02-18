/*
-------------------------------------------------------------------------------
This file is part of nsg-library.
http://nsg-library.googlecode.com/

Copyright (c) 2014-2015 N�stor Silveira Gorski

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
#include "NSG/Node.h"

namespace NSG
{
	class Camera;

	typedef std::shared_ptr<Camera> PCamera;

	class Camera
	{
	public:
		Camera(float fovy, float zNear, float zFar);
		
		~Camera();

		static Camera* GetActiveCamera();

		void Activate();

		void SetViewport(float xo, float yo, float xf, float yf);

		void ViewChanged(int32_t width, int32_t height);

		void SetLookAt(
			//Camera position, in World Space
			const Vertex3& eye, 
			//Looks at
			const Vertex3& center, 
			//Head direction
			const Vertex3& up);

		const glm::mat4& GetViewProjection() const { return matViewProjection_; }

	private:
		void Update();		

		Matrix4 matProjection_;
		Matrix4 matView_;
		Matrix4 matViewProjection_;

		PNode pNode_;

		float fovy_;
		float zNear_;
		float zFar_;

		int32_t width_;
		int32_t height_;

		float xo_;
		float yo_;
		float xf_;
		float yf_;
	};
}