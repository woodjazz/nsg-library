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
#include "NSG.h"
using namespace NSG;
#include <fstream>

static void Test01()
{
	{
		Path path("/sjhdjshd/sdhsdjhsd/kskd.txt");
		CHECK_CONDITION(!path.IsPathRelative());
	}
#if WIN32
	{
		Path path("c:/sjhdjshd/sdhsdjhsd/kskd.txt");
		CHECK_CONDITION(!path.IsPathRelative());
	}

	{
		Path path("a:\\sjhdjshd/sdhsdjhsd/kskd.txt");
		CHECK_CONDITION(!path.IsPathRelative());
	}
#endif
	{
		Path path("a:sjhdjshd/sdhsdjhsd/kskd.txt");
		CHECK_CONDITION(path.IsPathRelative());
	}

	{
		Path path("sjhdjshd/sdhsdjhsd/kskd.txt");
		CHECK_CONDITION(path.IsPathRelative());
	}

	{
		Path path("sjhdjshd/sdhsdjhsd/kskd.TXT");
		CHECK_CONDITION(path.GetFilename() == "kskd.TXT");
		CHECK_CONDITION(path.GetPath() == "sjhdjshd/sdhsdjhsd");
		CHECK_CONDITION(path.GetExtension() == "txt");
	}

	{
		Path path("sjhdjshd/data/kskd.TXT");
		CHECK_CONDITION(!path.AppendDirIfDoesNotExist("data"));
		CHECK_CONDITION(path.GetPath() == "sjhdjshd/data");
		CHECK_CONDITION(path.AppendDirIfDoesNotExist("Data"));
		CHECK_CONDITION(path.GetPath() == "sjhdjshd/data/Data");
	}

	{
		Path path("/sjhdjshd/sdhsdjhsd/kskd.txt");
		CHECK_CONDITION(path.GetExtension() == "txt");
		CHECK_CONDITION(path.GetFilename() == "kskd.txt");
		path.AddExtension("zip");
		CHECK_CONDITION(path.GetExtension() == "zip");
		CHECK_CONDITION(path.GetFilename() == "kskd.txt.zip");
	}

	{
		Path path("data/testfile.txt");
		auto mtime = path.GetModificationTime();
		CHECK_CONDITION(!mtime.empty());
		LOGI("%s", mtime.c_str());
	}

}

void Tests()
{
	Test01();
}