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

#include "UTF8String.h"
#include "Check.h"

namespace NSG
{
	UTF8String::UTF8String(const char* str)
        : std::string(str)
	{

	}

	UTF8String::~UTF8String()
	{
		
	}

    //
    // Copyright (c) 2008-2014 the Urho3D project.
    //
    // Permission is hereby granted, free of charge, to any person obtaining a copy
    // of this software and associated documentation files (the "Software"), to deal
    // in the Software without restriction, including without limitation the rights
    // to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    // copies of the Software, and to permit persons to whom the Software is
    // furnished to do so, subject to the following conditions:
    //
    // The above copyright notice and this permission notice shall be included in
    // all copies or substantial portions of the Software.
    //
    // THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    // IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    // FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    // AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    // LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    // OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    #define GET_NEXT_CONTINUATION_BYTE(ptr) *ptr; if ((unsigned char)*ptr < 0x80 || (unsigned char)*ptr >= 0xc0) return '?'; else ++ptr;
    unsigned UTF8String::DecodeUTF8(const char*& src) const
    {
        if (src 
            == 0)
            return 0;
        
        unsigned char char1 = *src++;
        
        // Check if we are in the middle of a UTF8 character
        if (char1 >= 0x80 && char1 < 0xc0)
        {
            while ((unsigned char)*src >= 0x80 && (unsigned char)*src < 0xc0)
                ++src;
            return '?';
        }
        
        if (char1 < 0x80)
            return char1;
        else if (char1 < 0xe0)
        {
            unsigned char char2 = GET_NEXT_CONTINUATION_BYTE(src);
            return (char2 & 0x3f) | ((char1 & 0x1f) << 6);
        }
        else if (char1 < 0xf0)
        {
            unsigned char char2 = GET_NEXT_CONTINUATION_BYTE(src);
            unsigned char char3 = GET_NEXT_CONTINUATION_BYTE(src);
            return (char3 & 0x3f) | ((char2 & 0x3f) << 6) | ((char1 & 0xf) << 12);
        }
        else if (char1 < 0xf8)
        {
            unsigned char char2 = GET_NEXT_CONTINUATION_BYTE(src);
            unsigned char char3 = GET_NEXT_CONTINUATION_BYTE(src);
            unsigned char char4 = GET_NEXT_CONTINUATION_BYTE(src);
            return (char4 & 0x3f) | ((char3 & 0x3f) << 6) | ((char2 & 0x3f) << 12) | ((char1 & 0x7) << 18);
        }
        else if (char1 < 0xfc)
        {
            unsigned char char2 = GET_NEXT_CONTINUATION_BYTE(src);
            unsigned char char3 = GET_NEXT_CONTINUATION_BYTE(src);
            unsigned char char4 = GET_NEXT_CONTINUATION_BYTE(src);
            unsigned char char5 = GET_NEXT_CONTINUATION_BYTE(src);
            return (char5 & 0x3f) | ((char4 & 0x3f) << 6) | ((char3 & 0x3f) << 12) | ((char2 & 0x3f) << 18) | ((char1 & 0x3) << 24);
        }
        else
        {
            unsigned char char2 = GET_NEXT_CONTINUATION_BYTE(src);
            unsigned char char3 = GET_NEXT_CONTINUATION_BYTE(src);
            unsigned char char4 = GET_NEXT_CONTINUATION_BYTE(src);
            unsigned char char5 = GET_NEXT_CONTINUATION_BYTE(src);
            unsigned char char6 = GET_NEXT_CONTINUATION_BYTE(src);
            return (char6 & 0x3f) | ((char5 & 0x3f) << 6) | ((char4 & 0x3f) << 12) | ((char3 & 0x3f) << 18) | ((char2 & 0x3f) << 24) |
                ((char1 & 0x1) << 30);
        }
    }

    //
    // Copyright (c) 2008-2014 the Urho3D project.
    //
    // Permission is hereby granted, free of charge, to any person obtaining a copy
    // of this software and associated documentation files (the "Software"), to deal
    // in the Software without restriction, including without limitation the rights
    // to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    // copies of the Software, and to permit persons to whom the Software is
    // furnished to do so, subject to the following conditions:
    //
    // The above copyright notice and this permission notice shall be included in
    // all copies or substantial portions of the Software.
    //
    // THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    // IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    // FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    // AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    // LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    // OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    unsigned UTF8String::ByteOffsetUTF8(unsigned index) const
    {
        unsigned byteOffset = 0;
        unsigned utfPos = 0;
        
        while (utfPos < index && byteOffset < length())
        {
            NextUTF8Char(byteOffset);
            ++utfPos;
        }
        
        return byteOffset;
    }

    //
    // Copyright (c) 2008-2014 the Urho3D project.
    //
    // Permission is hereby granted, free of charge, to any person obtaining a copy
    // of this software and associated documentation files (the "Software"), to deal
    // in the Software without restriction, including without limitation the rights
    // to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    // copies of the Software, and to permit persons to whom the Software is
    // furnished to do so, subject to the following conditions:
    //
    // The above copyright notice and this permission notice shall be included in
    // all copies or substantial portions of the Software.
    //
    // THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    // IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    // FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    // AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    // LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    // OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    unsigned UTF8String::NextUTF8Char(unsigned& byteOffset) const
    {
        const char* buffer = c_str();
        if (!buffer)
            return 0;
        
        const char* src = buffer + byteOffset;
        unsigned ret = DecodeUTF8(src);
		auto bytesDiff = src - buffer;
		CHECK_ASSERT(bytesDiff < std::numeric_limits<unsigned>::max());
        byteOffset = (unsigned)(src - buffer);
        
        return ret;
    }

    //
    // Copyright (c) 2008-2014 the Urho3D project.
    //
    // Permission is hereby granted, free of charge, to any person obtaining a copy
    // of this software and associated documentation files (the "Software"), to deal
    // in the Software without restriction, including without limitation the rights
    // to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    // copies of the Software, and to permit persons to whom the Software is
    // furnished to do so, subject to the following conditions:
    //
    // The above copyright notice and this permission notice shall be included in
    // all copies or substantial portions of the Software.
    //
    // THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    // IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    // FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    // AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    // LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    // OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    unsigned UTF8String::AtUTF8(unsigned index) const
    {
        unsigned byteOffset = ByteOffsetUTF8(index);
        return NextUTF8Char(byteOffset);
    }
	
}
