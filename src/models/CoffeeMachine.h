#pragma once

#include <string>
#include <string_view>

class CoffeeMachine {
 public:
  explicit CoffeeMachine(std::string_view name)
      : name_(name), isOn_(false), isBrewing_(false) {}

  void turnOn() noexcept { isOn_ = true; }

  void turnOff() noexcept {
    isOn_ = false;
    isBrewing_ = false;
  }

  [[nodiscard]] bool canBrew() const noexcept { return isOn_ && !isBrewing_; }

  void startBrewing() {
    if (canBrew()) {
      isBrewing_ = true;
    }
  }

  void stopBrewing() noexcept { isBrewing_ = false; }

  [[nodiscard]] const std::string &getName() const noexcept { return name_; }

 private:
  std::string name_;
  bool isOn_;
  bool isBrewing_;
};
