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
#include "Resource.h"
#include "ResourceFile.h"
#include "Texture.h"
#include "Sound.h"
#include "Log.h"
#include "Check.h"
#include "Util.h"
#include "Scene.h"
#include "Path.h"
#include "Material.h"
#include "Mesh.h"
#include "Image.h"
#include "LoaderXML.h"
#include "pugixml.hpp"
#include "b64/encode.h"
#include "b64/decode.h"

namespace NSG
{
    template<> std::map<std::string, PWeakResource> WeakFactory<std::string, Resource>::objsMap_ = std::map<std::string, PWeakResource>{};

    Resource::Resource(const std::string& name)
        : Object(name),
          serializable_(true)
    {
    }

    Resource::~Resource()
    {
    }

    void Resource::Load(const pugi::xml_node& node)
    {
        std::string name = node.attribute("name").as_string();
        pugi::xml_node dataNode = node.child("data");
        if (!dataNode)
        {
			ResourceFile resFile(name);
			CHECK_CONDITION(resFile.IsReady());
            SetBuffer(resFile.GetBuffer());
        }
        else
        {
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
			CHECK_ASSERT(bytes < std::numeric_limits<int>::max());
			auto decoded_length = base64::base64_decode_block(buffer.c_str(), (int)bytes, &decoded_binary[0], &state);
			decoded_binary.resize(decoded_length);
			SetBuffer(decoded_binary);
        }
    }

    void Resource::ReleaseResources()
    {
        buffer_.clear();
    }

    void Resource::SetSerializable(bool serializable)
    {
        serializable_ = serializable;
    }

    bool Resource::IsSerializable() const
    {
        return serializable_;
    }

    void Resource::SaveExternal(pugi::xml_node& node, const Path& path, const Path& outputDir)
    {
        if (!serializable_)
            return;

        pugi::xml_node child = node.append_child("Resource");

        Path newPath;
        newPath.SetPath(path.GetPath());
        newPath.SetFileName(Path(name_).GetFilename());
        SetName(newPath.GetFilePath());

        auto texture = Material::GetTextureWithResource(shared_from_this());
        if (texture)
        {
			Image image(shared_from_this());
			CHECK_CONDITION(image.IsReady());
			if (!image.SaveAsPNG(outputDir))
            {
                LOGE("Cannot save file: %s in %s", name_.c_str(), outputDir.GetPath().c_str());
            }
            else
            {
                newPath.SetExtension("png");
            }
        }
        else
        {
            std::ofstream os(newPath.GetFullAbsoluteFilePath(), std::ios::binary);
            if (os.is_open())
                os.write(&buffer_[0], buffer_.size());
            else
				LOGE("Cannot save file: %s", newPath.GetFilePath().c_str());
        }

        {
            std::vector<std::string> dirs = Path::GetDirs(outputDir.GetPath());
            Path relativePath;
            if (!dirs.empty())
                relativePath.SetPath(dirs.back());
            relativePath.SetFileName(newPath.GetFilename());
            SetName(relativePath.GetFilePath());
        }

        child.append_attribute("name").set_value(name_.c_str());
    }

    void Resource::Save(pugi::xml_node& node)
    {
        if (!serializable_)
            return;

        CHECK_CONDITION(IsReady());

        pugi::xml_node child = node.append_child("Resource");

        base64::base64_encodestate state;
        base64::base64_init_encodestate(&state);

        std::string encoded_data;
        encoded_data.resize(2 * buffer_.size());

		CHECK_ASSERT(buffer_.size() < std::numeric_limits<int>::max());
        auto numchars = base64::base64_encode_block(&buffer_[0], (int)buffer_.size(), &encoded_data[0], &state);
        numchars += base64::base64_encode_blockend(&encoded_data[0] + numchars, &state);
        encoded_data.resize(numchars);

        pugi::xml_node dataNode = child.append_child("data");
        dataNode.append_attribute("dataSize").set_value((unsigned)encoded_data.size());
        dataNode.append_child(pugi::node_pcdata).set_value(encoded_data.c_str());

        SetName(GetUniqueName(Path(name_).GetFilename()));
        child.append_attribute("name").set_value(name_.c_str());
    }

    void Resource::SetName(const std::string& name)
    {
        if (name != name_)
        {
            name_ = name;
            Invalidate(); //name_ has changed = > force reload
        }
    }

    void Resource::SaveResources(pugi::xml_node& node)
    {
        pugi::xml_node child = node.append_child("Resources");
        auto resources = Resource::GetObjs();
        for (auto& obj : resources)
            obj->Save(child);
    }

    void Resource::SaveResourcesExternally(pugi::xml_node& node, const Path& path, const Path& outputDir)
    {
        pugi::xml_node child = node.append_child("Resources");
		auto resources = Resource::GetObjs();
        for (auto& obj : resources)
            obj->SaveExternal(child, path, outputDir);
    }

	int Resource::GetBytes() const 
	{ 
		CHECK_ASSERT(buffer_.size() < std::numeric_limits<int>::max());
		return (int)buffer_.size(); 
	}
}