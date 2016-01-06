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
#include "LoaderXMLNode.h"
#include "LoaderXML.h"
#include "Check.h"
#include "ResourceFile.h"
#include "b64/encode.h"
#include "b64/decode.h"

namespace NSG
{
    LoaderXMLNode::LoaderXMLNode(const std::string& name)
        : Object(name)
	{
    }

    LoaderXMLNode::~LoaderXMLNode()
    {
    }

	void LoaderXMLNode::Set(LoaderXML* loaderXML, PObject obj, const std::string& type, const std::string& nameAttValue)
    {
		if (loaderXML_ != loaderXML || type_ != type || nameAttValue != nameAttValue_ || obj_.lock() != obj)
        {
			loaderXML_ = loaderXML;
            type_ = type;
            nameAttValue_ = nameAttValue;
            obj_ = obj;
            Invalidate();
        }
    }

    bool LoaderXMLNode::IsValid()
    {
		if (loaderXML_ && loaderXML_->IsReady() && obj_.lock() && !nameAttValue_.empty() && !type_.empty())
		{
			if (node_.empty())
			{
				auto& doc = loaderXML_->GetDocument();
				auto appNode = doc.child("App");
				auto resources = appNode.child(type_.c_str());
				node_ = resources.find_child([this](pugi::xml_node & node) { return node.attribute("name").as_string() == nameAttValue_; });
			}
			
			if (!node_)
			{
				LOGE("Node with name %s not found", nameAttValue_.c_str());
			}
			else 
			{
				auto obj = obj_.lock();
				std::string name = node_.attribute("name").as_string();
				pugi::xml_node dataNode = node_.child("data");
				if (!dataNode && std::dynamic_pointer_cast<Resource>(obj))
				{
					if (!resource_)
					{
						auto resourceFile = std::dynamic_pointer_cast<ResourceFile>(loaderXML_->GetResource());
						if (resourceFile)
						{
							Path path(resourceFile->GetPath().GetFilePath()); //use path of XML file
							path.SetFileName(Path(name).GetFilename());
							resource_ = std::make_shared<ResourceFile>(path.GetFilePath());
						}
						else
						{
							resource_ = std::make_shared<ResourceFile>(name);
						}
					}
					return resource_->IsReady();
				}
				return true;
			}
		}
		return false;
    }

    void LoaderXMLNode::AllocateResources()
    {
		auto obj = obj_.lock();
		if (resource_)
		{
			auto resourceFile = std::dynamic_pointer_cast<ResourceFile>(obj);
			if(resourceFile)
				resourceFile->SetPath(resource_->GetPath());
			else
				std::dynamic_pointer_cast<Resource>(obj)->SetBuffer(resource_->GetBuffer());
		}
		else
            obj->Load(node_);
    }

	void LoaderXMLNode::ReleaseResources()
	{
		node_ = pugi::xml_node();
		resource_ = nullptr;
	}
}