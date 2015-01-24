// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#pragma once

#include <string>
#include <memory>
#include "ppapi/cpp/completion_callback.h"
#include "ppapi/cpp/instance.h"
#include "ppapi/cpp/url_loader.h"
#include "ppapi/cpp/url_request_info.h"
#include "ppapi/utility/completion_callback_factory.h"
#include "NonCopyable.h"
#include "SharedPointers.h"

namespace NSG
{
  namespace NaCl 
  {
    class NaClURLLoader : NSG::NonCopyable 
    {
     public:
      static PNaClURLLoader Create(pp::Instance* instance_, const std::string& url);
      ~NaClURLLoader();
      bool IsDone() const;
      bool HasSucceed() const;
      const std::string& GetData() const;
      const std::string& GetURL() const;

     private:
      void Start();
      NaClURLLoader(pp::Instance* instance_, const std::string& url);
      void OnOpen(int32_t result);
      void OnRead(int32_t result);
      void ReadBody();
      void AppendDataBytes(const char* buffer, int32_t num_bytes);
      void ReportResult(const std::string& fname, const std::string& text, bool success);

      std::string url_;         // URL to be downloaded.
      pp::URLRequestInfo url_request_;
      pp::URLLoader url_loader_;  // URLLoader provides an API to download URLs.
      char* buffer_;              // Temporary buffer for reads.
      std::string url_response_body_;  // Contains accumulated downloaded data.
      pp::CompletionCallbackFactory<NaClURLLoader> cc_factory_;
      std::atomic<bool> isDone_;
      std::atomic<bool> hasSucceed_;
    };
  }
}
