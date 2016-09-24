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
#include "HTTPRequest.h"
#include "Util.h"
#include "Log.h"
#include "StringConverter.h"

#if !defined(EMSCRIPTEN)
#include "civetweb.h"
#else
#include "emscripten.h"
#endif

#if defined(IS_TARGET_WINDOWS)
#include <winsock.h>
#endif
#include <cstring>

namespace NSG
{
    struct InitializeComms
    {
        InitializeComms()
        {
            #if defined(IS_TARGET_WINDOWS)
            WSADATA data;
            WSAStartup(MAKEWORD(2, 2), &data);
            #endif
        }

        ~InitializeComms()
        {
            #if defined(IS_TARGET_WINDOWS)
            WSACleanup();
            #endif
        }
    };

    static InitializeComms inicomms;

    struct HTTPRequestData
    {
        HTTPRequest* obj_;
        std::string postData_;
    };

    HTTPRequest::HTTPRequest(const std::string& url, const Form& form, OnLoadFunction onLoad, OnErrorFunction onError, OnProgressFunction onProgress)
        : url_(url),
          #if EMSCRIPTEN
          requestHandle_(-1),
          #endif
          onLoad_(onLoad),
          onError_(onError),
          onProgress_(onProgress),
          form_(form),
          isPost_(true)
    {
		HTTPRequest::ParseURL(url, protocol_, host_, port_, path_);
    }

    HTTPRequest::HTTPRequest(const std::string& url, OnLoadFunction onLoad, OnErrorFunction onError, OnProgressFunction onProgress)
        : url_(url),
          #if EMSCRIPTEN
          requestHandle_(-1),
          #endif
          onLoad_(onLoad),
          onError_(onError),
          onProgress_(onProgress),
          isPost_(false)
    {
        HTTPRequest::ParseURL(url, protocol_, host_, port_, path_);
    }

    HTTPRequest::~HTTPRequest()
    {
        #if EMSCRIPTEN
        if (requestHandle_ != -1)
            emscripten_async_wget2_abort(requestHandle_);
        #else
        result_.wait();
        #endif
    }

    void HTTPRequest::ParseURL(const std::string& url, std::string& protocol, std::string& host, int& port, std::string& path)
    {
        protocol = "http";
        port = 80;
        path = "/";

        auto protocolEnd = url.find("://");
        if (protocolEnd != std::string::npos)
        {
            protocol = url.substr(0, protocolEnd);
            host = url.substr(protocolEnd + 3);
        }
        else
            host = url;

        auto pathStart = host.find('/');
        if (pathStart != std::string::npos)
        {
            path = host.substr(pathStart);
            host = host.substr(0, pathStart);
        }

        auto portStart = host.find(':');
        if (portStart != std::string::npos)
        {
            port = ToInt(host.substr(portStart + 1));
            host = host.substr(0, portStart);
        }
    }

    void HTTPRequest::StartRequest()
    {
        std::string postData;

		if (isPost_)
        {
            auto it = form_.begin();
            while (it != form_.end())
            {
                postData += it->first + "=" + it->second;
                ++it;
                if (it != form_.end())
                    postData += "&";
            }
        }

        HTTPRequestData* requestData = new HTTPRequestData{this, postData};

        #if EMSCRIPTEN
        {
            std::string url = protocol_ + "://" + host_ + ":" + ToString(port_) + path_;
            LOGI("REQUEST:%s", url.c_str());
            requestHandle_ = emscripten_async_wget2_data(
                                 url.c_str(),
                                 isPost_ ? "POST" : "GET",
                                 requestData->postData_.c_str(),
                                 requestData,
                                 0,
                                 &HTTPRequest::OnLoad,
                                 &HTTPRequest::OnError,
                                 &HTTPRequest::OnProgress);
        }
        #else
        {
            result_ = std::async(std::launch::async, [this](bool post, HTTPRequestData * p)
            {
                std::unique_ptr<HTTPRequestData> requestData(p);
                std::string headersStr;
                std::string requestType;
                if (post)
                {
                    requestType = "POST";
					headersStr = "content-type:application/x-www-form-urlencoded;\r\n";
					headersStr += "Content-Length: " + ToString(requestData->postData_.size()) + "\r\n";
					headersStr += "\r\n";
					headersStr += requestData->postData_;
                }
                else
                {
                    requestType = "GET";
                }

                int use_ssl = 0;
                static const unsigned ERROR_BUFFER_SIZE = 256;
                char errorBuffer[ERROR_BUFFER_SIZE];
                memset(errorBuffer, 0, sizeof(errorBuffer));
                auto connection = mg_download(host_.c_str(),
                                              port_,
                                              use_ssl,
                                              errorBuffer,
                                              sizeof(errorBuffer),
                                              "%s %s HTTP/1.0\r\n"
                                              "Host: %s\r\n"
                                              "%s\r\n",
                                              requestType.c_str(),
                                              path_.c_str(),
                                              host_.c_str(),
                                              headersStr.c_str());
                if (!connection)
                {
                    onError_(-1, errorBuffer);
                }
                else
                {
                    std::string response;
                    static const unsigned BUFFER_SIZE = 1024;
                    char buffer[BUFFER_SIZE];
                    int bytesRead = 0;
                    do
                    {
                        bytesRead = mg_read(connection, buffer, BUFFER_SIZE);
                        if (bytesRead > 0)
                        {
                            auto idx = response.size();
                            response.resize(idx + bytesRead);
                            memcpy(&response[idx], &buffer[0], bytesRead);
                        }
                    }
                    while (bytesRead > 0);

                    auto ri = mg_get_request_info(connection);
                    if (bytesRead < 0 || mg_strncasecmp(ri->uri, "200", 3) != 0)
                    {
						auto httpError = ToInt(ri->uri);
						mg_close_connection(connection);
						onError_(httpError, response);
                    }
                    else
                    {
                        mg_close_connection(connection);
                        onLoad_(response);
                    }
                }
			}, isPost_, requestData);
        }
        #endif
    }

    #if EMSCRIPTEN
    void HTTPRequest::OnLoad(unsigned int id, void* arg, void* buffer, unsigned bytes)
    {
        std::unique_ptr<HTTPRequestData> requestData(static_cast<HTTPRequestData*>(arg));
        requestData->obj_->requestHandle_ = -1;
        std::string data;
        if (buffer && bytes)
        {
            data.resize(bytes);
            memcpy(&data[0], buffer, bytes);
        }
        requestData->obj_->onLoad_(data);
    }

    void HTTPRequest::OnError(unsigned int id, void* arg, int httpError, const char* statusDescription)
    {
        std::unique_ptr<HTTPRequestData> requestData(static_cast<HTTPRequestData*>(arg));
        requestData->obj_->requestHandle_ = -1;
        requestData->obj_->onError_(httpError, statusDescription);
    }

    void HTTPRequest::OnProgress(unsigned int id, void* arg, int bytesLoaded, int totalSize)
    {
        HTTPRequestData* requestData = static_cast<HTTPRequestData*>(arg);
        unsigned progress = 100;
        if (totalSize)
            progress *= bytesLoaded / totalSize;
        requestData->obj_->onProgress_(progress);
    }

    #endif
}
