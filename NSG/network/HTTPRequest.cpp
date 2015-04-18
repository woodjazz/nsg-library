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
#include "HTTPRequest.h"
#include "Util.h"
#if !defined(EMSCRIPTEN)
#include "civetweb.h"
#else
#include "emscripten.h"
#endif

namespace NSG
{
    struct InitializeComms
    {
        InitializeComms()
        {
            #if defined(_WIN32)
            WSADATA data;
            WSAStartup(MAKEWORD(2, 2), &data);
            #endif
        }

        ~InitializeComms()
        {
            #if defined(_WIN32)
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
          onProgress_(onProgress)
    {
        ParseURL();
        Request(true, form);
    }

    HTTPRequest::HTTPRequest(const std::string& url, OnLoadFunction onLoad, OnErrorFunction onError, OnProgressFunction onProgress)
        : url_(url),
          #if EMSCRIPTEN
          requestHandle_(-1),
          #endif
          onLoad_(onLoad),
          onError_(onError),
          onProgress_(onProgress)
    {
        ParseURL();
        Request(false, Form());
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

    void HTTPRequest::ParseURL()
    {
        protocol_ = "http";
        port_ = 80;
        path_ = "/";

        auto protocolEnd = url_.find("://");
        if (protocolEnd != std::string::npos)
        {
            protocol_ = url_.substr(0, protocolEnd);
            host_ = url_.substr(protocolEnd + 3);
        }
        else
            host_ = url_;

        auto pathStart = host_.find('/');
        if (pathStart != std::string::npos)
        {
            path_ = host_.substr(pathStart);
            host_ = host_.substr(0, pathStart);
        }

        auto portStart = host_.find(':');
        if (portStart != std::string::npos)
        {
            port_ = ToInt(host_.substr(portStart + 1));
            host_ = host_.substr(0, portStart);
        }
    }

    void HTTPRequest::Request(bool post, const Form& form)
    {
        std::string postData;

        if (post)
        {
            auto it = form.begin();
            while (it != form.end())
            {
                postData += it->first + "=" + it->second;
                ++it;
                if (it != form.end())
                    postData += "&";
            }
        }

        HTTPRequestData* requestData = new HTTPRequestData{this, postData};

        #if EMSCRIPTEN
        {
            std::stringstream url;
            url << protocol_ << "://" << host_ << ":" << port_ << path_;
            requestHandle_ = emscripten_async_wget2_data(
                                 url.str().c_str(),
                                 post ? "POST" : "GET",
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
                    std::stringstream ss;
                    ss << "content-type:application/x-www-form-urlencoded;\r\n";
                    ss << "Content-Length: " <<  requestData->postData_.size() << "\r\n";
                    ss << "\r\n";
                    ss << requestData->postData_;
                    headersStr = ss.str();
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
            }, post, requestData);
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