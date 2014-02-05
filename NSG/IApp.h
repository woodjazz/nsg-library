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
#if NACL
#include "ppapi/cpp/var.h"
#endif

#include "Tick.h"

namespace NSG 
{
	class IApp 
	{
	public:
		virtual ~IApp() {};
		virtual void Start() = 0;
		virtual void Update(float delta) = 0;
		virtual void LateUpdate() = 0;
		virtual void RenderFrame() = 0;
		virtual void ViewChanged(int32_t width, int32_t height) = 0;
	#if NACL
		virtual void HandleMessage(const pp::Var& var_message) = 0;
	#endif
	};

	typedef std::shared_ptr<IApp> PApp;
}	
