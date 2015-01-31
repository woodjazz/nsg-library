/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Charlie C.

    Contributor(s): none yet.
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
//////////////////////////////////////////////////////////////////////////////////////////
// Updated by Néstor Silveira Gorski for nsg-library (http://nsg-library.googlecode.com/)
//////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <vector>
#include <memory>
#include "UtilConverter.h"

namespace BlenderConverter
{
    struct BezierVertex
    {
        float h1[2];
        float cp[2];
        float h2[2];
    };

    class BSpline
    {
    public:
        enum BezierInterpolation
        {
            BEZ_LINEAR,
            BEZ_CONSTANT,
            BEZ_CUBIC,
        };

    protected:
        std::vector<BezierVertex>         m_verts;
		SPLINE_CHANNEL_CODE code_;
        BezierInterpolation             m_interpMethod;

        int solveRoots(const double& x,
                       const double p0,
                       const double p1,
                       const double p2,
                       const double p3,
                       double& s) const;

        float interpolate(const double& t,
                          const double& p0,
                          const double& p1,
                          const double& p2,
                          const double& p3) const;


        void updateHandles(float* p0, float* p1, float* p2, float* p3) const;


    public:
		BSpline(SPLINE_CHANNEL_CODE code) : code_(code), m_interpMethod(BEZ_LINEAR) {}
        ~BSpline() {}

        // interpolate across this spline
        // delta is the normalized time
        // time is the current frame number
        float interpolate(float delta, float time) const;

        void addVertex(const BezierVertex& v)
        {m_verts.push_back(v);}

        const BezierVertex* getVerts(void) const
        {return &m_verts[0];}

        int getNumVerts(void) const
        {return (int)m_verts.size();}

        ///Gets the code bound to this spline.
        ///ie; where to apply results, loc, rot, ...
		SPLINE_CHANNEL_CODE GetCode(void) const
        { return code_ ;}

        void setInterpolationMethod(const BezierInterpolation& meth)
        { m_interpMethod = meth; }

        BezierInterpolation getInterpolationMethod(void) const
        { return m_interpMethod; }
    };

	typedef std::shared_ptr<BSpline> PBSpline;
}
