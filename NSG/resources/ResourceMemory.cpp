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
#include "ResourceMemory.h"
#include "Check.h"
#include "Path.h"
#include "b64/encode.h"
#include "b64/decode.h"
#include "pugixml.hpp"

namespace NSG
{
    ResourceMemory::ResourceMemory(const std::string& name)
		:Resource(name)
    {
		AllowInvalidate(false);
    }

    ResourceMemory::~ResourceMemory()
    {
        Invalidate();
    }

    void ResourceMemory::SetData(const char* buffer, size_t bytes)
    {
        buffer_.resize(bytes);
        memcpy(&buffer_[0], buffer, bytes);
    }

    bool ResourceMemory::IsValid()
    {
        return true;
    }

	void ResourceMemory::Load(const pugi::xml_node& node)
    {
		pugi::xml_node dataNode = node.child("data");
		size_t bytes = dataNode.attribute("dataSize").as_uint();
		std::string buffer;
		if (bytes)
		{
			const pugi::char_t* data = dataNode.child_value();
			buffer.resize(bytes);
			memcpy(&buffer[0], data, bytes);
		}

		std::string decoded_binary;
		decoded_binary.resize(bytes);
		base64::base64_decodestate state;
		base64::base64_init_decodestate(&state);
		int decoded_length = base64::base64_decode_block(buffer.c_str(), bytes, &decoded_binary[0], &state);
		decoded_binary.resize(decoded_length);
		SetData(decoded_binary.c_str(), decoded_binary.size());
    }
}