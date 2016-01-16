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
    LoaderXMLNode::LoaderXMLNode(LoaderXML* loaderXML, PObject obj, const std::string& type, const std::string& nameAttValue)
        : loaderXML_(loaderXML),
          obj_(obj),
          type_(type),
          nameAttValue_(nameAttValue)
    {
		CHECK_ASSERT(loaderXML_ && !nameAttValue_.empty() && !type_.empty());
    }

    LoaderXMLNode::~LoaderXMLNode()
    {
    }

    bool LoaderXMLNode::Load()
    {
        if (loaderXML_->IsReady() && obj_.lock())
        {
            auto& doc = loaderXML_->GetDocument();
            auto appNode = doc.child("App");
            auto resources = appNode.child(type_.c_str());
            auto node = resources.find_child([this](pugi::xml_node & node) { return node.attribute("name").as_string() == nameAttValue_; });
            CHECK_CONDITION(node);
            auto obj = obj_.lock();
            if (type_ == "Resources" && !node.child("data"))
            {
                Path path(node.attribute("name").as_string());
                auto resourceFile = std::dynamic_pointer_cast<ResourceFile>(obj);
                CHECK_ASSERT(resourceFile);
                auto xmlFile = std::dynamic_pointer_cast<ResourceFile>(loaderXML_->GetResource());
                if (xmlFile)
                    path.SetPath(xmlFile->GetPath().GetPath()); //use path of XML file
                resourceFile->SetPath(path);
            }
            else
                obj->Load(node);
            return true;
        }
        return false;
    }
}