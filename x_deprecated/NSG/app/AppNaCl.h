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
#ifdef NACL
#include "ppapi/cpp/graphics_3d.h"
#include "ppapi/cpp/instance.h"
#include "ppapi/cpp/module.h"
#include "ppapi/cpp/input_event.h"
#include "ppapi/utility/completion_callback_factory.h"
#include "App.h"
#ifdef WIN32
#undef PostMessage
// Allow 'this' in initializer list
#pragma warning(disable : 4355)
#endif

namespace NSG 
{
	namespace NaCl 
	{
		class NaCl3DInstance : public pp::Instance
		{
			public:
				explicit NaCl3DInstance(PP_Instance instance, App* pApp);
				~NaCl3DInstance();
				static pp::Instance* GetInstance();
				virtual bool Init(uint32_t argc, const char* argn[], const char* argv[]);
				virtual void DidChangeView(const pp::View& view);
				virtual void HandleMessage(const pp::Var& message);
				virtual bool HandleInputEvent(const pp::InputEvent& event);
			private:
				void BeginTick();
				void DoTick(float delta);
				void EndTick();

				bool InitGL(int new_width, int new_height);
				void MainLoop(int32_t);

				pp::CompletionCallbackFactory<NaCl3DInstance> callback_factory_;
				pp::Graphics3D context_;
				PInternalApp pApp_;
				int width_;
				int height_;
		};

		class Graphics3DModule : public pp::Module 
		{
			public:
				Graphics3DModule(App* pApp);
				virtual ~Graphics3DModule();
				virtual pp::Instance* CreateInstance(PP_Instance instance);
			private:
				App* pApp_;
		};
	}
}
#endif