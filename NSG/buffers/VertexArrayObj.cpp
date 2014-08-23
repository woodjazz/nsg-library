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
#include "VertexArrayObj.h"
#include "Program.h"
#include "Mesh.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Graphics.h"
#include "Check.h"

namespace NSG
{
    VertexArrayObj::VertexArrayObj(Program* program, Mesh* mesh)
        : vao_(0),
        program_(program),
        mesh_(mesh)
    {
        CHECK_GL_STATUS(__FILE__, __LINE__);

        glGenVertexArrays(1, &vao_);

        CHECK_ASSERT(vao_ != 0, __FILE__, __LINE__);

        Redo();

        CHECK_GL_STATUS(__FILE__, __LINE__);
    }

    VertexArrayObj::~VertexArrayObj()
    {
        if (Graphics::this_->GetVertexArrayObj() == this)
            Graphics::this_->SetVertexArrayObj(nullptr);

        glDeleteVertexArrays(1, &vao_);
    }

    void VertexArrayObj::Redo()
    {
        CHECK_GL_STATUS(__FILE__, __LINE__);

        CHECK_CONDITION(Graphics::this_->SetVertexArrayObj(this), __FILE__, __LINE__);

        VertexBuffer* vertexBuffer = mesh_->GetVertexBuffer();
        Graphics::this_->SetVertexBuffer(nullptr); // force binding in VAO (not sure if needed since VBO binding is NOT part of VAO)
        Graphics::this_->SetVertexBuffer(vertexBuffer);

        GLuint position_loc = program_->GetAttPositionLoc();
        GLuint texcoord_loc = program_->GetAttTextCoordLoc();
        GLuint normal_loc = program_->GetAttNormalLoc();
        GLuint color_loc = program_->GetAttColorLoc();

        if (position_loc != -1)
            glEnableVertexAttribArray(ATTRIBUTE_LOC::POSITION);

        if (normal_loc != -1)
            glEnableVertexAttribArray(ATTRIBUTE_LOC::NORMAL);

        if (texcoord_loc != -1)
            glEnableVertexAttribArray(ATTRIBUTE_LOC::COORD);

        if (color_loc != -1)
            glEnableVertexAttribArray(ATTRIBUTE_LOC::COLOR);

        Graphics::this_->SetVertexAttrPointers();

        IndexBuffer* indexBuffer = mesh_->GetIndexBuffer();
        Graphics::this_->SetIndexBuffer(nullptr); // forces binding in VAO (this shall be needed since IBO binding is part of VAO)
        Graphics::this_->SetIndexBuffer(indexBuffer);

        Graphics::this_->SetVertexArrayObj(nullptr);

        CHECK_GL_STATUS(__FILE__, __LINE__);
    }

    void VertexArrayObj::Bind()
    {
        glBindVertexArray(vao_);
    }

    void VertexArrayObj::Unbind()
    {
        glBindVertexArray(0);
    }
}
