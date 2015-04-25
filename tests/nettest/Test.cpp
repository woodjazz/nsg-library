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
#include "NSG.h"
using namespace NSG;

static int Test01()
{
    auto window = Window::Create("hiddenWindow", (int)WindowFlag::HIDDEN);

    HTTPRequest::Form form;
    form["user"] = "user2";
    form["value"] = "200";

    auto onLoad0 = [&](const std::string & data)
    {
		TRACE_PRINTF("HTTP OnLoad0: %s", data.c_str());
    };

	auto onLoad1 = [&](const std::string & data)
	{
		TRACE_PRINTF("HTTP OnLoad1: %d %s", data.size(), data.c_str());
		window = nullptr;
	};

    auto onError = [&](int httpError, const std::string & description)
    {
		TRACE_PRINTF("HTTP Error: %d. %s", httpError, description.c_str());
		CHECK_CONDITION(false, __FILE__, __LINE__);
    };

    auto onProgress = [&](unsigned percentage)
    {
		TRACE_PRINTF("HTTP Progress: %d", percentage);
    };

    HTTPRequest postRequest("http://nsg-datacollector.appspot.com/store", form, onLoad0, onError, onProgress);
	HTTPRequest getRequest("http://nsg-datacollector.appspot.com/retrieve", onLoad1, onError, onProgress);
    return Engine().Run();
}

static int Test02()
{
    auto window0 = Window::Create("hiddenWindow", (int)WindowFlag::HIDDEN);
    auto window1 = window0;

    HTTPRequest::Form form;
    auto onLoad = [&](const std::string & data)
    {
        CHECK_CONDITION(false, __FILE__, __LINE__);
    };

    auto onError0 = [&](int httpError, const std::string & description)
    {
		TRACE_PRINTF("HTTP Error: %d. %s", httpError, description.c_str());
        window0 = nullptr;
    };

    auto onError1 = [&](int httpError, const std::string & description)
    {
		TRACE_PRINTF("HTTP Error: %d. %s", httpError, description.c_str());
        window1 = nullptr;
    };

    auto onProgress = [&](unsigned percentage)
    {
		TRACE_PRINTF("HTTP Progress: %d", percentage);
    };

    HTTPRequest request0("http://nsg-datacollector.appspot11111.com/storexasas", form, onLoad, onError0, onProgress);
    HTTPRequest request1("http://nsg-datacollector.appspot.com/retrievesasa", onLoad, onError1, onProgress);
	return Engine().Run();
}

void Tests()
{
	Test01();
    Test02();
}