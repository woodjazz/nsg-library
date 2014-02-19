#pragma once

#include "ppapi/cpp/graphics_3d.h"
#include "ppapi/cpp/instance.h"
#include "ppapi/cpp/module.h"
#include "ppapi/cpp/input_event.h"
#include "ppapi/utility/completion_callback_factory.h"

#include "NSG/IApp.h"

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
				explicit NaCl3DInstance(PP_Instance instance, PApp pApp);
				static pp::Instance* GetInstance();
				virtual bool Init(uint32_t argc, const char* argn[], const char* argv[]);
				virtual void DidChangeView(const pp::View& view);
				virtual void HandleMessage(const pp::Var& message);
				virtual bool HandleInputEvent(const pp::InputEvent& event);
			private:
				void BeginTick();
				void DoTick(float delta);
				void EndTick();

				bool InitGL(int32_t new_width, int32_t new_height);
				void MainLoop(int32_t);

				pp::CompletionCallbackFactory<NaCl3DInstance> callback_factory_;
				pp::Graphics3D context_;
				PInternalApp pApp_;
				int32_t width_;
				int32_t height_;
		};

		class Graphics3DModule : public pp::Module 
		{
			public:
				Graphics3DModule(NSG::PApp pApp);
				virtual ~Graphics3DModule();
				virtual pp::Instance* CreateInstance(PP_Instance instance);
			private:
				PApp pApp_;
		};
	}
}