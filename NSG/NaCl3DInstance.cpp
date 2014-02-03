#ifdef NACL
#include "NaCl3DInstance.h"

#include <sys/mount.h>
#include "nacl_io/nacl_io.h"
#include "ppapi/cpp/var.h"
#include "ppapi/cpp/var_array.h"
#include "ppapi/lib/gl/gles2/gl2ext_ppapi.h"
#include "Log.h"

static pp::Instance* s_ppInstance = nullptr;

int PPPrintMessage(const char* format, ...) 
{
	va_list args;
	const int maxBuffer = 1024;
	char buffer[maxBuffer];
	va_start(args, format);
	int ret_status = vsnprintf(buffer, maxBuffer, format, args);
	va_end(args);

	assert(s_ppInstance != nullptr && "s_ppInstance is null");

	pp::Var var_reply(buffer);
	s_ppInstance->PostMessage(var_reply);

	return ret_status;	
}

static pthread_t g_handle_message_thread;

namespace NSG 
{
	namespace NaCl 
	{
		NaCl3DInstance::NaCl3DInstance(PP_Instance instance, NSG::PApp pApp) 
		: pp::Instance(instance), callback_factory_(this), pApp_(pApp) 
		{
			s_ppInstance = this;
		}

		pp::Instance* NaCl3DInstance::GetInstance()
		{
			return s_ppInstance;
		}

		bool NaCl3DInstance::Init(uint32_t argc, const char* argn[], const char* argv[]) 
		{
			nacl_io_init_ppapi(pp::Instance::pp_instance(), pp::Module::Get()->get_browser_interface());

			int status = mount("","/persistence","html5fs",0,"type=PERSISTENT,expected_size=1048576");

			if(status != 0)
				TRACE_LOG("mount has failed with error=" << status)

			return true;
		}

		void NaCl3DInstance::DidChangeView(const pp::View& view) 
		{
			int32_t new_width = view.GetRect().width();
			int32_t new_height = view.GetRect().height();
			if (context_.is_null()) {
				if (!InitGL(new_width, new_height)) 
				{
					return; // failed.
				}
				pApp_->Setup();
				MainLoop(0);
			} 
			else 
			{
				// Resize the buffers to the new size of the module.
				int32_t result = context_.ResizeBuffers(new_width, new_height);
				if (result < 0) {
					fprintf(stderr,
							"Unable to resize buffers to %d x %d!\n",
							new_width,
							new_height);

					return;
				}
			}

			pApp_->ViewChanged(new_width, new_height);
		}

		void NaCl3DInstance::HandleMessage(const pp::Var& message) 
		{
			pApp_->HandleMessage(message);
		}

		bool NaCl3DInstance::InitGL(int32_t new_width, int32_t new_height) 
		{
			if (!glInitializePPAPI(pp::Module::Get()->get_browser_interface())) 
			{
				fprintf(stderr, "Unable to initialize GL PPAPI!\n");
				return false;
			}
			const int32_t attrib_list[] = 
			{
				PP_GRAPHICS3DATTRIB_ALPHA_SIZE, 8,
				PP_GRAPHICS3DATTRIB_DEPTH_SIZE, 24,
				PP_GRAPHICS3DATTRIB_WIDTH, new_width,
				PP_GRAPHICS3DATTRIB_HEIGHT, new_height,
				PP_GRAPHICS3DATTRIB_NONE
			};
			
			context_ = pp::Graphics3D(this, attrib_list);
			
			if (!BindGraphics(context_)) 
			{
				fprintf(stderr, "Unable to bind 3d context!\n");
				context_ = pp::Graphics3D();
				glSetCurrentContextPPAPI(0);
				return false;
			}

			glSetCurrentContextPPAPI(context_.pp_resource());
			
			return true;
		}

		void NaCl3DInstance::MainLoop(int32_t) 
		{
			if(pApp_->Update())
				pApp_->Draw();
			
			context_.SwapBuffers(callback_factory_.NewCallback(&NaCl3DInstance::MainLoop));
		}

		Graphics3DModule::Graphics3DModule(NSG::PApp pApp) : pp::Module(), pApp_(pApp) 
		{
		}

		Graphics3DModule::~Graphics3DModule() 
		{
		}

		pp::Instance* Graphics3DModule::CreateInstance(PP_Instance instance) 
		{
			return new NaCl3DInstance(instance, pApp_);
		}
	}
}
#endif