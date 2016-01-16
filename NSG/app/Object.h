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
#include "Types.h"
#include "LoaderXMLNode.h"
#include <string>
#include <functional>
namespace NSG
{
    class App;
    class Object
    {
    public:
        Object(const std::string& name);
        virtual ~Object();
		void DisableInvalidation() { disableInvalidation_ = true; }
		void EnableInvalidation() { disableInvalidation_ = false; }
        void Invalidate();
        bool IsReady();
        void TryReady();
        virtual void Load(const pugi::xml_node& node) {}
        static void InvalidateAll();
        const std::string& GetName() const { return name_; }
        SignalEmpty::PSignal SigAllocated() {return signalAllocated_; }
        SignalEmpty::PSignal SigReleased() {return signalReleased_; }
        void SetLoader(PLoaderXMLNode nodeLoader);
        template<typename T, typename U>
        static std::vector<std::shared_ptr<T>> LoadAll(LoaderXML* loader, const char* collectionType)
        {
            std::vector<std::shared_ptr<T>> result;
            auto adder = [&](const std::string& name)
            {
				auto obj = T:: template GetOrCreateClass<U>(name);
				obj->SetLoader(std::make_shared<LoaderXMLNode>(loader, obj, collectionType, name));
	            result.push_back(obj);
            };
            Object::LoadAll(loader, collectionType, adder);
            return result;
        }
    protected:
        std::string name_;
        PLoaderXMLNode nodeLoader_;
    private:
    	typedef std::function<void(const std::string&)> AdderFunction;
    	static void LoadAll(LoaderXML* loader, const char* collectionType, AdderFunction adder);
        static SignalEmpty::PSignal SigInvalidateAll();
        virtual bool IsValid() { return true; }
        virtual void AllocateResources() {}
        virtual void ReleaseResources()	{}
        std::string GetType() const;
        std::string GetNameType() const;
        bool isValid_;
        bool resourcesAllocated_;
        SignalEmpty::PSlot slotInvalidateAll_;
        SignalEmpty::PSignal signalAllocated_;
        SignalEmpty::PSignal signalReleased_;
		bool disableInvalidation_;
    };
}