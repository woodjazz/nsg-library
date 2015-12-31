/*
-------------------------------------------------------------------------------
This file is part of nsg-library.
http://github.com/woodjazz/nsg-library

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
#include "Types.h"
#include "Constants.h"

namespace NSG
{
    BlurFilter::BlurFilter()
        : blurDir_(1, 1),
          sigma_(3.5f)
    {
    }

    bool BlurFilter::operator != (const BlurFilter& obj) const
    {
        return blurDir_ != obj.blurDir_ || blurRadius_ != obj.blurRadius_ || sigma_ != obj.sigma_;
    }

    WaveFilter::WaveFilter()
        : factor_(PI * 8),
          offset_(0)
    {
    }

    bool WaveFilter::operator != (const WaveFilter& obj) const
    {
        return factor_ != obj.factor_ || offset_ != obj.offset_;
    }

    ShockWaveFilter::ShockWaveFilter()
        : center_(0, 0),
          time_(0),
          params_(10, 0.4f, 0.04f)
    {
    }

    bool ShockWaveFilter::operator != (const ShockWaveFilter& obj) const
    {
        return center_ != obj.center_ || time_ != obj.time_ || params_ != obj.params_;
    }

    RayNodeResult::RayNodeResult()
        : distance_(0),
          node_(nullptr)
    {
    }

    RayNodeResult::RayNodeResult(float distance, SceneNode* node)
        : distance_(distance),
          node_(node)
    {
    }

    OrthoProjection::OrthoProjection()
        : left_(0), right_(0), bottom_(0), top_(0), near_(0), far_(0)
    {
    }

    OrthoProjection::OrthoProjection(float left, float right, float bottom, float top, float near, float far)
        : left_(left), right_(right), bottom_(bottom), top_(top), near_(near), far_(far)
    {

    }
}