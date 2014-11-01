// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#if NACL

#include "NaClURLLoader.h"
#include "Log.h"

#include <stdio.h>
#include <stdlib.h>
#include "ppapi/c/pp_errors.h"
#include "ppapi/c/ppb_instance.h"
#include "ppapi/cpp/module.h"
#include "ppapi/cpp/var.h"

#ifdef WIN32
#undef min
#undef max
#undef PostMessage

// Allow 'this' in initializer list
#pragma warning(disable : 4355)
#endif

#define READ_BUFFER_SIZE 32768

namespace NSG
{
    namespace NaCl
    {
        PNaClURLLoader NaClURLLoader::Create(pp::Instance* instance, const std::string& url)
        {
            NaClURLLoader* p = new NaClURLLoader(instance, url);

            p->Start();

            return PNaClURLLoader(p);
        }

        NaClURLLoader::NaClURLLoader(pp::Instance* instance, const std::string& url)
            : url_(url),
              url_request_(instance),
              url_loader_(instance),
              buffer_(new char[READ_BUFFER_SIZE]),
              cc_factory_(this),
              isDone_(false),
              hasSucceed_(false)
        {
            if(!url_request_.SetURL(url))
                TRACE_LOG("NaClURLLoader: incorrect url:" << url);

            url_request_.SetMethod("GET");
            url_request_.SetRecordDownloadProgress(true);
        }

        NaClURLLoader::~NaClURLLoader()
        {
            delete[] buffer_;
            buffer_ = nullptr;
        }

        bool NaClURLLoader::IsDone() const
        {
            return isDone_;
        }

        const std::string& NaClURLLoader::GetData() const
        {
            return url_response_body_;
        }

        const std::string& NaClURLLoader::GetURL() const
        {
            return url_;
        }

        void NaClURLLoader::Start()
        {
            pp::CompletionCallback cc = cc_factory_.NewCallback(&NaClURLLoader::OnOpen);

            int32_t result = url_loader_.Open(url_request_, cc);
            if(PP_OK_COMPLETIONPENDING != result && PP_OK != result)
                ReportResult(url_, "pp::URLLoader::Open() failed", false);
        }

        void NaClURLLoader::OnOpen(int32_t result)
        {
            if (result != PP_OK)
            {
                ReportResult(url_, "pp::URLLoader::Open() failed", false);
                return;
            }
            int64_t bytes_received = 0;
            int64_t total_bytes_to_be_received = 0;

            if (url_loader_.GetDownloadProgress(&bytes_received, &total_bytes_to_be_received))
            {
                if (total_bytes_to_be_received > 0)
                {
                    url_response_body_.reserve(total_bytes_to_be_received);
                }
            }

            // We will not use the download progress anymore, so just disable it.
            url_request_.SetRecordDownloadProgress(false);

            // Start streaming.
            ReadBody();
        }

        void NaClURLLoader::AppendDataBytes(const char* buffer, int32_t num_bytes)
        {
            if (num_bytes <= 0)
                return;

            // Make sure we don't get a buffer overrun.
            num_bytes = std::min(READ_BUFFER_SIZE, num_bytes);
            // Note that we do *not* try to minimally increase the amount of allocated
            // memory here by calling url_response_body_.reserve().  Doing so causes a
            // lot of string reallocations that kills performance for large files.
            url_response_body_.insert(url_response_body_.end(), buffer, buffer + num_bytes);
        }

        void NaClURLLoader::OnRead(int32_t result)
        {
            if (result == PP_OK)
            {
                // Streaming the file is complete, delete the read buffer since it is
                // no longer needed.
                delete[] buffer_;
                buffer_ = nullptr;
                ReportResult(url_, "", true);
            }
            else if (result > 0)
            {
                // The URLLoader just filled "result" number of bytes into our buffer.
                // Save them and perform another read.
                AppendDataBytes(buffer_, result);
                ReadBody();
            }
            else
            {
                // A read error occurred.
                ReportResult(url_, "pp::URLLoader::ReadResponseBody() result<0", false);
            }
        }

        void NaClURLLoader::ReadBody()
        {
            // Note that you specifically want an "optional" callback here. This will
            // allow ReadBody() to return synchronously, ignoring your completion
            // callback, if data is available. For fast connections and large files,
            // reading as fast as we can will make a large performance difference
            // However, in the case of a synchronous return, we need to be sure to run
            // the callback we created since the loader won't do anything with it.
            pp::CompletionCallback cc = cc_factory_.NewOptionalCallback(&NaClURLLoader::OnRead);
            int32_t result = PP_OK;
            do
            {
                result = url_loader_.ReadResponseBody(buffer_, READ_BUFFER_SIZE, cc);
                // Handle streaming data directly. Note that we *don't* want to call
                // OnRead here, since in the case of result > 0 it will schedule
                // another call to this function. If the network is very fast, we could
                // end up with a deeply recursive stack.
                if (result > 0)
                {
                    AppendDataBytes(buffer_, result);
                }
            }
            while (result > 0);

            if (result != PP_OK_COMPLETIONPENDING)
            {
                // Either we reached the end of the stream (result == PP_OK) or there was
                // an error. We want OnRead to get called no matter what to handle
                // that case, whether the error is synchronous or asynchronous. If the
                // result code *is* COMPLETIONPENDING, our callback will be called
                // asynchronously.
                cc.Run(result);
            }
        }

        void NaClURLLoader::ReportResult(const std::string& fname, const std::string& text, bool success)
        {
            hasSucceed_ = success;

            if (!success)
            {
                url_response_body_.clear();
                TRACE_LOG("NaClURLLoader::Failed to load file=" << fname << " Status=" << text);
            }

            isDone_ = true;
        }

        bool NaClURLLoader::HasSucceed() const
        {
            return hasSucceed_;
        }
    }
}
#endif