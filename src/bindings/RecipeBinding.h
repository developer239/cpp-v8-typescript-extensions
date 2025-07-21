#pragma once

#include <v8.h>
#include <memory>

#include "../models/Recipe.h"
#include "V8ObjectWrapper.h"

class RecipeBinding {
 public:
  static void Bind(
      v8::Isolate *isolate, const v8::Local<v8::Context> context,
      const v8::Local<v8::Object> global
  ) {
    const auto recipeTemplate = v8::FunctionTemplate::New(isolate);
    recipeTemplate->SetClassName(
        v8::String::NewFromUtf8(isolate, "Recipe").ToLocalChecked()
    );

    // Constructor
    recipeTemplate->SetCallHandler(
        [](const v8::FunctionCallbackInfo<v8::Value> &args) {
          auto *isolate = args.GetIsolate();
          v8::HandleScope scope(isolate);
          const auto context = isolate->GetCurrentContext();

          if (args.IsConstructCall()) {
            std::string name = "Custom Recipe";
            int strength = 50;
            int waterAmount = 250;
            int brewTime = 2000;

            if (args.Length() > 0 && args[0]->IsString()) {
              v8::String::Utf8Value str(isolate, args[0]);
              name = *str;
            }
            if (args.Length() > 1 && args[1]->IsNumber()) {
              strength = args[1]->Int32Value(context).FromJust();
            }
            if (args.Length() > 2 && args[2]->IsNumber()) {
              waterAmount = args[2]->Int32Value(context).FromJust();
            }
            if (args.Length() > 3 && args[3]->IsNumber()) {
              brewTime = args[3]->Int32Value(context).FromJust();
            }

            const auto recipe =
                std::make_shared<Recipe>(name, strength, waterAmount, brewTime);
            V8ObjectWrapper<Recipe>::wrap(args.This(), recipe);
            args.GetReturnValue().Set(args.This());
          }
        }
    );

    // Instance template
    const auto instanceTemplate = recipeTemplate->InstanceTemplate();
    instanceTemplate->SetInternalFieldCount(1);

    // Methods
    instanceTemplate->Set(
        v8::String::NewFromUtf8(isolate, "getName").ToLocalChecked(),
        v8::FunctionTemplate::New(
            isolate,
            [](const v8::FunctionCallbackInfo<v8::Value> &args) {
              if (const auto recipe =
                      V8ObjectWrapper<Recipe>::unwrap(args.This())) {
                args.GetReturnValue().Set(
                    v8::String::NewFromUtf8(
                        args.GetIsolate(),
                        recipe->getName().c_str()
                    )
                        .ToLocalChecked()
                );
              }
            }
        )
    );

    instanceTemplate->Set(
        v8::String::NewFromUtf8(isolate, "getStrength").ToLocalChecked(),
        v8::FunctionTemplate::New(
            isolate,
            [](const v8::FunctionCallbackInfo<v8::Value> &args) {
              if (const auto recipe =
                      V8ObjectWrapper<Recipe>::unwrap(args.This())) {
                args.GetReturnValue().Set(recipe->getStrength());
              }
            }
        )
    );

    instanceTemplate->Set(
        v8::String::NewFromUtf8(isolate, "getBrewTime").ToLocalChecked(),
        v8::FunctionTemplate::New(
            isolate,
            [](const v8::FunctionCallbackInfo<v8::Value> &args) {
              if (const auto recipe =
                      V8ObjectWrapper<Recipe>::unwrap(args.This())) {
                args.GetReturnValue().Set(recipe->getBrewTime());
              }
            }
        )
    );

    instanceTemplate->Set(
        v8::String::NewFromUtf8(isolate, "getDescription").ToLocalChecked(),
        v8::FunctionTemplate::New(
            isolate,
            [](const v8::FunctionCallbackInfo<v8::Value> &args) {
              if (const auto recipe =
                      V8ObjectWrapper<Recipe>::unwrap(args.This())) {
                args.GetReturnValue().Set(
                    v8::String::NewFromUtf8(
                        args.GetIsolate(),
                        recipe->getDescription().c_str()
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
            v8::String::NewFromUtf8(isolate, "Recipe").ToLocalChecked(),
            recipeTemplate->GetFunction(context).ToLocalChecked()
        )
        .Check();
  }
};
