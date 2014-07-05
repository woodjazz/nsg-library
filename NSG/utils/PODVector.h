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
#include <cassert>
#include <vector>

namespace NSG
{
	template<typename T, size_t N>
	class PODVector : public std::vector<T>
	{
	public:
		PODVector()
		{
			reserve(N);
		}

		~PODVector()
		{
		}

		typedef typename std::vector<T>::value_type value_type;
		typedef typename std::vector<T>::size_type size_type;

		void push_back(const value_type& value)
		{
			assert(std::vector<T>::size() < N);
			std::vector<T>::push_back(value);
		}
	private:
		void reserve(size_type count)
		{
			std::vector<T>::reserve(count);
		}
	};
}