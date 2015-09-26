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
#pragma once
#include "Types.h"
#include "Util.h"
#include "Check.h"
#include "Log.h"
#include <memory>
#include <map>

namespace NSG
{
    template<typename K, typename T>
    class WeakFactory
    {
		typedef std::shared_ptr<T> PT;
		typedef std::weak_ptr<T> PWT;
    public:
        WeakFactory() 
        {
        }

		template<typename U>
		static std::shared_ptr<U> CreateClass(const K& key = GetUniqueName(typeid(U).name()))
		{
			CHECK_CONDITION(!Has(key));
			auto obj = std::make_shared<U>(key);
			auto result = objsMap_.insert(typename std::map<K, PWT>::value_type(key, obj));
			if (!result.second)
			{
				LOGE("WeakFactory failed creating %s!!!", key.c_str());
				return nullptr;
			}
			return obj;
		}

		template<typename U>
		static std::shared_ptr<U> GetOrCreateClass(const K& key)
		{
			auto it = objsMap_.find(key);
			if (it == objsMap_.end())
                return CreateClass<U>(key);
            else if(!it->second.lock())
            {
                objsMap_.erase(it);
				return CreateClass<U>(key);
            }
			else
				return std::dynamic_pointer_cast<U>(it->second.lock());
		}

		template<typename U>
		static std::shared_ptr<U> GetClass(const K& key)
		{
			auto it = objsMap_.find(key);
			if (it != objsMap_.end() && it->second.lock())
				return std::dynamic_pointer_cast<U>(it->second.lock());
			return nullptr;
		}

		static PT Get(const K& key)
		{
			auto it = objsMap_.find(key);
			if (it != objsMap_.end() && it->second.lock())
				return it->second.lock();
			return nullptr;
		}

		template<typename U>
		static std::shared_ptr<U> Get(const K& key)
		{
			return GetClass<U>(key);
		}

		static PT Create(const K& key = GetUniqueName(typeid(T).name()))
        {
			return CreateClass<T>(key);
        }

		template<typename U>
		static std::shared_ptr<U> Create(const K& key = GetUniqueName(typeid(U).name()))
		{
			return CreateClass<U>(key);
		}

		static PT GetOrCreate(const K& key)
        {
			return GetOrCreateClass<T>(key);
        }

		template<typename U>
		static std::shared_ptr<U> GetOrCreate(const K& key)
		{
			return GetOrCreateClass<U>(key);
		}

		static std::vector<PT> GetObjs()
        {
			std::vector<PT> objs;
			for (auto& obj : objsMap_)
			{
				auto p = obj.second.lock();
				if (p) objs.push_back(p);
			}
			return objs;
        }

		static bool Has(const K& key)
		{
			return objsMap_.find(key) != objsMap_.end();
		}

        static void Clear()
        {
        	objsMap_.clear();
        }

        static bool AreReady()
       	{
			for (auto& obj : objsMap_)
			{
				auto p = obj.second.lock();
				if(p && !p->IsReady())
					return false;
			}
			return true;
       	}
       	
    private:
		static std::map<K, PWT> objsMap_;
    };
}