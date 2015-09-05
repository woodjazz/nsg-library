#include "Font.h"
#include "Check.h"
#include "Graphics.h"
#include "Camera.h"
#include "Program.h"
#include "Pass.h"
#include "VertexBuffer.h"
#define Font_IMPLEMENTATION    // Expands implementation
#include "fontstash.h"

namespace NSG
{
	Font::Font(PWindow window, int width, int height, int flags)
        : window_(window),
          context_(nullptr),
          graphics_(Graphics::GetPtr()),
          pass_(std::make_shared<Pass>()),
          program_(Program::GetOrCreate("IMGUI\n")),
          tex_(0),
          width_(0),
          height_(0)
    {
        auto currentCamera = graphics_->GetCamera();
        camera_ = std::make_shared<Camera>("FontCamera");
        graphics_->SetCamera(currentCamera);

        pass_->SetBlendMode(BLEND_MODE::ALPHA);
        pass_->EnableScissorTest(false);
        pass_->EnableDepthTest(false);
        pass_->SetCullFace(CullFaceMode::DISABLED);

        camera_->EnableOrtho();

		Create(width, height, flags);
    }

    Font::~Font()
    {
		Destroy();
    }

    int Font::RenderCreate(void* userPtr, int width, int height)
    {
		auto pThis = (Font*)userPtr;
		return pThis->InternalCreate(width, height);
    }

	int Font::InternalCreate(int width, int height)
	{
		if (tex_ != 0)
		{
			glDeleteTextures(1, &tex_);
			tex_ = 0;
		}

		glGenTextures(1, &tex_);
		if (!tex_) return 0;
		width_ = width;
		height_ = height;
		graphics_->SetTexture(0, tex_);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, width, height, 0, GL_ALPHA, GL_UNSIGNED_BYTE, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		return 1;
	}


    int Font::RenderResize(void* userPtr, int width, int height)
    {
        // Reuse create to resize too.
        return RenderCreate(userPtr, width, height);
    }

    void Font::RenderUpdate(void* userPtr, int* rect, const unsigned char* data)
    {
		auto pThis = (Font*)userPtr;
		pThis->InternalUpdate(rect, data);
    }

	void Font::InternalUpdate(int* rect, const unsigned char* data)
	{
		int w = rect[2] - rect[0];
		int h = rect[3] - rect[1];

		if (tex_ == 0) return;
		//glPushClientAttrib(GL_CLIENT_PIXEL_STORE_BIT);
		graphics_->SetTexture(0, tex_);
		//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		//glPixelStorei(GL_UNPACK_ROW_LENGTH, gl->width);
		//glPixelStorei(GL_UNPACK_SKIP_PIXELS, rect[0]);
		//glPixelStorei(GL_UNPACK_SKIP_ROWS, rect[1]);
		glTexSubImage2D(GL_TEXTURE_2D, 0, rect[0], rect[1], w, h, GL_ALPHA, GL_UNSIGNED_BYTE, data);
		//glPopClientAttrib();
	}


    void Font::RenderDraw(void* userPtr, const float* verts, const float* tcoords, const unsigned int* colors, int nverts)
    {
		auto pThis = (Font*)userPtr;
        pThis->InternalDraw(verts, tcoords, colors, nverts);
    }

    void Font::InternalDraw(const float* verts, const float* tcoords, const unsigned int* colors, int nverts)
    {
		if (tex_ == 0) return;
        camera_->SetWindow(window_.get());
        camera_->SetOrthoProjection({ 0, (float)width_, (float)height_, 0, -1, 1 });
        graphics_->SetupPass(pass_.get());
        CHECK_CONDITION(graphics_->SetProgram(program_.get()), __FILE__, __LINE__);
        graphics_->SetCamera(camera_.get());
        program_->SetVariables(false);

        graphics_->SetAttributes([&]()
        {
            auto attribLocationPosition = program_->GetAttPositionLoc();
            auto attribLocationUV = program_->GetAttTextCoordLoc0();
            auto attribLocationColor = program_->GetAttColorLoc();
            glVertexAttribPointer(attribLocationPosition, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, verts);
            glVertexAttribPointer(attribLocationUV, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, tcoords);
            glVertexAttribPointer(attribLocationColor, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(unsigned int), colors);
        });

        graphics_->SetTexture(0, tex_);
        graphics_->DrawArrays(GL_TRIANGLES, 0, nverts);
    }

    void Font::RenderDelete(void* userPtr)
    {
		auto pThis = (Font*)userPtr;
		pThis->InternalDelete();
    }

	void Font::InternalDelete()
	{
		if (tex_ != 0)
			glDeleteTextures(1, &tex_);
		tex_ = 0;
	}

    void Font::Create(int width, int height, int flags)
    {
        FONSparams params;

        memset(&params, 0, sizeof(params));
        params.width = width;
        params.height = height;
        params.flags = (unsigned char)flags;
        params.renderCreate = Font::RenderCreate;
        params.renderResize = Font::RenderResize;
        params.renderUpdate = Font::RenderUpdate;
        params.renderDraw = Font::RenderDraw;
        params.renderDelete = Font::RenderDelete;
        params.userPtr = this;

        context_ = fonsCreateInternal(&params);
    }

	void Font::Destroy()
    {
        fonsDeleteInternal(context_);
    }
}


