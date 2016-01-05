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
#include "Object.h"
#include "Check.h"
#include "Log.h"
#include "Util.h"
#include "SignalSlots.h"
#include "LoaderXML.h"
#include "LoaderXMLNode.h"

namespace NSG
{
	Object::Object(const std::string& name)
		: name_(name),
		isValid_(false),
		resourcesAllocated_(false),
		signalAllocated_(new SignalEmpty),
		signalReleased_(new SignalEmpty),
		disableInvalidation_(false)
    {
        if (name_.empty())
            name_ = GetUniqueName("Object");

        slotInvalidateAll_ = Object::SigInvalidateAll()->Connect([this]()
        {
            Invalidate();
        });
    }

    Object::~Object()
    {
    }

    void Object::Invalidate()
    {
		if (!disableInvalidation_)
		{
			isValid_ = false;
			if (resourcesAllocated_)
			{
				ReleaseResources();
				resourcesAllocated_ = false;
				LOGI("Released resources for %s.", GetNameType().c_str());
				signalReleased_->Run();
			}
		}
    }

    void Object::SetLoader(PLoaderXMLNode nodeLoader)
    {
        if (nodeLoader_ != nodeLoader)
        {
            nodeLoader_ = nodeLoader;
            Invalidate();
        }
    }

    std::string Object::GetType() const
    {
        return typeid(*this).name();
    }

    std::string Object::GetNameType() const
    {
        return name_ + "->" + GetType();
    }

    bool Object::IsReady()
    {
        TryReady();
        return isValid_;
    }

    void Object::TryReady()
    {
        if (!isValid_)
        {
            isValid_ = (!nodeLoader_ || nodeLoader_->IsReady()) && IsValid();

            if (isValid_)
            {
                CHECK_ASSERT(!resourcesAllocated_);
                LOGI("Begin: Allocating resources for %s", GetNameType().c_str());
                AllocateResources();
                LOGI("End: Allocating resources for %s", GetNameType().c_str());
                resourcesAllocated_ = true;
                signalAllocated_->Run();
            }
        }
    }

    SignalEmpty::PSignal Object::SigInvalidateAll()
    {
        static SignalEmpty::PSignal signalInvalidateAll(new SignalEmpty);
        return signalInvalidateAll;
    }

    void Object::InvalidateAll()
    {
        SigInvalidateAll()->Run();
    }

    void Object::LoadAll(LoaderXML* loader, const char* collectionType, AdderFunction adder)
    {
        auto& doc = loader->GetDocument();
        pugi::xml_node node = doc.child("App");
        pugi::xml_node collection = node.child(collectionType);
        if (collection)
        {
            pugi::xml_node child = collection.first_child();
            while (child)
            {
                std::string name = child.attribute("name").as_string();
                adder(name);
                child = child.next_sibling();
            }
        }
    }

    void Object::SetLoader(LoaderXML* loader, const char* collectionType, PObject obj, const std::string& name)
    {
        auto nodeLoader = std::make_shared<LoaderXMLNode>(name);
        nodeLoader->Set(loader, obj, collectionType, name);
        obj->SetLoader(nodeLoader);

    }

}