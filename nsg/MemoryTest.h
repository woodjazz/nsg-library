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

#if defined(WIN32) && defined(_MSC_VER)
//http://msdn.microsoft.com/en-us/library/x98tx3cf.aspx
// {,,msvcr100d.dll}_crtBreakAlloc
#  include <stdio.h>
# define _CRTDBG_MAP_ALLOC
#  include <stdlib.h>
#  include <crtdbg.h>
#  include <windows.h>

namespace NSG {
    inline int CRTReportHook(int type, char* msg, int* ret) {
        printf("%s", msg);
        return 0;
    }
    struct MemoryTest {
        MemoryTest() {
            _CrtSetDbgFlag (_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
            _CrtSetReportHook(CRTReportHook);
        }
    } objMemoryReportTest;
}
#endif


