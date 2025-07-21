#pragma once

#include <v8.h>
#include <chrono>
#include <iostream>
#include <thread>

class GlobalFunctions {
 public:
  static void Bind(
      v8::Isolate *isolate, v8::Local<v8::Context> context,
      v8::Local<v8::Object> global
  ) {
    // wait() function - returns a Promise that resolves after specified
    // milliseconds
    global
        ->Set(
            context,
            v8::String::NewFromUtf8(isolate, "wait").ToLocalChecked(),
            v8::Function::New(context, waitCallback).ToLocalChecked()
        )
        .Check();

    // Setup console object
    setupConsole(isolate, context, global);
  }

 private:
  static void waitCallback(const v8::FunctionCallbackInfo<v8::Value> &args) {
    auto *isolate = args.GetIsolate();
    v8::HandleScope scope(isolate);
    auto context = isolate->GetCurrentContext();

    if (args.Length() < 1 || !args[0]->IsNumber()) {
      args.GetReturnValue().SetUndefined();
      return;
    }

    auto milliseconds = args[0]->Int32Value(context).FromJust();

    // Create Promise
    auto resolver = v8::Promise::Resolver::New(context).ToLocalChecked();

    // Simulate async wait with synchronous sleep
    // Note: In production, this should be integrated with an event loop
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
    resolver->Resolve(context, v8::Undefined(isolate)).Check();

    args.GetReturnValue().Set(resolver->GetPromise());
  }

  static void setupConsole(
      v8::Isolate *isolate, v8::Local<v8::Context> context,
      v8::Local<v8::Object> global
  ) {
    auto console = v8::Object::New(isolate);

    // console.log implementation with proper object serialization
    console
        ->Set(
            context,
            v8::String::NewFromUtf8(isolate, "log").ToLocalChecked(),
            v8::Function::New(context, consoleLogCallback).ToLocalChecked()
        )
        .Check();

    global
        ->Set(
            context,
            v8::String::NewFromUtf8(isolate, "console").ToLocalChecked(),
            console
        )
        .Check();
  }

  static void consoleLogCallback(
      const v8::FunctionCallbackInfo<v8::Value> &args
  ) {
    auto *isolate = args.GetIsolate();
    v8::HandleScope scope(isolate);
    auto context = isolate->GetCurrentContext();

    for (int i = 0; i < args.Length(); ++i) {
      if (i > 0) {
        std::cout << " ";
      }

      auto arg = args[i];

      if (arg->IsObject() && !arg->IsFunction() && !arg->IsArray()) {
        // Convert object to JSON string for display
        auto global = context->Global();
        auto json =
            global
                ->Get(
                    context,
                    v8::String::NewFromUtf8(isolate, "JSON").ToLocalChecked()
                )
                .ToLocalChecked()
                .As<v8::Object>();

        auto stringify = json->Get(
                                 context,
                                 v8::String::NewFromUtf8(isolate, "stringify")
                                     .ToLocalChecked()
        )
                             .ToLocalChecked()
                             .As<v8::Function>();

        v8::Local<v8::Value> stringifyArgs[] = {arg};
        auto result =
            stringify->Call(context, json, 1, stringifyArgs).ToLocalChecked();

        v8::String::Utf8Value str(isolate, result);
        std::cout << *str;
      } else {
        // Regular string conversion for primitives, arrays, functions
        v8::String::Utf8Value str(isolate, arg);
        std::cout << *str;
      }
    }
    std::cout << std::endl;
  }
};
