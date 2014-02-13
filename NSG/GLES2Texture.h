#pragma once

#include "GLES2Includes.h"
#include <memory>
#include <string>

#if NACL
#include "NSG/NaClURLLoader.h"
#elif ANDROID
#include <android/asset_manager.h>
#endif

namespace NSG
{
	class GLES2Texture
	{
	public:
		GLES2Texture(const char* filename);
	#if ANDROID	
		GLES2Texture(AAssetManager* pAAssetManager, const char* filename);
	#endif		
		~GLES2Texture();
		bool Done();
		GLuint GetId() const { return texture_; }
		void Bind() { glBindTexture(GL_TEXTURE_2D, texture_); }
		static void UnBind() { glBindTexture(GL_TEXTURE_2D, 0); }

	private:
		GLuint texture_;
		std::string filename_;
		bool loaded_;

	#if NACL		
		NaCl::PNaClURLLoader pLoader_;
	#elif ANDROID		
		AAssetManager* pAAssetManager_;		
	#endif
	};

	typedef std::shared_ptr<GLES2Texture> PGLES2Texture;
}