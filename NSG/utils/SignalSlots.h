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
#pragma once
#include <functional>
#include <algorithm>
#include <memory>
#include <vector>

namespace NSG
{
struct Alive
{
    bool ok;
    Alive() : ok(true) {}
    ~Alive()
    {
        ok = false;
    }
};
template <typename... PARAMS>
class Signal : public std::enable_shared_from_this<Signal<PARAMS...>>
{
public:
    typedef std::function<void(PARAMS...)> CallbackFunction;
    typedef std::weak_ptr<Signal<PARAMS...>> PWeakSignal;
    typedef std::shared_ptr<Signal<PARAMS...>> PSignal;
    class Slot;
    typedef std::shared_ptr<Slot> PSlot;
    typedef std::weak_ptr<Slot> PWeakSlot;

    static Alive alive_;

    Signal()
        : running_(false)
    {
    }

    ~Signal()
    {
    }

    bool HasSlots() const
    {
        return !slots_.empty() || !runSlots_.empty();
    }

    PSlot Connect(CallbackFunction callback)
    {
        auto slot = std::make_shared<Slot>(Signal<PARAMS...>::shared_from_this(), callback);
        if (!running_)
            slots_.push_back(slot);
        else
            runSlots_.push_back(slot);
        return slot;
    }

    void Run(PARAMS... arguments)
    {
        if(alive_.ok)
        {
            running_ = true;
            ExecuteRunSlots(arguments...);
            for (auto& slot : slots_)
            {
                PSlot obj(slot.lock());
                if (obj)
                    obj->Execute(arguments...);
            }
            FreeSlots(); // release memory removing destroyed slots
            running_ = false;
        }
    }

    bool FreeSlots()
    {
        auto condition = [&](PWeakSlot slot)
        {
            return !slot.lock();
        };
        auto it = std::remove_if(slots_.begin(), slots_.end(), condition);
        if (it != slots_.end())
        {
            slots_.erase(it, slots_.end());
            return true;
        }
        return false;
    }

private:

    void ExecuteRunSlots(PARAMS... arguments)
    {
        slots_.insert(slots_.end(), runSlots_.begin(), runSlots_.end());
        //while (!runSlots_.empty())
        {
            auto tmp = runSlots_;
            runSlots_.clear();
            for (auto& slot : tmp)
            {
                PSlot obj(slot.lock());
                if (obj)
                    obj->Execute(arguments...);
            }
            //ExecuteRunSlots(arguments...);
        }
    }

    bool running_;
    std::vector<PWeakSlot> slots_;
    std::vector<PWeakSlot> runSlots_; // slots connected while running
public:
    class Slot
    {
    public:
        Slot(PSignal signal, CallbackFunction callback) :
            signal_(signal),
            callback_(callback),
            enable_(true)
        {
        }

        void Enable(bool enable)
        {
            enable_ = enable;
        }

        void Execute(PARAMS... arguments)
        {
            if (enable_)
                callback_(arguments...);
        }
    private:
        PWeakSignal signal_;
        CallbackFunction callback_;
        bool enable_;
    };
};
template <typename... PARAMS>
Alive Signal<PARAMS...>::alive_;
}
