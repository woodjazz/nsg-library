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
#if 0
#include "Types.h"
#include "Path.h"
#include <string>
using namespace NSG;

bool ReadDNA(const char* fileName, std::string& buffer)
{
	FILE* fp = fopen(fileName, "rb");

    if (!fp)
    {
		LOGE("File loading failed %s", fileName);
        return false;
    }

    fseek(fp, 0L, SEEK_END);
    int len = ftell(fp);
    fseek(fp, 0L, SEEK_SET);
    std::string buf;
    buf.resize(len + 1);
    fread(&buf[0], len, 1, fp);

    fclose(fp);

    bool blender = !strncmp("BLENDER", &buf[0], 6);
    if (!blender)
    {
        LOGE("Invalid header, compressed file?");
        return false;
    }

    const char* ver = &buf[9];
    int version = atoi(ver);
	if (version < 274)
	{
		LOGE("Only versions starting from 274 are supported");
		return false;
	}

	LOGI("Reading DNA version %d", version);
    bool is64Bit = false;
    if (buf[7] == '-')
        is64Bit = true;

    int dnastart = -1;
    const char* tbuf = buf.data();
    for (int i = 0; i < len; ++i)
    {
        if (!strncmp("SDNA", tbuf, 4))
        {
            dnastart = i;
            break;
        }
        ++tbuf;
    }

    if (dnastart != -1)
    {
        const char* dnaStr = &buf[dnastart];
		auto size = len - dnastart;
		buffer.resize(size);
		memcpy(&buffer[0], dnaStr, size);
    }

    return true;
}
#endif