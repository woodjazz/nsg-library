/*
-------------------------------------------------------------------------------
This file is part of nsg-library.
http://github.com/woodjazz/nsg-library

Copyright (c) 2014-2017 Néstor Silveira Gorski

-------------------------------------------------------------------------------
This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The  this software must not be misrepresented; you must not
claim that you wrote the software. If you use this software
in a product, an acknowledgment in the product documentation would be
appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
misrepresented as being the software.
3. This notice may not be removed or altered from any source distribution.
-------------------------------------------------------------------------------
*/

#include "FrustumMesh.h"
#include "Frustum.h"

namespace NSG
{
    FrustumMesh::FrustumMesh(const std::string& name)
        : LinesMesh(name)
    {

    }

    void FrustumMesh::SetFrustum(PFrustum frustum, const Color& color)
    {
        if (frustum_ != frustum || color_ != color)
        {
            frustum_ = frustum;
            color_ = color;

            Clear();

            for (int i = 0; i < NUM_FRUSTUM_FACES; i++)
            {
				auto& face = frustum->GetFace((FrustumFace)i);
                Add(face.vertices[0], face.vertices[1], color);
                Add(face.vertices[1], face.vertices[2], color);
                Add(face.vertices[2], face.vertices[3], color);
                Add(face.vertices[3], face.vertices[0], color);
            }
        }
    }

    bool FrustumMesh::IsValid()
    {
        return frustum_ && LinesMesh::IsValid();
    }
}
