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
#include "Check.h"
#include "Plane.h"
#include "Path.h"
#include "StringConverter.h"
#include "pugixml.hpp"
#include "lz4.h"
#include <algorithm>
#include <cctype>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdio>
#include <thread>
#ifndef IS_TARGET_WINDOWS
#include <unistd.h>
#include <cerrno>
#else
#include <windows.h>
#define snprintf _snprintf
#endif
#if EMSCRIPTEN
#include "emscripten.h"
#endif
#include "LinearMath/btTransform.h"
#include "LinearMath/btVector3.h"
#include "LinearMath/btQuaternion.h"

namespace NSG
{
	btTransform ToTransform(const Vector3& pos, const Quaternion& rot)
	{
		btTransform trans;
		trans.setIdentity();
		trans.setOrigin(btVector3(pos.x, pos.y, pos.z));
		trans.setRotation(btQuaternion(rot.x, rot.y, rot.z, rot.w));
		return trans;
	}

	btVector3 ToBtVector3(const Vector3& obj)
	{
		return btVector3(obj.x, obj.y, obj.z);
	}

	Vector3 ToVector3(const btVector3& obj)
	{
		return Vector3(obj.x(), obj.y(), obj.z());
	}

	btQuaternion ToBtQuaternion(const Quaternion& q)
	{
		return btQuaternion(q.x, q.y, q.z, q.w);
	}

	Quaternion ToQuaternion(const btQuaternion& q)
	{
		return Quaternion(q.w(), q.x(), q.y(), q.z());
	}

	std::string GetUniqueName(const std::string& name)
	{
		static int counter = 0;
		const int MaxBuffer = 100;
		char buffer[MaxBuffer];
		if (name == "")
			snprintf(buffer, MaxBuffer, "Gen%d", counter++);
		else
			snprintf(buffer, MaxBuffer, "%s%d", name.c_str(), counter++);
		return buffer;
	}


    unsigned short Transform(unsigned char selected[4])
	{
        auto b3 = (unsigned short)selected[3] / 0x10;
        auto b2 = (unsigned short)selected[2] / 0x10;
        auto b1 = (unsigned short)selected[1] / 0x10;
        auto b0 = (unsigned short)selected[0] / 0x10;
        auto index = b3 << 12 | b2 << 8 | b1 << 4 | b0;
		return index;
	}

    Color Transform(unsigned short id)
	{
		Color color;
		color[0] = (id & 0x000F) / 15.0f;
		color[1] = ((id & 0x00F0) >> 4) / 15.0f;
		color[2] = ((id & 0x0F00) >> 8) / 15.0f;
		color[3] = ((id & 0xF000) >> 12) / 15.0f;
		return color;
	}

	static size_t HeaderSize = 128;
	std::string CompressBuffer(const std::string& buf)
	{
		std::string buffer = ToString(buf.size()) + " ";
		buffer.resize(HeaderSize);
		buffer += buf;
		std::string compressBuffer;
		CHECK_ASSERT(buffer.size() < std::numeric_limits<int>::max());
		compressBuffer.resize(LZ4_compressBound((int)buffer.size()));
		auto bufferSize = LZ4_compress(buffer.c_str(), &compressBuffer[0], (int)buffer.size());
		CHECK_ASSERT(bufferSize >= 0);
		compressBuffer.resize(bufferSize);
		return compressBuffer;
	}


	std::string DecompressBuffer(const std::string& buffer)
	{
		std::string smallBuffer;
		smallBuffer.resize(HeaderSize);
		CHECK_ASSERT(buffer.size() < std::numeric_limits<int>::max());
		CHECK_ASSERT(smallBuffer.size() < std::numeric_limits<int>::max());
		LZ4_decompress_safe_partial(&buffer[0], &smallBuffer[0], (int)buffer.size(), (int)smallBuffer.size(), (int)smallBuffer.size());
		std::string::size_type bytes = ToInt(smallBuffer);
		bytes += smallBuffer.size();
		std::string outputBuffer;
		outputBuffer.resize(bytes);
		CHECK_ASSERT(bytes < std::numeric_limits<int>::max());
		int totalBytes = LZ4_decompress_safe(&buffer[0], &outputBuffer[0], (int)buffer.size(), (int)bytes);
        CHECK_ASSERT(totalBytes == (int)bytes);
		outputBuffer.erase(outputBuffer.begin(), outputBuffer.begin() + smallBuffer.size());
		return outputBuffer;
	}
#if 0
	void SleepMs(unsigned milliseconds)
	{
#if EMSCRIPTEN
		emscripten_sleep(milliseconds);
#else
		std::this_thread::sleep_for(Milliseconds(milliseconds));
#endif
	}
#endif
}
