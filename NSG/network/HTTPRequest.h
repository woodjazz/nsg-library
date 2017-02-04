/*
-------------------------------------------------------------------------------
This file is part of nsg-library.
http://github.com/woodjazz/nsg-library

Copyright (c) 2014-2017 Néstor Silveira Gorski

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
#include "Types.h"
#include <string>
#include <map>
#include <future>
#include <atomic>

namespace NSG
{
    class HTTPRequest
    {
    public:
        typedef std::map<std::string, std::string> Form;
        typedef std::function<void(const std::string& data)> OnLoadFunction;
        typedef std::function<void(int httpError, const std::string& description)> OnErrorFunction;
        typedef std::function<void(unsigned percentage)> OnProgressFunction;
        HTTPRequest(const std::string& url, const Form& form, OnLoadFunction onLoad, OnErrorFunction onError, OnProgressFunction onProgress); // Post
        HTTPRequest(const std::string& url, OnLoadFunction onLoad, OnErrorFunction onError, OnProgressFunction onProgress); // Get
        ~HTTPRequest();
        void StartRequest();
        static void ParseURL(const std::string& url, std::string& protocol, std::string& host, int& port, std::string& path);
    private:
        #if EMSCRIPTEN
        static void OnLoad(unsigned int id, void* arg, void* buffer, unsigned bytes);
        static void OnError(unsigned int id, void* arg, int httpError, const char* statusDescription);
        static void OnProgress(unsigned int id, void* arg, int bytesLoaded, int totalSize);
        #endif
        std::string url_;
        std::string protocol_;
        std::string host_;
        int port_;
        std::string path_;
        #if EMSCRIPTEN
        int requestHandle_;
        #else
        int httpError_;
        std::atomic<bool> httpHasResult_;
        std::string response_;
        std::future<void> result_;
        SignalEmpty::PSlot slotBeginFrame_;
        #endif
        OnLoadFunction onLoad_;
        OnErrorFunction onError_;
        OnProgressFunction onProgress_;
        Form form_;
        bool isPost_;
    };
}
