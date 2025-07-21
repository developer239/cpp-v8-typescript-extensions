#pragma once

#include <chrono>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <thread>

#include "Recipe.h"

class CoffeeMachine {
 public:
  explicit CoffeeMachine(std::string_view name)
      : name_(name), isOn_(false), isBrewing_(false) {}

  void turnOn() noexcept { isOn_ = true; }

  void turnOff() noexcept {
    isOn_ = false;
    isBrewing_ = false;
  }

  bool canBrew() const noexcept { return isOn_ && !isBrewing_; }

  std::string brew(const std::shared_ptr<Recipe>& recipe) {
    if (!recipe) {
      throw std::invalid_argument("No recipe provided");
    }

    if (!canBrew()) {
      throw std::runtime_error("Machine not ready to brew");
    }

    // Start brewing
    isBrewing_ = true;

    // Simulate brewing delay
    std::this_thread::sleep_for(
        std::chrono::milliseconds(recipe->getBrewTime())
    );

    // Stop brewing
    isBrewing_ = false;

    // Return success message
    return "Coffee ready! Brewed " + recipe->getName();
  }

  const std::string& getName() const noexcept { return name_; }

 private:
  std::string name_;
  bool isOn_;
  bool isBrewing_;
};
