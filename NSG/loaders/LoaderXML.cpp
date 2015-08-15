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
#include "LoaderXML.h"
#include "Check.h"
#include "Resource.h"

namespace NSG
{
	template<> std::map<std::string, PLoaderXML> StrongFactory<std::string, LoaderXML>::objsMap_ = std::map<std::string, PLoaderXML>{};

    LoaderXML::LoaderXML(const std::string& name)
    : Object(name)
    {
    }

    LoaderXML::~LoaderXML()
    {
    }

    void LoaderXML::Set(PResource resource)
    {
        if (resource != resource_)
        {
            resource_ = resource;
            Invalidate();
        }
    }

    bool LoaderXML::IsValid()
    {
        return resource_ && resource_->IsReady();
    }

    void LoaderXML::AllocateResources()
    {
        pugi::xml_parse_result result = doc_.load_buffer((void*)resource_->GetData(), resource_->GetBytes());
        if (!result)
        {
			LOGE("Cannot load XML %s. Error description: %s", resource_->GetName().c_str(), result.description());
			LOGE("Error offset: %d", result.offset);
        }
    }

    void LoaderXML::ReleaseResources()
    {
        doc_.reset();
    }

	pugi::xml_node LoaderXML::GetNode(const std::string& type, const std::string& name) const
	{
		auto appNode = doc_.child("App");
		auto collection = appNode.child(type.c_str());
		auto node = collection.find_child([&](pugi::xml_node & node) { return name == node.attribute("name").as_string(); });
		return node;
	}
}