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
#include <memory>

namespace NSG
{
	template <std::size_t N>
	class Arena
	{
	public:
		Arena() : ptr_(buffer_) {}
		~Arena() 
		{ 
			ptr_ = nullptr; 
		}

	    Arena(const Arena&) = delete;
	    Arena& operator=(const Arena&) = delete;

	    char* Allocate(std::size_t n)
		{
		    assert(pointer_in_buffer(ptr_) && "pointer is not in buffer");

			if ((size_t)(buffer_ + N - ptr_) >= n)
		    {
		        char* r = ptr_;
		        ptr_ += n;
		        return r;
		    }
		    return static_cast<char*>(::operator new(n));
		}

	    void Deallocate(char* p, std::size_t n)
		{
		    assert(pointer_in_buffer(ptr_) && "pointer is not in buffer");


		    if(pointer_in_buffer(p))
		    {
		        if (p + n == ptr_)
		            ptr_ = p;
		    }
		    else
		        ::operator delete(p);
		}

	    static std::size_t Size() 
	    {
	    	return N;
	    }

	    std::size_t Used() const 
	    {
			return static_cast<std::size_t>(ptr_ - buffer_);
	    }

	    void Reset() 
	    {
			ptr_ = buffer_;
	    }

	private:
		bool pointer_in_buffer(char* p) 
		{
			return buffer_ <= p && p <= buffer_ + N;
		}

#if _MSC_VER
		char buffer_[N];
#else	
		alignas(16) char buffer_[N];
#endif	
	    char* ptr_;
	};
}