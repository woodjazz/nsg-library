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
#include "Object.h"
#include "Check.h"
#include "Util.h"
#include "SignalSlots.h"

namespace NSG
{
    Object::Object(const std::string& name)
        : name_(name),
          isValid_(false),
          resourcesAllocated_(false),
          signalAllocated_(new SignalEmpty),
          signalReleased_(new SignalEmpty)
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
        isValid_ = false;
        if (resourcesAllocated_)
        {
            ReleaseResources();
            resourcesAllocated_ = false;
            TRACE_PRINTF("--->Released resources for %s\n", GetNameType().c_str());
            signalReleased_->Run();
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
        if (!isValid_)
        {
            isValid_ = IsValid();

            if (isValid_)
            {
                CHECK_ASSERT(!resourcesAllocated_, __FILE__, __LINE__);
                AllocateResources();
                resourcesAllocated_ = true;
                TRACE_PRINTF("--->Allocated resources for %s\n", GetNameType().c_str());
                signalAllocated_->Run();
            }
        }
        return isValid_;
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
}