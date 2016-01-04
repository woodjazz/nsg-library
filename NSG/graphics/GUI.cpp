/*
-------------------------------------------------------------------------------
This file is part of nsg-library.
http://github.com/woodjazz/nsg-library

Copyright (c) 2014-2016 Néstor Silveira Gorski

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
#include "GUI.h"
#include "imgui.h"
#include "Check.h"
#include "Graphics.h"
#include "Camera.h"
#include "Texture2D.h"
#include "Window.h"
#include "Engine.h"
#include "Pass.h"
#include "Keys.h"
#include "Program.h"
#include "Camera.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "StringConverter.h"
#include <map>

namespace NSG
{
    template<> std::map<std::string, PGUI> StrongFactory<std::string, GUI>::objsMap_ = std::map<std::string, PGUI> {};

    GUI::GUI(const std::string& name)
        : Object(name),
          graphics_(Graphics::GetPtr()),
          fontTexture_(std::make_shared<Texture2D>(name + "GUIFontTexture")),
          pass_(std::make_shared<Pass>()),
          program_(Program::GetOrCreate("IMGUI\n")),
          vBuffer_(std::make_shared<VertexBuffer>(GL_STREAM_DRAW)),
          iBuffer_(std::make_shared<IndexBuffer>(GL_STREAM_DRAW)),
          window_(nullptr),
          state_(new char[ImGui::GetInternalStateSize()]),
          configured_(false)
    {
        ImGui::SetInternalState(state_, true);

        camera_ = std::make_shared<Camera>(name + "IMGUICamera");
        program_->Set(camera_.get());

        pass_->SetBlendMode(BLEND_MODE::ALPHA);
        pass_->EnableScissorTest(false);
        pass_->EnableDepthTest(false);
        pass_->SetCullFace(CullFaceMode::DISABLED);

        camera_->EnableOrtho();
        ImGuiIO& io = ImGui::GetIO();
        io.UserData = this;
        io.RenderDrawListsFn = [](ImDrawData * draw_data) {static_cast<GUI*>(ImGui::GetIO().UserData)->InternalDraw(draw_data); };

        unsigned char* pixels;
        int width, height;
        io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);
        fontTexture_->SetData(pixels);
        fontTexture_->SetWrapMode(TextureWrapMode::REPEAT);
        fontTexture_->SetSize(width, height);

        slotTextureReleased_ = fontTexture_->SigReleased()->Connect([this]()
        {
            ImGui::SetInternalState(state_);
            unsigned char* pixels;
            int width, height;
            ImGuiIO& io = ImGui::GetIO();
            io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);
            fontTexture_->SetData(pixels);
            fontTexture_->SetWrapMode(TextureWrapMode::REPEAT);
            fontTexture_->SetSize(width, height);
        });

        slotTextureAllocated_ = fontTexture_->SigAllocated()->Connect([this]()
        {
            ImGui::SetInternalState(state_);
            ImGuiIO& io = ImGui::GetIO();
            // Store our identifier
            io.Fonts->TexID = (void*)(intptr_t)fontTexture_->GetID();
            // Cleanup (don't clear the input data if you want to append new fonts later)
            io.Fonts->ClearInputData();
            io.Fonts->ClearTexData();
        });

        CHECK_CONDITION(fontTexture_->IsReady());
    }

    GUI::~GUI()
    {
        ImGui::SetInternalState(state_);
        ImGui::Shutdown();
        delete[] state_;
    }

	bool GUI::IsValid()
	{
		return fontTexture_->IsReady();
	}

    void GUI::InternalDraw(ImDrawData* draw_data)
    {
        CHECK_GL_STATUS();

        if (!fontTexture_->IsReady())
            return;

        program_->Set(camera_.get());
        auto& io = ImGui::GetIO();
        const float width = io.DisplaySize.x;
        const float height = io.DisplaySize.y;
        camera_->SetWindow(window_);
        camera_->SetOrthoProjection({ 0, width, height, 0, -1, 1 });
        graphics_->SetupPass(pass_.get());
        CHECK_CONDITION(graphics_->SetProgram(program_.get()));
        graphics_->SetVertexBuffer(vBuffer_.get());
        graphics_->SetIndexBuffer(iBuffer_.get());
        program_->SetVariables(false);

        graphics_->SetVertexBuffer(vBuffer_.get());
        graphics_->SetAttributes([&]()
        {
            auto attribLocationPosition = program_->GetAttPositionLoc();
            auto attribLocationUV = program_->GetAttTextCoordLoc0();
            auto attribLocationColor = program_->GetAttColorLoc();

#define OFFSETOF(TYPE, ELEMENT) ((size_t)&(((TYPE *)0)->ELEMENT))
            glVertexAttribPointer(attribLocationPosition, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, pos));
            glVertexAttribPointer(attribLocationUV, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, uv));
            glVertexAttribPointer(attribLocationColor, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, col));
#undef OFFSETOF
        });

        for (int n = 0; n < draw_data->CmdListsCount; n++)
        {
            const ImDrawList* cmd_list = draw_data->CmdLists[n];
            const ImDrawIdx* idx_buffer_offset = 0;
            vBuffer_->SetData((GLsizeiptr)cmd_list->VtxBuffer.size() * sizeof(ImDrawVert), (GLvoid*)&cmd_list->VtxBuffer.front());
            iBuffer_->SetData((GLsizeiptr)cmd_list->IdxBuffer.size() * sizeof(ImDrawIdx), (GLvoid*)&cmd_list->IdxBuffer.front());
            for (const ImDrawCmd* pcmd = cmd_list->CmdBuffer.begin(); pcmd != cmd_list->CmdBuffer.end(); pcmd++)
            {
                if (pcmd->UserCallback)
                {
                    pcmd->UserCallback(cmd_list, pcmd);
                }
                else
                {
                    graphics_->SetTexture(0, (GLuint)(intptr_t)pcmd->TextureId);
                    graphics_->SetScissorTest(true, (int)pcmd->ClipRect.x, (int)(height - pcmd->ClipRect.w), (int)(pcmd->ClipRect.z - pcmd->ClipRect.x), (int)(pcmd->ClipRect.w - pcmd->ClipRect.y));
                    graphics_->DrawElements(GL_TRIANGLES, (GLsizei)pcmd->ElemCount, GL_UNSIGNED_SHORT, idx_buffer_offset);
                }
                idx_buffer_offset += pcmd->ElemCount;
            }
        }
        CHECK_GL_STATUS();
    }

    void GUI::Render(Window* window, std::function<void(void)> callback)
    {
        if (!window || !IsReady())
            return;
        Setup();
        ImGui::SetInternalState(state_);
        window_ = window;
        window->BeginImguiRender();
        ImGuiIO& io = ImGui::GetIO();
        auto width = (float)window->GetWidth();
        auto height = (float)window->GetHeight();

        if (area_ != Rect(0))
            io.DisplaySize = ImVec2{ Clamp(area_.z, 0.f, width), Clamp(area_.w, 0.f, height) };
        else
            io.DisplaySize = ImVec2{ width, height };

        io.DeltaTime = Engine::GetPtr()->GetDeltaTime();
        ImGui::NewFrame();
        callback();
        ImGui::Render();
    }

    void GUI::Setup()
    {
        if (configured_)
            return;

        configured_ = true;

        auto mainWindow = Window::GetMainWindow();

        mainWindow->SetupImgui();

        slotKey_ = mainWindow->SigKey()->Connect([this](int key, int action, int modifier)
        {
            ImGui::SetInternalState(state_);
            ImGuiIO& io = ImGui::GetIO();
            io.KeysDown[key] = action ? true : false;
            io.KeyShift = (modifier & NSG_KEY_MOD_SHIFT) != 0;
            io.KeyCtrl = (modifier & NSG_KEY_MOD_CONTROL) != 0;
            io.KeyAlt = (modifier & NSG_KEY_MOD_ALT) != 0;
        });

        slotText_ = mainWindow->SigText()->Connect([this](std::string text)
        {
            ImGui::SetInternalState(state_);
            ImGuiIO& io = ImGui::GetIO();
            io.AddInputCharactersUTF8(text.c_str());
        });

        slotMouseMoved_ = mainWindow->SigMouseMoved()->Connect([this](int x, int y)
        {
            ImGui::SetInternalState(state_);
            ImGuiIO& io = ImGui::GetIO();
            io.MousePos = ImVec2((float)x - area_.x, (float)y - area_.y);
            //LOGI("%s", ToString(area_).c_str());
        });

        slotMouseDown_ = mainWindow->SigMouseDownInt()->Connect([this](int button, int x, int y)
        {
            ImGui::SetInternalState(state_);
            ImGuiIO& io = ImGui::GetIO();
            if (button == NSG_BUTTON_LEFT)
                io.MouseDown[0] = true;
            else if (button == NSG_BUTTON_RIGHT)
                io.MouseDown[1] = true;
            else if (button == NSG_BUTTON_MIDDLE)
                io.MouseDown[2] = true;
            io.MousePos = ImVec2((float)x - area_.x, (float)y - area_.y);
        });

        slotMouseUp_ = mainWindow->SigMouseUpInt()->Connect([this](int button, int x, int y)
        {
            ImGui::SetInternalState(state_);
            ImGuiIO& io = ImGui::GetIO();
            if (button == NSG_BUTTON_LEFT)
                io.MouseDown[0] = false;
            else if (button == NSG_BUTTON_RIGHT)
                io.MouseDown[1] = false;
            else if (button == NSG_BUTTON_MIDDLE)
                io.MouseDown[2] = false;
            io.MousePos = ImVec2((float)x - area_.x, (float)y - area_.y);
        });

        slotMouseWheel_ = mainWindow->SigMouseWheel()->Connect([this](float x, float y)
        {
            ImGui::SetInternalState(state_);
            ImGuiIO& io = ImGui::GetIO();
            if (y > 0)
                io.MouseWheel = 1;
            else if (y < 0)
                io.MouseWheel = -1;
        });

        slotMultiGesture_ = mainWindow->SigMultiGesture()->Connect([this](int timestamp, float x, float y, float dTheta, float dDist, int numFingers)
        {
            //ImGui::SetInternalState(state_);
            //ImGuiIO& io = ImGui::GetIO();
            //io.MouseDown[0] = numFingers > 0 ? true : false;
            //io.MousePos = io.MousePos = ImVec2((float)x - area_.x, (float)y - area_.y);
        });
    }

    void GUI::SetArea(const Rect& area)
    {
        area_ = area;
    }
}
