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
#include "Image.h"
#include "Resource.h"
#include "Util.h"
#include "Check.h"
#include "Decompress.h"
#include "Path.h"
#include "Log.h"
#include "RenderingContext.h"
#include "RenderingCapabilities.h"
#include "Maths.h"
#include "image_helper.h"
#include "DDS.h"
#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "jpgd.h"
#include <cerrno>
#include <cstring>

#ifndef MAKEFOURCC
#define MAKEFOURCC(ch0, ch1, ch2, ch3) ((unsigned)(ch0) | ((unsigned)(ch1) << 8) | ((unsigned)(ch2) << 16) | ((unsigned)(ch3) << 24))
#endif
#define FOURCC_DXT1 (MAKEFOURCC('D','X','T','1'))
#define FOURCC_DXT2 (MAKEFOURCC('D','X','T','2'))
#define FOURCC_DXT3 (MAKEFOURCC('D','X','T','3'))
#define FOURCC_DXT4 (MAKEFOURCC('D','X','T','4'))
#define FOURCC_DXT5 (MAKEFOURCC('D','X','T','5'))

namespace NSG
{
    Image::Image(PResource resource)
        : Object(resource->GetName()),
          resource_(resource)
    {
        ResetState();
    }

    Image::~Image()
    {
        if (allocated_)
            stbi_image_free((void*)imgData_); // same as free
    }

    void Image::ResetState()
    {
        compressed_ = false;
        numCompressedLevels_ = 0;
        format_ = TextureFormat::UNKNOWN;
        imgData_ = nullptr;
        imgDataSize_ = 0;
        allocated_ = false;
        channels_ = 0;
        depth_ = 0;
        width_ = 0;
        height_ = 0;
    }

    bool Image::IsValid()
    {
        if (RenderingContext::GetPtr() && resource_->IsReady())
        {
            auto dataSize = resource_->GetBytes();
            return dataSize > 4; // at least we have to read the type of image
        }
        return false;
    }

    void Image::ReadResource()
    {
        auto data = resource_->GetData();
        if (memcmp(&data[0], "DDS ", 4) == 0)
            ReadDDS();
        else if (memcmp(&data[0], "\253KTX", 4) == 0)
            ReadKTX();
        else if (memcmp(&data[0], "PVR\3", 4) == 0)
            ReadPVR();
        else
            ReadGeneric();
    }

    void Image::Decompress()
    {
        CHECK_CONDITION(compressed_);
        CHECK_CONDITION(depth_ == 1);
        channels_ = 4;
        imgDataSize_ = width_ * height_ * channels_;
        unsigned char* newData = (unsigned char*)malloc(imgDataSize_);
		LOGI("Decompressing %s", resource_->GetName().c_str());
        switch (format_)
        {
            case TextureFormat::DXT1:
            case TextureFormat::DXT3:
            case TextureFormat::DXT5:
                DecompressImageDXT(newData, imgData_, width_, height_, depth_, format_);
                break;
            case TextureFormat::ETC1:
                DecompressImageETC(newData, imgData_, width_, height_);
                break;
            case TextureFormat::PVRTC_RGB_2BPP:
            case TextureFormat::PVRTC_RGBA_2BPP:
            case TextureFormat::PVRTC_RGB_4BPP:
            case TextureFormat::PVRTC_RGBA_4BPP:
                DecompressImagePVRTC(newData, imgData_, width_, height_, format_);
                break;
            default:
                CHECK_CONDITION(!"Cannot decompress unknown format!!!");
                break;
        }
        if (allocated_)
            free((void*)imgData_);
        imgData_ = newData;
        allocated_ = true;
        compressed_ = false;
        numCompressedLevels_ = 0;
        format_ = TextureFormat::RGBA;
    }

    void Image::AllocateResources()
    {
        ReadResource();

        if (compressed_ && RenderingContext::GetPtr()->NeedsDecompress(format_))
            Decompress();

        if (!compressed_ && !RenderingContext::GetPtr()->IsTextureSizeCorrect(width_, height_))
            Resize2PowerOf2();

        auto maxTextureSize = RenderingCapabilities::GetPtr()->GetMaxTextureSize();
        if (maxTextureSize < width_ || maxTextureSize < height_)
            Reduce(maxTextureSize);
    }

    void Image::ReleaseResources()
    {
        if (allocated_)
            stbi_image_free((void*)imgData_); // same as free
        ResetState();
    }

    void Image::ReadGeneric()
    {
        auto data = (const unsigned char*)resource_->GetData();
        auto dataSize = resource_->GetBytes();

        imgData_ = stbi_load_from_memory(data, dataSize, &width_, &height_, &channels_, 0);
        if (!imgData_)
        {
            imgData_ = jpgd::decompress_jpeg_image_from_memory(data, dataSize, &width_, &height_, &channels_, 4);
            if (!imgData_)
            {
				LOGE("Resource=%s failed with reason:%s", resource_->GetName().c_str(), stbi_failure_reason());
            }
        }

        if (imgData_)
        {
            CHECK_ASSERT((channels_ == 4 || channels_ == 3 || channels_ == 1) && "Incorrect number of channels");
            compressed_ = false;
            allocated_ = true;
            numCompressedLevels_ = 0;
            depth_ = 1;
            if (channels_ == 4)
                format_ = TextureFormat::RGBA;
            else if (channels_ == 3)
                format_ = TextureFormat::RGB;
            else
            {
                CHECK_ASSERT(channels_ == 1);
                format_ = TextureFormat::ALPHA;
            }
        }
    }

    void Image::ReadDDS()
    {
        const int DDS_DATA_START = 4 + sizeof(DDSurfaceDesc2);
        auto data = resource_->GetData();
        auto dataSize = resource_->GetBytes();
        CHECK_ASSERT(dataSize > DDS_DATA_START);
        CHECK_ASSERT(memcmp(&data[0], "DDS ", 4) == 0);
        DDSurfaceDesc2 ddsd;
        memcpy(&ddsd, &data[4], sizeof(ddsd));
        CHECK_CONDITION(ddsd.ddpfPixelFormat_.dwFourCC_ != 0 && "Uncompressed DDS image is not supported.!!!");
        switch (ddsd.ddpfPixelFormat_.dwFourCC_)
        {
            case FOURCC_DXT1:
                format_ = TextureFormat::DXT1;
                channels_ = 3;
                break;
            case FOURCC_DXT3:
                format_ = TextureFormat::DXT3;
                channels_ = 4;
                break;
            case FOURCC_DXT5:
                format_ = TextureFormat::DXT5;
                channels_ = 4;
                break;
            default:
                CHECK_CONDITION(!"Unrecognized DDS image format.!!!");
                break;
        }
        imgData_ = (unsigned char*)&data[DDS_DATA_START];
        imgDataSize_ = dataSize - DDS_DATA_START;
        width_ = ddsd.dwWidth_;
        height_ = ddsd.dwHeight_;
        depth_ = ddsd.dwDepth_;
        if (!depth_)
            depth_ = 1;
        numCompressedLevels_ = ddsd.dwMipMapCount_;
        if (!numCompressedLevels_)
            numCompressedLevels_ = 1;
        compressed_ = true;
        allocated_ = false;
    }

    void Image::ReadKTX()
    {
        auto data = resource_->GetData();
        auto dataSize = resource_->GetBytes();
        CHECK_ASSERT(memcmp(&data[0], "\253KTX", 4) == 0);
        const int KTX_DATA_START = 12;
        CHECK_ASSERT(dataSize > KTX_DATA_START);
        unsigned idx = KTX_DATA_START;
        unsigned endianness;
        memcpy(&endianness, &data[idx], sizeof(endianness));
        idx += sizeof(endianness);
        unsigned type;
        memcpy(&type, &data[idx], sizeof(type));
        idx += sizeof(type);
        /* unsigned typeSize = */ idx += sizeof(unsigned);
        unsigned format;
        memcpy(&format, &data[idx], sizeof(format));
        idx += sizeof(format);
        unsigned internalFormat;
        memcpy(&internalFormat, &data[idx], sizeof(internalFormat));
        idx += sizeof(internalFormat);
        /* unsigned baseInternalFormat = */ idx += sizeof(unsigned);
        unsigned width;
        memcpy(&width, &data[idx], sizeof(width));
        idx += sizeof(width);
        unsigned height;
        memcpy(&height, &data[idx], sizeof(height));
        idx += sizeof(height);
        unsigned depth;
        memcpy(&depth, &data[idx], sizeof(depth));
        idx += sizeof(depth);
        /* unsigned arrayElements = */ idx += sizeof(unsigned);
        unsigned faces;
        memcpy(&faces, &data[idx], sizeof(faces));
        idx += sizeof(faces);
        unsigned mipmaps;
        memcpy(&mipmaps, &data[idx], sizeof(mipmaps));
        idx += sizeof(mipmaps);
        unsigned keyValueBytes;
        memcpy(&keyValueBytes, &data[idx], sizeof(keyValueBytes));
        idx += sizeof(keyValueBytes);

        CHECK_CONDITION(endianness == 0x04030201 && "Big-endian KTX files not supported");
        CHECK_CONDITION(type == 0 && format == 0 && "Uncompressed KTX files not supported");
        CHECK_CONDITION(faces <= 1 && depth <= 1 && "3D or cube KTX files not supported");
        CHECK_CONDITION(mipmaps != 0 && "KTX files without explicitly specified mipmap count not supported");

        format_ = TextureFormat::UNKNOWN;
        switch (internalFormat)
        {
            case 0x83f1:
                format_ = TextureFormat::DXT1;
                channels_ = 4;
                break;
            case 0x83f2:
                format_ = TextureFormat::DXT3;
                channels_ = 4;
                break;
            case 0x83f3:
                format_ = TextureFormat::DXT5;
                channels_ = 4;
                break;
            case 0x8d64:
                format_ = TextureFormat::ETC1;
                channels_ = 3;
                break;
            case 0x8c00:
                format_ = TextureFormat::PVRTC_RGB_4BPP;
                channels_ = 3;
                break;
            case 0x8c01:
                format_ = TextureFormat::PVRTC_RGB_2BPP;
                channels_ = 3;
                break;
            case 0x8c02:
                format_ = TextureFormat::PVRTC_RGBA_4BPP;
                channels_ = 4;
                break;
            case 0x8c03:
                format_ = TextureFormat::PVRTC_RGBA_2BPP;
                channels_ = 4;
                break;
        }

        CHECK_CONDITION(format_ != TextureFormat::UNKNOWN && "Unsupported texture format in KTX file");
        idx += keyValueBytes;
        imgDataSize_ = dataSize - idx - mipmaps * sizeof(unsigned);
        imgData_ = (unsigned char*)malloc(imgDataSize_);
		CHECK_ASSERT(imgData_);
        width_ = width;
        height_ = height;
        numCompressedLevels_ = mipmaps;
        depth_ = 1;
        compressed_ = true;
        allocated_ = true;

        unsigned dataOffset = 0;
        for (unsigned i = 0; i < mipmaps; ++i)
        {
            unsigned levelSize;
            memcpy(&levelSize, &data[idx], sizeof(levelSize));
            idx += sizeof(levelSize);
            CHECK_CONDITION(levelSize + dataOffset <= imgDataSize_ && "KTX mipmap level data size exceeds file size");
            memcpy((void*)&imgData_[dataOffset], &data[idx], levelSize);
            idx += levelSize;
            dataOffset += levelSize;
            if (idx & 3)
                idx = (idx + 3) & 0xfffffffc;
        }
    }

    void Image::ReadPVR()
    {
        auto data = resource_->GetData();
        auto dataSize = resource_->GetBytes();
        CHECK_ASSERT(memcmp(&data[0], "PVR\3", 4) == 0);
        const int PVR_DATA_START = 4;
        CHECK_ASSERT(dataSize > PVR_DATA_START);
        unsigned idx = PVR_DATA_START;
        /* unsigned flags = */ idx += sizeof(unsigned);
        unsigned pixelFormatLo;
        memcpy(&pixelFormatLo, &data[idx], sizeof(pixelFormatLo));
        idx += sizeof(pixelFormatLo);
        /* unsigned pixelFormatHi = */ idx += sizeof(unsigned);
        /* unsigned colourSpace = */ idx += sizeof(unsigned);
        /* unsigned channelType = */ idx += sizeof(unsigned);
        unsigned height;
        memcpy(&height, &data[idx], sizeof(height));
        idx += sizeof(height);
        unsigned width;
        memcpy(&width, &data[idx], sizeof(width));
        idx += sizeof(width);
        unsigned depth;
        memcpy(&depth, &data[idx], sizeof(depth));
        idx += sizeof(depth);
        /* unsigned numSurfaces = */ idx += sizeof(unsigned);
        unsigned faces;
        memcpy(&faces, &data[idx], sizeof(faces));
        idx += sizeof(faces);
        unsigned mipmaps;
        memcpy(&mipmaps, &data[idx], sizeof(mipmaps));
        idx += sizeof(mipmaps);
        unsigned metaDataSize;
        memcpy(&metaDataSize, &data[idx], sizeof(metaDataSize));
        idx += sizeof(metaDataSize);
        CHECK_CONDITION(faces <= 1 && depth <= 1 && "3D or cube PVR files not supported");
        CHECK_CONDITION(mipmaps != 0 && "PVR files without explicitly specified mipmap count not supported");
        format_ = TextureFormat::UNKNOWN;
        switch (pixelFormatLo)
        {
            case 0:
                format_ = TextureFormat::PVRTC_RGB_2BPP;
                channels_ = 3;
                break;
            case 1:
                format_ = TextureFormat::PVRTC_RGBA_2BPP;
                channels_ = 4;
                break;
            case 2:
                format_ = TextureFormat::PVRTC_RGB_4BPP;
                channels_ = 3;
                break;
            case 3:
                format_ = TextureFormat::PVRTC_RGBA_4BPP;
                channels_ = 4;
                break;
            case 6:
                format_ = TextureFormat::ETC1;
                channels_ = 3;
                break;
            case 7:
                format_ = TextureFormat::DXT1;
                channels_ = 4;
                break;
            case 9:
                format_ = TextureFormat::DXT3;
                channels_ = 4;
                break;
            case 11:
                format_ = TextureFormat::DXT5;
                channels_ = 4;
                break;
        }

        CHECK_CONDITION(format_ != TextureFormat::UNKNOWN && "Unsupported texture format in KTX file");

        idx += metaDataSize;
        imgDataSize_ = dataSize - idx;
        imgData_ = (unsigned char*)malloc(imgDataSize_);
        memcpy((void*)&imgData_[0], &data[idx], imgDataSize_);
        width_ = width;
        height_ = height;
        numCompressedLevels_ = mipmaps;
        depth_ = 1;
        compressed_ = true;
        allocated_ = true;
    }

    unsigned Image::GetRowDataSize() const
    {
        switch (format_)
        {
            case TextureFormat::DXT1:
                return ((width_ + 3) >> 2) * 8;
            case TextureFormat::DXT3:
            case TextureFormat::DXT5:
                return ((width_ + 3) >> 2) * 16;
            case TextureFormat::ETC1:
                return ((width_ + 3) >> 2) * 8;
            case TextureFormat::PVRTC_RGB_2BPP:
            case TextureFormat::PVRTC_RGBA_2BPP:
                return (width_ * 2 + 7) >> 3;
            case TextureFormat::PVRTC_RGB_4BPP:
            case TextureFormat::PVRTC_RGBA_4BPP:
                return (width_ * 4 + 7) >> 3;
            default:
                CHECK_ASSERT(false && "Unknown compressed format!!!");
                return 0;
        }
    }

    unsigned Image::GetCompressedDataSize() const
    {
        switch (format_)
        {
            case TextureFormat::DXT1:
            case TextureFormat::DXT3:
            case TextureFormat::DXT5:
            case TextureFormat::ETC1:
                return GetRowDataSize() * ((height_ + 3) >> 2);
            case TextureFormat::PVRTC_RGB_2BPP:
            case TextureFormat::PVRTC_RGBA_2BPP:
                return (std::max(width_, 16) * std::max(height_, 8) * 2 + 7) >> 3;
            case TextureFormat::PVRTC_RGB_4BPP:
            case TextureFormat::PVRTC_RGBA_4BPP:
                return (std::max(width_, 8) * std::max(height_, 8) * 4 + 7) >> 3;
            default:
                CHECK_ASSERT(false && "Unknown compressed format!!!");
                return 0;
        }
    }

    Image::CompressedLevel Image::GetCompressedLevel(const unsigned char* data, unsigned dataSize, unsigned index) const
    {
        CHECK_ASSERT(index < numCompressedLevels_);
        CompressedLevel level;
        level.width_ = width_;
        level.height_ = height_;
        level.depth_ = depth_;

        if (format_ >= TextureFormat::DXT1 && format_ <= TextureFormat::ETC1)
        {
            level.blockSize_ = format_ == TextureFormat::DXT1 || format_ == TextureFormat::ETC1 ? 8 : 16;
            unsigned i = 0;
            unsigned offset = 0;
            for (;;)
            {
                if (!level.width_)
                    level.width_ = 1;
                if (!level.height_)
                    level.height_ = 1;
                if (!level.depth_)
                    level.depth_ = 1;

                level.rowSize_ = ((level.width_ + 3) / 4) * level.blockSize_;
                level.rows_ = ((level.height_ + 3) / 4);
                level.data_ = data + offset;
                level.dataSize_ = level.depth_ * level.rows_ * level.rowSize_;

                CHECK_ASSERT(offset + level.dataSize_ <= dataSize);

                if (i == index)
                    return level;

                offset += level.dataSize_;
                level.width_ /= 2;
                level.height_ /= 2;
                level.depth_ /= 2;
                ++i;
            }
        }
        else
        {
            CHECK_ASSERT(format_ > TextureFormat::ETC1);
            level.blockSize_ = format_ < TextureFormat::PVRTC_RGB_4BPP ? 2 : 4;
            unsigned i = 0;
            unsigned offset = 0;
            for (;;)
            {
                if (!level.width_)
                    level.width_ = 1;
                if (!level.height_)
                    level.height_ = 1;

                int dataWidth = std::max(level.width_, level.blockSize_ == 2 ? 16 : 8);
                int dataHeight = std::max(level.height_, 8);
                level.data_ = data + offset;
                level.dataSize_ = (dataWidth * dataHeight * level.blockSize_ + 7) >> 3;
                level.rows_ = dataHeight;
                level.rowSize_ = level.dataSize_ / level.rows_;

                CHECK_ASSERT(offset + level.dataSize_ <= dataSize);

                if (i == index)
                    return level;

                offset += level.dataSize_;
                level.width_ /= 2;
                level.height_ /= 2;
                ++i;
            }
        }
    }

    void Image::FlipBlockVertical(unsigned char* dest, const unsigned char* src, TextureFormat format)
    {
        CHECK_ASSERT(format == TextureFormat::DXT1 || format == TextureFormat::DXT3 || format == TextureFormat::DXT5);

        switch (format)
        {
            case TextureFormat::DXT1:
                for (unsigned i = 0; i < 4; ++i)
                {
                    dest[i] = src[i];
                    dest[i + 4] = src[7 - i];
                }
                break;

            case TextureFormat::DXT3:
                for (unsigned i = 0; i < 8; i += 2)
                {
                    dest[i] = src[6 - i];
                    dest[i + 1] = src[6 - i + 1];
                }
                for (unsigned i = 0; i < 4; ++i)
                {
                    dest[i + 8] = src[i + 8];
                    dest[i + 12] = src[15 - i];
                }
                break;

            case TextureFormat::DXT5:
                dest[0] = src[0];
                dest[1] = src[1];
                {
                    unsigned a1 = src[2] | ((unsigned)src[3] << 8) | ((unsigned)src[4] << 16);
                    unsigned a2 = src[5] | ((unsigned)src[6] << 8) | ((unsigned)src[7] << 16);
                    unsigned b1 = ((a1 & 0x000fff) << 12) | (a1 & 0xfff000) >> 12;
                    unsigned b2 = ((a2 & 0x000fff) << 12) | (a2 & 0xfff000) >> 12;
                    dest[2] = b2 & 0xff;
                    dest[3] = (b2 >> 8) & 0xff;
                    dest[4] = (b2 >> 16) & 0xff;
                    dest[5] = b1 & 0xff;
                    dest[6] = (b1 >> 8) & 0xff;
                    dest[7] = (b1 >> 16) & 0xff;
                }
                for (unsigned i = 0; i < 4; ++i)
                {
                    dest[i + 8] = src[i + 8];
                    dest[i + 12] = src[15 - i];
                }
                break;

            default:
                CHECK_CONDITION(!"Format not supported!!!");
                break;
        }
    }

    GLint Image::ConvertFormat2GL() const
    {
        GLint result = GL_RGBA;
        switch (format_)
        {
            case TextureFormat::ALPHA:
                result = GL_ALPHA;
                break;
            case TextureFormat::RGB:
                result = GL_RGB;
                break;
            case TextureFormat::RGBA:
                result = GL_RGBA;
                break;
            case TextureFormat::DEPTH:
                result = GL_DEPTH_COMPONENT;
                break;
            case TextureFormat::DXT1:
                result = GL_COMPRESSED_RGB_S3TC_DXT1_EXT;
                break;
            case TextureFormat::ETC1:
                result = GL_ETC1_RGB8_OES;
                break;                
            case TextureFormat::DXT3:
                result = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
                break;
            case TextureFormat::DXT5:
                result = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
                break;
            case TextureFormat::PVRTC_RGB_2BPP:
                result = GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG;
                break;
            case TextureFormat::PVRTC_RGBA_2BPP:
                result = GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG;
                break;
            case TextureFormat::PVRTC_RGB_4BPP:
                result = GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG;
                break;
            case TextureFormat::PVRTC_RGBA_4BPP:
                result = GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG;
                break;
            default:
                CHECK_CONDITION(!"Unknown texture format!!!");
                break;
        }
        return result;
    }

    bool Image::FlipVertical()
    {
        if (compressed_)
        {
            if (format_ == TextureFormat::DXT1 || format_ == TextureFormat::DXT3 || format_ == TextureFormat::DXT5)
            {
                unsigned dataOffset = 0;
                unsigned char* inverted = (unsigned char*)malloc(imgDataSize_);

                for (unsigned i = 0; i < numCompressedLevels_; ++i)
                {
                    CompressedLevel level = GetCompressedLevel(imgData_, imgDataSize_, i);
                    CHECK_ASSERT(level.data_ && "Compressed level with no data");
                    for (unsigned y = 0; y < level.rows_; ++y)
                    {
                        const unsigned char* src = level.data_ + y * level.rowSize_;
                        unsigned char* dest = inverted + dataOffset + (level.rows_ - y - 1) * level.rowSize_;
                        for (unsigned x = 0; x < level.rowSize_; x += level.blockSize_)
                            FlipBlockVertical(dest + x, src + x, format_);
                    }
                    dataOffset += level.dataSize_;
                }
                if (allocated_)
                    free((void*)imgData_); // same as stbi_image_free
                imgData_ = inverted;
                allocated_ = true;
            }
            else
                return false;
        }
        else
        {
            unsigned char* inverted = (unsigned char*)malloc(channels_ * width_ * height_);
			if (!inverted) return false;
            memcpy(inverted, imgData_, channels_ * width_ * height_);
            for (int j = 0; j * 2 < height_; ++j )
            {
                int index1 = j * width_ * channels_;
                int index2 = (height_ - 1 - j) * width_ * channels_;
                for (int i = width_ * channels_; i > 0; --i )
                {
                    unsigned char temp = inverted[index1];
                    inverted[index1] = inverted[index2];
                    inverted[index2] = temp;
                    ++index1;
                    ++index2;
                }
            }
            if (allocated_)
                free((void*)imgData_); // same as stbi_image_free
            imgData_ = inverted;
            allocated_ = true;
        }

        return true;
    }

    void Image::Resize2PowerOf2(const unsigned char*& imgData, int& width, int& height, int channels)
    {
        int new_width = width;
        int new_height = height;
        GetPowerOfTwoValues(new_width, new_height);
        CHECK_ASSERT(new_width != width || new_height != height);
        unsigned char* resampled = (unsigned char*)malloc(channels * new_width * new_height);
        up_scale_image(imgData, width, height, channels, resampled, new_width, new_height);
        free((void*)imgData); // same as stbi_image_free
        imgData = resampled;
        width = new_width;
        height = new_height;
    }

    void Image::Resize2PowerOf2()
    {
        CHECK_CONDITION(!compressed_ && "Resize not supported for compressed images!!!");
        Image::Resize2PowerOf2(imgData_, width_, height_, channels_);
		LOGI("Image %s has been resized to power of two", name_.c_str());
    }

    void Image::Reduce(int size)
    {
        CHECK_CONDITION(!compressed_ && "Reduce not supported for compressed images!!!");

        if (!IsPowerOfTwo(width_) || !IsPowerOfTwo(height_))
            Resize2PowerOf2();

        int reduceBlockX = 1;
        int reduceBlockY = 1;
        if (width_ > size)
            reduceBlockX = width_ / size;
        if (height_ > size)
            reduceBlockY = height_ / size;
        if (reduceBlockX > 1 || reduceBlockY > 1)
        {
            auto newWidth = width_ / reduceBlockX;
            auto newHeight = height_ / reduceBlockY;
            unsigned char* newImgData = (unsigned char*)malloc( channels_ * newWidth * newHeight);
            mipmap_image(imgData_, width_, height_, channels_, newImgData, reduceBlockX, reduceBlockY);
            free((void*)imgData_);
            width_ = newWidth;
            height_ = newHeight;
            imgData_ = newImgData;
            LOGI("Image %s has been reduced to %d", name_.c_str(), size);
        }
    }

    bool Image::SaveAsPNG(const Path& outputDir)
    {
        CHECK_CONDITION_ARGS(imgData_ != nullptr && "Resource must be ready at this point!!!", resource_->GetName());
        CHECK_ASSERT(!compressed_ && "Save for compressed image is not supported!!!");
        Path oPath;
        oPath.SetPath(outputDir.GetPath());
        oPath.SetName(Path(resource_->GetName()).GetName());
        oPath.SetExtension("png");
        int result = stbi_write_png(oPath.GetFullAbsoluteFilePath().c_str(), width_, height_, channels_, imgData_, 0);
        if (!result)
        {
			LOGE("Error %s writting file %s", std::strerror(errno), oPath.GetFilePath().c_str());
            return false;
        }
        return true;
    }

}
