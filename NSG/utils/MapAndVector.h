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
#pragma once
#include "Types.h"
#include <memory>
#include <vector>
#include <unordered_map>

namespace NSG
{
    template<typename K, typename T>
    class MapAndVector
    {
		typedef std::shared_ptr<T> PT;
		typedef std::weak_ptr<T> PWT;
    public:
        MapAndVector() 
        {
        }

		template<typename U>
		std::shared_ptr<U> CreateClass(const K& key)
		{
			std::shared_ptr<U> obj = std::make_shared<U>(key);
			objs_.push_back(obj);
			objsMap_[key] = obj;
			return obj;
		}

		template<typename U>
		std::shared_ptr<U> GetOrCreateClass(const K& key)
		{
			auto it = objsMap_.find(key);
			if (it == objsMap_.end() || !it->second.lock())
				return CreateClass<U>(key);
			else
				return std::dynamic_pointer_cast<U>(it->second.lock());
		}

		template<typename U>
		std::shared_ptr<U> GetClass(const K& key) const
		{
			auto it = objsMap_.find(key);
			if (it != objsMap_.end() && it->second.lock())
				return std::dynamic_pointer_cast<U>(it->second.lock());
			return nullptr;
		}

		PT Get(const K& key) const
		{
			auto it = objsMap_.find(key);
			if (it != objsMap_.end() && it->second.lock())
				return it->second.lock();
			return nullptr;
		}

        void Add(const K& key, PT obj)
        {
            CHECK_ASSERT(obj, __FILE__, __LINE__);
            objs_.push_back(obj);
            objsMap_[key] = obj;
        }

        PT Create(const K& key)
        {
			return CreateClass<T>(key);
        }

        PT GetOrCreate(const K& key)
        {
			return GetOrCreateClass<T>(key);
        }

        std::vector<PT> GetObjs() const
        {
			std::vector<PT> objs;
			for (auto& obj : objs_)
			{
				auto p = obj.lock();
				if (p) objs.push_back(p);
			}
			return objs;
        }

		bool Has(const K& key) const
		{
			return objsMap_.find(key) != objsMap_.end();
		}

        void Clear()
        {
        	objs_.clear();
        	objsMap_.clear();
        }
    private:
        std::vector<PWT> objs_;
		std::unordered_map<K, PWT> objsMap_;
    };
}