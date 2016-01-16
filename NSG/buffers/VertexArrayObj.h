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
#pragma once

#include "Types.h"
#include "Object.h"
#include <string>
#include <map>

namespace NSG
{
	struct VAOKey
	{
		InstanceBuffer* instancesBuffer;
		Program* program;
		Mesh* mesh;
        bool solid;
		bool operator < (const VAOKey& obj) const;
		std::string GetName() const;
	};

    class VertexArrayObj : public Object
    {
    public:
		VertexArrayObj(const VAOKey& key);
        ~VertexArrayObj();
        void Use();
        void Bind();
        static void Unbind();
        static PVertexArrayObj GetOrCreate(const VAOKey& key);
        static void Clear();
    private:
        bool IsValid() override;
        void AllocateResources() override;
        void ReleaseResources()	override;
        GLuint vao_; // vertex array object
        VAOKey key_;
        SignalEmpty::PSlot slotProgramReleased_;
        SignalEmpty::PSlot slotMeshReleased_;
		typedef std::map<VAOKey, PVertexArrayObj> VAOMap;
		static VAOMap vaoMap_;
        RenderingContext* graphics_;
    };
}
