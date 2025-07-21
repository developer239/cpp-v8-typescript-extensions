#pragma once

#include <v8.h>

#include "bindings/CoffeeMachineBinding.h"
#include "bindings/GlobalFunctions.h"
#include "bindings/RecipeBinding.h"

class V8Bindings {
 public:
  V8Bindings(v8::Isolate *isolate, const v8::Local<v8::Context> context)
      : isolate_(isolate), context_(context) {}

  void Initialize() const {
    v8::HandleScope handleScope(isolate_);

    // Get global object
    const auto global = context_->Global();

    // Bind global functions (wait, console)
    GlobalFunctions::Bind(isolate_, context_, global);

    // Bind all classes
    CoffeeMachineBinding::Bind(isolate_, context_, global);
    RecipeBinding::Bind(isolate_, context_, global);
  }

 private:
  v8::Isolate *isolate_;
  v8::Local<v8::Context> context_;
};
