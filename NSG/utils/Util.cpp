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

#include "Util.h"
#include "Constants.h"
#include "Path.h"
#include "pugixml.hpp"
#include "lz4.h"
#include <string>
#include <algorithm>
#include <cctype>
#include <fstream>
#include <sstream>
#include <string>
#ifndef WIN32
#include <unistd.h>
#include <cerrno>
#else
#include "windows.h"
#endif

namespace NSG
{
    Vector3 Lerp(const Vector3& lhs, const Vector3& rhs, float t)
    {
        return lhs * (1.0f - t) + rhs * t;
    }

    void DecomposeMatrix(const Matrix4& m, Vertex3& position, Quaternion& q, Vertex3& scale)
    {
        Vertex3 scaling(glm::length(m[0]), glm::length(m[1]), glm::length(m[2]));

        Matrix3 tmp1(glm::scale(glm::mat4(1.0f), Vertex3(1) / scaling) * m);

        q = glm::quat_cast(tmp1);

        position = Vertex3(m[3]);

        Matrix3 tmp2(glm::inverse(tmp1) * Matrix3(m));

        scale = Vertex3(tmp2[0].x, tmp2[1].y, tmp2[2].z);

        // prevent zero scale
        if(IsZeroLength(scale))
            scale = Vector3(1);
    }

    bool NSGCopyFile(const Path& source, const Path& target)
    {
        std::ifstream is(source.GetFullAbsoluteFilePath(), std::ios::binary);
        if (is.is_open())
        {
            std::ofstream os(target.GetFullAbsoluteFilePath(), std::ios::binary);
            if (os.is_open())
            {
                os << is.rdbuf();
                return true;
            }
        }
        return false;
    }

    std::istream& operator >> (std::istream& s, Vertex2& obj)
    {
        char ch(0);
        s >> ch;
        CHECK_ASSERT(ch == '[', __FILE__, __LINE__);
        s >> obj.x;
        s >> ch;
        CHECK_ASSERT(ch == ',', __FILE__, __LINE__);
        s >> obj.y;
        s >> ch;
        CHECK_ASSERT(ch == ']', __FILE__, __LINE__);

        return s;
    }

    std::istream& operator >> (std::istream& s, Vertex3& obj)
    {
        char ch(0);
        s >> ch;
        CHECK_ASSERT(ch == '[', __FILE__, __LINE__);
        s >> obj.x;
        s >> ch;
        CHECK_ASSERT(ch == ',', __FILE__, __LINE__);
        s >> obj.y;
        s >> ch;
        CHECK_ASSERT(ch == ',', __FILE__, __LINE__);
        s >> obj.z;
        s >> ch;
        CHECK_ASSERT(ch == ']', __FILE__, __LINE__);

        return s;
    }

    std::istream& operator >> (std::istream& s, Vertex4& obj)
    {
        char ch(0);
        s >> ch;
        CHECK_ASSERT(ch == '[', __FILE__, __LINE__);
        s >> obj.x;
        s >> ch;
        CHECK_ASSERT(ch == ',', __FILE__, __LINE__);
        s >> obj.y;
        s >> ch;
        CHECK_ASSERT(ch == ',', __FILE__, __LINE__);
        s >> obj.z;
        s >> ch;
        CHECK_ASSERT(ch == ',', __FILE__, __LINE__);
        s >> obj.w;
        s >> ch;
        CHECK_ASSERT(ch == ']', __FILE__, __LINE__);

        return s;
    }

    Vertex2 GetVertex2(const std::string& buffer)
    {
        std::stringstream ss;
        ss << buffer;
        Vertex2 obj;
        ss >> obj;
        return obj;
    }


    Vertex3 GetVertex3(const std::string& buffer)
    {
        std::stringstream ss;
        ss << buffer;
        Vertex3 obj;
        ss >> obj;
        return obj;
    }

    Vertex4 GetVertex4(const std::string& buffer)
    {
        std::stringstream ss;
        ss << buffer;
        Vertex4 obj;
        ss >> obj;
        return obj;
    }

    Matrix4 GetMatrix4(const std::string& buffer)
    {
        std::stringstream ss;
        ss << buffer;
        Matrix4 obj;
        ss >> obj;
        return obj;
    }

    std::string ToString(const Matrix4& m)
    {
        std::stringstream ss;
        ss << '[';
        ss << glm::column(m, 0);
        ss << glm::column(m, 1);
        ss << glm::column(m, 2);
        ss << glm::column(m, 3);
        ss << ']';

        return ss.str();
    }

    std::istream& operator >> (std::istream& s, Matrix4& obj)
    {
        char ch(0);
        s >> ch;
        CHECK_ASSERT(ch == '[', __FILE__, __LINE__);
        Vector4 col0;
        s >> col0;
        Vector4 col1;
        s >> col1;
        Vector4 col2;
        s >> col2;
        Vector4 col3;
        s >> col3;
        s >> ch;
        CHECK_ASSERT(ch == ']', __FILE__, __LINE__);

        obj = Matrix4(col0, col1, col2, col3);

        return s;
    }


    std::istream& operator >> (std::istream& s, Quaternion& obj)
    {
        char ch(0);
        s >> ch;
        CHECK_ASSERT(ch == '[', __FILE__, __LINE__);
        s >> obj.w;
        s >> ch;
        CHECK_ASSERT(ch == ',', __FILE__, __LINE__);
        s >> obj.x;
        s >> ch;
        CHECK_ASSERT(ch == ',', __FILE__, __LINE__);
        s >> obj.y;
        s >> ch;
        CHECK_ASSERT(ch == ',', __FILE__, __LINE__);
        s >> obj.z;
        s >> ch;
        CHECK_ASSERT(ch == ']', __FILE__, __LINE__);

        return s;
    }

    Quaternion GetQuaternion(const std::string& buffer)
    {
        std::stringstream ss;
        ss << buffer;
        Quaternion obj;
        ss >> obj;
        return obj;
    }

    bool SetCurrentDir(const std::string& path)
    {
        if (!path.empty())
        {
            #ifdef WIN32
            {
                if (SetCurrentDirectory(path.c_str()) == FALSE)
                {
                    TRACE_LOG("Failed to change directory to " << path << " with error = " << GetLastError());
                    return false;
                }
            }
            #else
            {
                if (chdir(path.c_str()) != 0)
                {
                    TRACE_LOG("Failed to change directory to " << path << " with error = " << errno);
                    return false;
                }
            }
            #endif
        }
        return true;
    }

    std::string GetUniqueName(const std::string& name)
    {
        static int counter = 0;
        std::stringstream ss;
        if (name.empty())
            ss << "GeneratedName";
        ss << counter++;
        return name + ss.str();
    }

    void GetPowerOfTwoValues(unsigned& width, unsigned& height)
    {
        unsigned new_width = 1;
        unsigned new_height = 1;

        while (new_width < width)
            new_width *= 2;

        while (new_height < height)
            new_height *= 2;

        width = new_width;
        height = new_height;
    }

    bool IsPowerOfTwo(unsigned value)
    {
        return (!(value & (value - 1)) && value);
    }

    bool IsZeroLength(const Vector3& obj)
    {
        auto length = glm::length(obj);
        return length <= glm::epsilon<float>();
    }

    GLushort Transform(GLubyte selected[4])
    {
        GLushort b3 = (GLushort)selected[3] / 0x10;
        GLushort b2 = (GLushort)selected[2] / 0x10;
        GLushort b1 = (GLushort)selected[1] / 0x10;
        GLushort b0 = (GLushort)selected[0] / 0x10;
        GLushort index = b3 << 12 | b2 << 8 | b1 << 4 | b0;
        return index;
    }

    Color Transform(GLushort id)
    {
        Color color;
        color[0] = (id & 0x000F) / 15.0f;
        color[1] = ((id & 0x00F0) >> 4) / 15.0f;
        color[2] = ((id & 0x0F00) >> 8) / 15.0f;
        color[3] = ((id & 0xF000) >> 12) / 15.0f;
        return color;
    }

	struct LZ4Header
	{
		char tag_[4];
		std::string::size_type bytes_;
		LZ4Header()
		{
			tag_[0] = 'L';
			tag_[1] = 'Z';
			tag_[2] = '4';
			tag_[3] = '*';
			bytes_ = 0;
		}
		LZ4Header(const std::string& buffer)
		{
			tag_[0] = 'L';
			tag_[1] = 'Z';
			tag_[2] = '4';
			tag_[3] = '*';
			bytes_ = buffer.size();
		}
		
		static bool HasHeader(const std::string& buffer, std::string::size_type& bytes)
		{
			if (buffer.size() >= sizeof(LZ4Header))
			{
				if (buffer[0] == 'L' && buffer[1] == 'Z' && buffer[2] == '4' && buffer[3] == '*')
				{
					LZ4Header header;
					memcpy(&header, &buffer[0], sizeof(header));
					//int offset = offsetof(LZ4Header, bytes_);
					bytes = header.bytes_;
					return true;
				}
			}
			return false;
		}
	};

	struct Writer : pugi::xml_writer
	{
		std::string buffer_;
		std::string compressBuffer_;
		Writer()
		{
			LZ4Header header;
			buffer_.insert(buffer_.begin(), (char*)&header, (char*)&header + sizeof(header));
		}

		void write(const void* data, size_t size) override
		{
			const char* m = (const char*)data;
			buffer_.insert(buffer_.end(), &m[0], &m[size]);
		}

		bool Compress()
		{
			compressBuffer_.resize(LZ4_compressBound(buffer_.size()));
			auto bytes = LZ4_compress(buffer_.c_str(), &compressBuffer_[0], buffer_.size());
			if (bytes >= 0)
			{
/*				LZ4Header header(buffer_);
				header.bytes_ = bytes;
				memcpy(&buffer_[0], &header, sizeof(header));*/
				bytes = LZ4_compress(buffer_.c_str(), &compressBuffer_[0], buffer_.size());
				CHECK_CONDITION(bytes > 0, __FILE__, __LINE__);
				compressBuffer_.resize(bytes);
				return true;
			}
			compressBuffer_.clear();
			return false;
		}

		bool Save(const std::string& filename)
		{
			std::ofstream os(filename, std::ios::binary);
			if (os.is_open())
			{
				os.write(&compressBuffer_[0], compressBuffer_.size());
				return true;
			}
			return false;
		}
	};

	std::string DecompressBuffer(const std::string& buffer)
	{
		std::string::size_type bytes = 0;
		if(LZ4Header::HasHeader(buffer, bytes))
		{
			std::string outputBuffer;
			outputBuffer.resize(bytes);
			int totalBytes = LZ4_decompress_safe(&buffer[0], &outputBuffer[0], buffer.size(), bytes);
			CHECK_CONDITION(totalBytes == bytes, __FILE__, __LINE__);
			return outputBuffer;
		}
		else
			return buffer;
	}


    bool SaveDocument(const std::string& filename, const pugi::xml_document& doc, bool compress)
    {
        TRACE_LOG("Saving file: " << filename);
		if (false)//compress)
		{
			Writer writer;
			doc.save(writer);
			return writer.Compress() && writer.Save(filename);
		}
		else
	        return doc.save_file(filename.c_str());
    }
}