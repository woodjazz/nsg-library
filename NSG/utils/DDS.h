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

namespace NSG
{
    /// DirectDraw color key definition.
    struct DDColorKey
    {
        unsigned dwColorSpaceLowValue_;
        unsigned dwColorSpaceHighValue_;
    };

    /// DirectDraw pixel format definition.
    struct DDPixelFormat
    {
        unsigned dwSize_;
        unsigned dwFlags_;
        unsigned dwFourCC_;
        union
        {
            unsigned dwRGBBitCount_;
            unsigned dwYUVBitCount_;
            unsigned dwZBufferBitDepth_;
            unsigned dwAlphaBitDepth_;
            unsigned dwLuminanceBitCount_;
            unsigned dwBumpBitCount_;
            unsigned dwPrivateFormatBitCount_;
        };
        union
        {
            unsigned dwRBitMask_;
            unsigned dwYBitMask_;
            unsigned dwStencilBitDepth_;
            unsigned dwLuminanceBitMask_;
            unsigned dwBumpDuBitMask_;
            unsigned dwOperations_;
        };
        union
        {
            unsigned dwGBitMask_;
            unsigned dwUBitMask_;
            unsigned dwZBitMask_;
            unsigned dwBumpDvBitMask_;
            struct
            {
                unsigned short wFlipMSTypes_;
                unsigned short wBltMSTypes_;
            } multiSampleCaps_;
        };
        union
        {
            unsigned dwBBitMask_;
            unsigned dwVBitMask_;
            unsigned dwStencilBitMask_;
            unsigned dwBumpLuminanceBitMask_;
        };
        union
        {
            unsigned dwRGBAlphaBitMask_;
            unsigned dwYUVAlphaBitMask_;
            unsigned dwLuminanceAlphaBitMask_;
            unsigned dwRGBZBitMask_;
            unsigned dwYUVZBitMask_;
        };
    };

    /// DirectDraw surface capabilities.
    struct DDSCaps2
    {
        unsigned dwCaps_;
        unsigned dwCaps2_;
        unsigned dwCaps3_;
        union
        {
            unsigned dwCaps4_;
            unsigned dwVolumeDepth_;
        };
    };

    /// DirectDraw surface description.
    struct DDSurfaceDesc2
    {
        unsigned dwSize_;
        unsigned dwFlags_;
        unsigned dwHeight_;
        unsigned dwWidth_;
        union
        {
            unsigned lPitch_;
            unsigned dwLinearSize_;
        };
        union
        {
            unsigned dwBackBufferCount_;
            unsigned dwDepth_;
        };
        union
        {
            unsigned dwMipMapCount_;
            unsigned dwRefreshRate_;
            unsigned dwSrcVBHandle_;
        };
        unsigned dwAlphaBitDepth_;
        unsigned dwReserved_;
        unsigned lpSurface_; // Do not define as a void pointer, as it is 8 bytes in a 64bit build
        union
        {
            DDColorKey ddckCKDestOverlay_;
            unsigned dwEmptyFaceColor_;
        };
        DDColorKey ddckCKDestBlt_;
        DDColorKey ddckCKSrcOverlay_;
        DDColorKey ddckCKSrcBlt_;
        union
        {
            DDPixelFormat ddpfPixelFormat_;
            unsigned dwFVF_;
        };
        DDSCaps2 ddsCaps_;
        unsigned dwTextureStage_;
    };
}