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
#include "NSG.h"
using namespace NSG;

static void Test01()
{
	{
		Path path("/sjhdjshd/sdhsdjhsd/kskd.txt");
		TRACE_LOG(path);
		CHECK_ASSERT(!path.IsPathRelative(), __FILE__, __LINE__);
	}
#if WIN32
	{
		Path path("c:/sjhdjshd/sdhsdjhsd/kskd.txt");
		CHECK_ASSERT(!path.IsPathRelative(), __FILE__, __LINE__);
	}

	{
		Path path("a:\\sjhdjshd/sdhsdjhsd/kskd.txt");
		CHECK_ASSERT(!path.IsPathRelative(), __FILE__, __LINE__);
	}
#endif
	{
		Path path("a:sjhdjshd/sdhsdjhsd/kskd.txt");
		CHECK_ASSERT(path.IsPathRelative(), __FILE__, __LINE__);
	}

	{
		Path path("sjhdjshd/sdhsdjhsd/kskd.txt");
		TRACE_LOG(path);
		CHECK_ASSERT(path.IsPathRelative(), __FILE__, __LINE__);
	}

	{
		Path path("sjhdjshd/sdhsdjhsd/kskd.TXT");
		CHECK_ASSERT(path.GetFilename() == "kskd.TXT", __FILE__, __LINE__);
		CHECK_ASSERT(path.GetPath() == "sjhdjshd/sdhsdjhsd", __FILE__, __LINE__);
		CHECK_ASSERT(path.GetExtension() == "txt", __FILE__, __LINE__);
	}
}

void Tests()
{
	Test01();
}