#pragma once

#include <memory>

#include <libplatform/libplatform.h>
#include <v8.h>

class V8Platform {
 public:
  V8Platform() {
    v8::V8::InitializeICUDefaultLocation("");
    v8::V8::InitializeExternalStartupData("");

    platform_ = v8::platform::NewDefaultPlatform();
    v8::V8::InitializePlatform(platform_.get());
    v8::V8::Initialize();
  }

  ~V8Platform() {
    v8::V8::Dispose();
    v8::V8::DisposePlatform();
  }

 private:
  std::unique_ptr<v8::Platform> platform_;
};
