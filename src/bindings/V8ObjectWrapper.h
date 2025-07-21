#pragma once

#include <v8.h>
#include <memory>

// Base class for wrapping C++ objects in V8.
// Manages object lifetime using shared_ptr to ensure proper cleanup.
//
// Note: We cannot use unique_ptr here because V8's garbage collector determines
// when objects are destroyed, and multiple JavaScript references may exist to
// the same C++ object. shared_ptr ensures the object stays alive as long as
// any V8 wrapper exists.
template <typename T>
class V8ObjectWrapper {
 public:
  static void wrap(
      v8::Local<v8::Object> jsObject, std::shared_ptr<T> cppObject
  ) {
    auto *wrapper = new V8ObjectWrapper<T>(std::move(cppObject));
    jsObject->SetInternalField(
        0,
        v8::External::New(jsObject->GetIsolate(), wrapper)
    );

    // Set up weak callback for cleanup when V8 garbage collects the object
    auto *isolate = jsObject->GetIsolate();
    auto persistent =
        std::make_unique<v8::Global<v8::Object> >(isolate, jsObject);
    persistent->SetWeak(
        wrapper,
        [](const v8::WeakCallbackInfo<V8ObjectWrapper<T> > &data) {
          delete data.GetParameter();
        },
        v8::WeakCallbackType::kParameter
    );
    persistent.release();  // V8 now owns this
  }

  static std::shared_ptr<T> unwrap(const v8::Local<v8::Object> jsObject) {
    if (jsObject->InternalFieldCount() == 0) {
      return nullptr;
    }

    auto *wrapper = static_cast<V8ObjectWrapper<T> *>(
        jsObject->GetInternalField(0).As<v8::External>()->Value()
    );
    return wrapper ? wrapper->object_ : nullptr;
  }

 private:
  explicit V8ObjectWrapper(std::shared_ptr<T> object)
      : object_(std::move(object)) {}

  std::shared_ptr<T> object_;
};
