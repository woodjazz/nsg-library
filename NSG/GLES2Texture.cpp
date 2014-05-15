#include "GLES2Texture.h"
#include "GLES2PlaneMesh.h"
#include "GLES2Camera.h"
#include "Context.h"
#include "SOIL.h"
#include "Log.h"
#include "Check.h"
#include "App.h"
#if NACL
#include "AppNaCl.h"
#endif
#include <thread>
#include <chrono>
#include <fstream>
#include <sstream>
#include <assert.h>

static const char* vShader = STRINGIFY(
	attribute vec4 a_position;
	attribute vec2 a_texcoord;
	varying vec2 v_texcoord;

	void main()
	{
		gl_Position = a_position;
		v_texcoord = vec2(a_texcoord.x, 1.0 - a_texcoord.y);
	}
);

static const char* fShader = STRINGIFY(
	uniform sampler2D u_texture0;
	varying vec2 v_texcoord;
	void main()
	{
		gl_FragColor = texture2D(u_texture0, v_texcoord);
	}
);


namespace NSG
{
	GLES2Texture::GLES2Texture(const char* filename) 
	: filename_(filename),
	texture_(0), 
	pResource_(new Resource(filename)), 
    pMaterial_(new GLES2Material ()),
	pMesh_(new GLES2PlaneMesh(2, 2, 2, 2, GL_STATIC_DRAW)),
	width_(0),
	height_(0),
	format_(GL_RGBA)
	{
		PGLES2Program pProgram(new GLES2Program(vShader, fShader));
		pMaterial_->SetProgram(pProgram);
		pMaterial_->EnableDepthTest(false);
		
		Context::this_->Add(this);
	}

	GLES2Texture::GLES2Texture(GLint format, GLsizei width, GLsizei height, const char* pixels) 
	: texture_(0), 
    pResource_(new Resource(pixels, width*height)),
    pMesh_(new GLES2PlaneMesh(2, 2, 2, 2, GL_STATIC_DRAW)),
	width_(width),
	height_(height),
	format_(format)
	{
		Context::this_->Add(this);
	}


	GLES2Texture::~GLES2Texture()
	{
		Context::this_->Remove(this);
	}

	bool GLES2Texture::IsValid()
	{
		return pResource_->IsLoaded();
	}

	void GLES2Texture::AllocateResources()
	{
		glGenTextures(1, &texture_);
		glBindTexture(GL_TEXTURE_2D, texture_);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		if(!pResource_->GetFilename().empty())
		{
            int channels = 0;
			unsigned char* img = SOIL_load_image_from_memory((const unsigned char*)pResource_->GetData(), pResource_->GetBytes(), &width_, &height_, &channels, 0);

            if(channels == 4)
            {
                format_ = GL_RGBA;
            }
            else if(channels == 3)
            {
                format_ = GL_RGB;
            }
            else
            {
                format_ = GL_RGB;
                TRACE_LOG(pResource_->GetFilename() << " Unknown internalformat");
            }

            glBindTexture(GL_TEXTURE_2D, texture_);

			glTexImage2D(GL_TEXTURE_2D,
				0,
				format_,
				width_,
				height_,
				0,
				format_,
				GL_UNSIGNED_BYTE,
				img);

			glBindTexture(GL_TEXTURE_2D, 0);

			SOIL_free_image_data(img);
		}
		else
		{
			const char* data = nullptr;

			if(pResource_->GetBytes())
			{
            	CHECK_ASSERT(width_*height_ == pResource_->GetBytes(), __FILE__, __LINE__);
            	data = pResource_->GetData();
            }

			glBindTexture(GL_TEXTURE_2D, texture_);

			glTexImage2D(GL_TEXTURE_2D,
				0,
				format_,
				width_,
				height_,
				0,
				format_,
				GL_UNSIGNED_BYTE,
				data);

            if(data == nullptr)
            {
            	int channels = 0;

            	switch(format_)
            	{
            		case GL_ALPHA:
            		case GL_LUMINANCE:
            			channels = 1;
            			break;
            		case GL_LUMINANCE_ALPHA:
            			channels = 2;
            			break;
            		case GL_RGB:
            			channels = 3;
            			break;
            		case GL_RGBA:
            			channels = 4;
            			break;
            		default:
            			CHECK_ASSERT(false && "Unknown format", __FILE__, __LINE__);
            			break; 
            	}

		        std::vector<GLubyte> emptyData(width_ * height_ * channels, 0);
		        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width_, height_, format_, GL_UNSIGNED_BYTE, &emptyData[0]);		
            }

			glBindTexture(GL_TEXTURE_2D, 0);

		}
	}

	void GLES2Texture::ReleaseResources()
	{
		glDeleteTextures(1, &texture_);
	}

	void GLES2Texture::Show(PGLES2Texture pTexture)
	{
		if(IsReady() && pTexture->IsReady())
		{
			CHECK_GL_STATUS(__FILE__, __LINE__);

            if(!pMaterial_)
            {
                pMaterial_ = PGLES2Material(new GLES2Material ());

		        PGLES2Program pProgram(new GLES2Program(vShader, fShader));
		        pMaterial_->SetProgram(pProgram);
		        pMaterial_->EnableDepthTest(false);
            }

			pMaterial_->SetTexture0(pTexture);

			if(pMaterial_->IsReady())
			{
				GLES2Camera* pCurrent = GLES2Camera::Deactivate();

				pMaterial_->Render(true, nullptr, pMesh_.get());

				GLES2Camera::Activate(pCurrent);
			}

			CHECK_GL_STATUS(__FILE__, __LINE__);
		}
	}

	BindTexture::BindTexture(GLES2Texture& obj)
	: obj_(obj)
	{
		obj.Bind();
	}

	BindTexture::~BindTexture()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

}