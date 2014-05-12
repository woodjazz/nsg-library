#include "GLES2Texture.h"
#include "GLES2PlaneMesh.h"
#include "GLES2Camera.h"
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
	isLoaded_(false),
    pMaterial_(new GLES2Material ()),
	pMesh_(new GLES2PlaneMesh(2, 2, 2, 2, GL_STATIC_DRAW)),
	width_(0),
	height_(0)
	{
		InitializeCommonSettings();

		glGenTextures(1, &texture_);
		glBindTexture(GL_TEXTURE_2D, texture_);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	}

	GLES2Texture::GLES2Texture(GLint format, GLenum type, GLsizei width, GLsizei height, const GLvoid* pixels) 
	: texture_(0), 
	isLoaded_(true),
    pMaterial_(new GLES2Material ()),
    pMesh_(new GLES2PlaneMesh(2, 2, 2, 2, GL_STATIC_DRAW)),
	width_(width),
	height_(height)
	{
		InitializeCommonSettings();

		glGenTextures(1, &texture_);
		glBindTexture(GL_TEXTURE_2D, texture_);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D,
			0,
			format,
			width,
			height,
			0,
			format,
			type,
			pixels);
	}

	void GLES2Texture::InitializeCommonSettings()
	{
		PGLES2Program pProgram(new GLES2Program(vShader, fShader));
		pMaterial_->SetProgram(pProgram);
		pMaterial_->EnableDepthTest(false);
	}

	GLES2Texture::~GLES2Texture()
	{
		glDeleteTextures(1, &texture_);
	}

	bool GLES2Texture::IsReady()
	{
		if(!isLoaded_ && pResource_->IsReady())
		{
			int img_width = 0;
			int img_height = 0;
            int channels = 0;
			unsigned char* img = SOIL_load_image_from_memory(pResource_->GetData(), pResource_->GetBytes(), &img_width, &img_height, &channels, 0);

			width_ = img_width;
			height_ = img_height;

            GLint internalformat = GL_RGBA;

            if(channels == 4)
            {
                internalformat = GL_RGBA;
            }
            else if(channels == 3)
            {
                internalformat = GL_RGB;
            }
            else
            {
                internalformat = GL_RGB;
                TRACE_LOG(pResource_->GetFilename() << " Unknown internalformat");
            }

            glBindTexture(GL_TEXTURE_2D, texture_);

			glTexImage2D(GL_TEXTURE_2D,
				0,
				internalformat,
				img_width,
				img_height,
				0,
				internalformat,
				GL_UNSIGNED_BYTE,
				img);

			SOIL_free_image_data(img);

			pResource_ = nullptr;

			isLoaded_ = true;
		}

		return isLoaded_;
	}

	void GLES2Texture::Show(PGLES2Texture pTexture)
	{
		if(pTexture->IsReady())
		{
			CHECK_GL_STATUS(__FILE__, __LINE__);

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