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
#include "types.h"
#include <memory>
#include <vector>
#include <map>

namespace NSG
{
    template<typename K, typename T>
    class MapAndVector
    {
    	typedef std::shared_ptr<T> PT;
    public:
        MapAndVector() 
        {

        }

		PT Get(const K& key) const
		{
			auto it = objsMap_.find(key);
			if (it != objsMap_.end())
				return it->second;
			return nullptr;
		}

        void Add(const K& key, PT obj)
        {
            CHECK_ASSERT(!key.empty(), __FILE__, __LINE__);
            CHECK_ASSERT(obj, __FILE__, __LINE__);
            objs_.push_back(obj);
            objsMap_[key] = obj;
        }

        PT Create(const K& key)
        {
            CHECK_ASSERT(!key.empty(), __FILE__, __LINE__);
            auto it = objsMap_.find(key);
            CHECK_ASSERT(it == objsMap_.end(), __FILE__, __LINE__);
            PT obj(new T(key));
            objs_.push_back(obj);
            objsMap_[key] = obj;
            return obj;
        }

        PT GetOrCreate(const K& key)
        {
            CHECK_ASSERT(!key.empty(), __FILE__, __LINE__);
            auto it = objsMap_.find(key);
            if (it == objsMap_.end())
                return Create(key);
            else
                return it->second;
        }

        const std::vector<PT>& GetConstObjs() const
        {
        	return objs_;
        }

        std::vector<PT>& GetObjs()
        {
            return objs_;
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
        std::vector<PT> objs_;
        std::map<K, PT> objsMap_;
    };
}