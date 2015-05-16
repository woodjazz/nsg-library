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
#include "Object.h"

namespace NSG
{
	class Image : public Object
	{
	public:
		Image(PResource resource);
		~Image();
        const unsigned char* GetData() const { return imgData_; }
        unsigned GetCompressedDataSize() const;
        bool FlipVertical();
        void Reduce(int size);
        void Resize2PowerOf2();
        static void Resize2PowerOf2(const unsigned char*& imgData, int& width, int& height, int channels);
        bool IsCompressed() const { return compressed_; }
        int GetWidth() const { return width_; }
        int GetHeight() const { return height_; }
        bool SaveAsPNG(const Path& outputDir);
		GLint ConvertFormat2GL() const;
        void ReadResource();
        TextureFormat GetFormat() const { return format_; }
        void Decompress();
        int GetChannels() const { return channels_; }
	private:
        bool IsValid() override;
        void AllocateResources() override;
        void ReleaseResources() override;
        struct CompressedLevel
        {
            const unsigned char* data_;
            int width_;
            int height_;
            int depth_;
            unsigned blockSize_;
            unsigned dataSize_;
            unsigned rowSize_;
            unsigned rows_;
        };
        CompressedLevel GetCompressedLevel(const unsigned char* data, unsigned dataSize, unsigned index) const;
        static void FlipBlockVertical(unsigned char* dest, const unsigned char* src, TextureFormat format);
        unsigned GetRowDataSize() const;
        void ReadGeneric();
		void ReadDDS();
        void ReadKTX();
        void ReadPVR();
        void ResetState();
	private:
		PResource resource_;
		bool compressed_;
		unsigned numCompressedLevels_;
		TextureFormat format_;
		const unsigned char* imgData_;
		unsigned imgDataSize_;
		bool allocated_;
        int channels_;
        int depth_; // (1 => 2D texture) (>1 => 3D texture)
        int width_;
        int height_;
	};
}
