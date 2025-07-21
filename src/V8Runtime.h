#pragma once

#include "V8Bindings.h"

#include <iostream>
#include <memory>
#include <string>

#include <v8.h>

class V8Runtime {
 public:
  V8Runtime() : isolate_(nullptr), allocator_(nullptr) {}

  ~V8Runtime() { cleanup(); }

  // Initialize V8 environment and bindings
  void initialize() {
    // Create isolate with allocator
    v8::Isolate::CreateParams createParams;
    allocator_ = v8::ArrayBuffer::Allocator::NewDefaultAllocator();
    createParams.array_buffer_allocator = allocator_;
    isolate_ = v8::Isolate::New(createParams);

    // Initialize context and bindings within proper scopes
    initializeContextAndBindings();
  }

  // Execute JavaScript code
  bool executeScript(const std::string& jsCode) {
    if (!isolate_) {
      std::cerr << "V8 runtime not initialized!" << std::endl;
      return false;
    }

    return runScriptInContext(jsCode);
  }

  // Clean up resources in correct order
  void cleanup() {
    // Clear persistent handles first
    if (!context_.IsEmpty()) {
      context_.Reset();
    }

    // Reset bindings before isolate disposal
    bindings_.reset();

    // Dispose isolate
    if (isolate_) {
      isolate_->Dispose();
      isolate_ = nullptr;
    }

    // Delete allocator last
    if (allocator_) {
      delete allocator_;
      allocator_ = nullptr;
    }
  }

 private:
  void initializeContextAndBindings() {
    v8::Isolate::Scope isolateScope(isolate_);
    v8::HandleScope handleScope(isolate_);

    // Create and persist context
    v8::Local<v8::Context> context = v8::Context::New(isolate_);
    context_.Reset(isolate_, context);

    // Initialize bindings in context
    v8::Context::Scope contextScope(context);
    bindings_ = std::make_unique<V8Bindings>(isolate_, context);
    bindings_->Initialize();
  }

  bool runScriptInContext(const std::string& jsCode) {
    v8::Isolate::Scope isolateScope(isolate_);
    v8::HandleScope handleScope(isolate_);
    const v8::Local<v8::Context> context = context_.Get(isolate_);
    v8::Context::Scope contextScope(context);

    try {
      return compileAndExecute(context, jsCode);
    } catch (const std::exception& e) {
      std::cerr << "Error: " << e.what() << std::endl;
      return false;
    }
  }

  bool compileAndExecute(
      v8::Local<v8::Context> context, const std::string& jsCode
  ) const {
    std::cout << "\nRunning script:\n" << std::endl;
    std::cout << "================================" << std::endl;

    const v8::Local<v8::String> source =
        v8::String::NewFromUtf8(isolate_, jsCode.c_str()).ToLocalChecked();
    const v8::Local<v8::Script> script =
        v8::Script::Compile(context, source).ToLocalChecked();

    if (const v8::Local<v8::Value> result =
            script->Run(context).ToLocalChecked();
        result.IsEmpty()) {
      std::cerr << "Script execution failed!" << std::endl;
      return false;
    }

    std::cout << "================================" << std::endl;
    std::cout << "\nScript completed successfully!" << std::endl;
    return true;
  }

  v8::Isolate* isolate_;
  v8::Persistent<v8::Context> context_;
  std::unique_ptr<V8Bindings> bindings_;
  v8::ArrayBuffer::Allocator* allocator_;
};
