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
#include "DebugRenderer.h"
#include "LinesMesh.h"
#include "Maths.h"
#include "Vector3.h"
#include <cmath>

namespace NSG
{
    DebugRenderer::DebugRenderer()
        : lines_(std::make_shared<LinesMesh>("DebugRenderer"))
    {

    }

    DebugRenderer::~DebugRenderer()
    {

    }

    void DebugRenderer::AddLine(const Vector3& from, const Vector3& to, const Color& color)
    {
    	lines_->Add(from, to, color);
    }

    void DebugRenderer::AddSphere(const Vector3& center, float radius, const Color& color)
    {
        const int Res = 8;
        const int DoubleRes = Res * 2;
        const float polarInc = PI / Res; // ringAngle
        const float azimInc = TWO_PI / DoubleRes; // segAngle

        std::vector<Vector3> v;
        v.reserve(DoubleRes * (Res + 1));

        for (float i = 0; i < Res + 1; i++)
        {
            float tr = sin( PI - i * polarInc );
            float ny = cos( PI - i * polarInc );

            for (float j = 0; j <= DoubleRes; j++)
            {
                float nx = tr * sin(j * azimInc);
                float nz = tr * cos(j * azimInc);
				auto normal = center + radius * Vertex3(nx, ny, nz);
                v.push_back(normal);
            }
        }

        const int Nr = DoubleRes + 1;

        int index1, index2, index3;

        // Triangles
        // Front Face CCW
        for (int iy = 0; iy < Res; iy++)
        {
            for (int ix = 0; ix < DoubleRes; ix++)
            {
                // first tri
                if (iy > 0)
                {
                    index1 = (iy + 0) * (Nr) + (ix + 0);
                    index2 = (iy + 0) * (Nr) + (ix + 1);
                    index3 = (iy + 1) * (Nr) + (ix + 0);

                    AddLine(v[index1], v[index2], color);
                    AddLine(v[index2], v[index3], color);
                    AddLine(v[index3], v[index1], color);
                }
            }
        }

    }

    void DebugRenderer::Clear()
    {
        lines_->Clear();
    }

}
