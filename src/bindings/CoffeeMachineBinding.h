#pragma once

#include <v8.h>
#include <chrono>
#include <memory>
#include <thread>

#include "../models/CoffeeMachine.h"
#include "../models/Recipe.h"
#include "V8ObjectWrapper.h"

class CoffeeMachineBinding {
 public:
  static void Bind(
      v8::Isolate *isolate, v8::Local<v8::Context> context,
      v8::Local<v8::Object> global
  ) {
    auto coffeeTemplate = v8::FunctionTemplate::New(isolate);
    coffeeTemplate->SetClassName(
        v8::String::NewFromUtf8(isolate, "CoffeeMachine").ToLocalChecked()
    );

    // Constructor
    coffeeTemplate->SetCallHandler(
        [](const v8::FunctionCallbackInfo<v8::Value> &args) {
          auto *isolate = args.GetIsolate();
          v8::HandleScope scope(isolate);

          if (args.IsConstructCall()) {
            std::string name = "Coffee Machine";
            if (args.Length() > 0 && args[0]->IsString()) {
              v8::String::Utf8Value str(isolate, args[0]);
              name = *str;
            }

            auto machine = std::make_shared<CoffeeMachine>(name);
            V8ObjectWrapper<CoffeeMachine>::wrap(args.This(), machine);
            args.GetReturnValue().Set(args.This());
          }
        }
    );

    // Instance template
    auto instanceTemplate = coffeeTemplate->InstanceTemplate();
    instanceTemplate->SetInternalFieldCount(1);

    // Methods
    instanceTemplate->Set(
        v8::String::NewFromUtf8(isolate, "turnOn").ToLocalChecked(),
        v8::FunctionTemplate::New(
            isolate,
            [](const v8::FunctionCallbackInfo<v8::Value> &args) {
              if (auto machine =
                      V8ObjectWrapper<CoffeeMachine>::unwrap(args.This())) {
                machine->turnOn();
              }
            }
        )
    );

    instanceTemplate->Set(
        v8::String::NewFromUtf8(isolate, "turnOff").ToLocalChecked(),
        v8::FunctionTemplate::New(
            isolate,
            [](const v8::FunctionCallbackInfo<v8::Value> &args) {
              if (auto machine =
                      V8ObjectWrapper<CoffeeMachine>::unwrap(args.This())) {
                machine->turnOff();
              }
            }
        )
    );

    instanceTemplate->Set(
        v8::String::NewFromUtf8(isolate, "brew").ToLocalChecked(),
        v8::FunctionTemplate::New(isolate, brewCallback)
    );

    instanceTemplate->Set(
        v8::String::NewFromUtf8(isolate, "getName").ToLocalChecked(),
        v8::FunctionTemplate::New(
            isolate,
            [](const v8::FunctionCallbackInfo<v8::Value> &args) {
              if (auto machine =
                      V8ObjectWrapper<CoffeeMachine>::unwrap(args.This())) {
                args.GetReturnValue().Set(
                    v8::String::NewFromUtf8(
                        args.GetIsolate(),
                        machine->getName().c_str()
                    )
                        .ToLocalChecked()
                );
              }
            }
        )
    );

    // Set constructor to global
    global
        ->Set(
            context,
            v8::String::NewFromUtf8(isolate, "CoffeeMachine").ToLocalChecked(),
            coffeeTemplate->GetFunction(context).ToLocalChecked()
        )
        .Check();
  }

 private:
  static void brewCallback(const v8::FunctionCallbackInfo<v8::Value> &args) {
    auto *isolate = args.GetIsolate();
    v8::HandleScope scope(isolate);
    auto context = isolate->GetCurrentContext();

    auto machine = V8ObjectWrapper<CoffeeMachine>::unwrap(args.This());
    if (!machine) {
      args.GetReturnValue().SetUndefined();
      return;
    }

    // Extract Recipe from argument
    std::shared_ptr<Recipe> recipe;
    if (args.Length() > 0 && args[0]->IsObject()) {
      auto recipeObj = args[0].As<v8::Object>();
      recipe = V8ObjectWrapper<Recipe>::unwrap(recipeObj);
    }

    // Create and return a Promise
    auto resolver = v8::Promise::Resolver::New(context).ToLocalChecked();

    if (!machine->canBrew()) {
      resolver
          ->Reject(
              context,
              v8::String::NewFromUtf8(isolate, "Machine not ready to brew")
                  .ToLocalChecked()
          )
          .Check();
    } else if (!recipe) {
      resolver
          ->Reject(
              context,
              v8::String::NewFromUtf8(isolate, "No recipe provided")
                  .ToLocalChecked()
          )
          .Check();
    } else {
      machine->startBrewing();

      // Simulate brewing delay
      // Note: In production, this should be integrated with an event loop
      std::this_thread::sleep_for(
          std::chrono::milliseconds(recipe->getBrewTime())
      );

      machine->stopBrewing();

      std::string result = "Coffee ready! Brewed " + recipe->getName();
      resolver
          ->Resolve(
              context,
              v8::String::NewFromUtf8(isolate, result.c_str()).ToLocalChecked()
          )
          .Check();
    }

    args.GetReturnValue().Set(resolver->GetPromise());
  }
};
