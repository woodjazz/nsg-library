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
#include "Check.h"
#include "Arena.h"
#include <cassert>

namespace NSG
{
	template< class T, size_t N>
	class Allocator : public std::allocator<T> 
	{
	public:
		//template<class U> struct rebind { typedef Allocator<U, N> other; };
		//template<class U> Allocator(const Allocator<U, N>& other) : pool_(other.pool_) {}

		Allocator() {}
		Allocator(const Allocator& obj) = default;
		Allocator& operator = (const Allocator& obj) { pool_ = obj.pool_; }

		typedef typename std::allocator<T>::pointer pointer;
		typedef typename std::allocator<T>::size_type size_type;

		pointer allocate(size_type n)
		{
			return reinterpret_cast<pointer>(pool_.Allocate(sizeof(T) * n));
		}
		void deallocate(pointer p, size_type n)
		{
			pool_.Deallocate(reinterpret_cast<char*>(p), n);
		}

		template <class T1, std::size_t N1, class U, std::size_t M>
		friend bool operator==(const Allocator<T1, N1>& x, const Allocator<U, M>& y);
		template <class U, std::size_t M> friend class Allocator;

	private:
		Arena<N> pool_;
	};

	template <class T, std::size_t N, class U, std::size_t M>
	inline bool operator == (const Allocator<T, N>& x, const Allocator<U, M>& y)
	{
	    return N == M && &x.pool_ == &y.pool_;
	}

	template <class T, std::size_t N, class U, std::size_t M>
	inline bool operator != (const Allocator<T, N>& x, const Allocator<U, M>& y)
	{
	    return !(x == y);
	}	
}
