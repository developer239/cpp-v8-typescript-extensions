#pragma once

#include <algorithm>
#include <string>
#include <string_view>

class Recipe {
 public:
  Recipe(std::string_view name, int strength, int waterAmount, int brewTime)
      : name_(name),
        strength_(std::clamp(strength, 0, 100)),
        waterAmount_(std::max(waterAmount, 0)),
        brewTime_(std::max(brewTime, 0)) {}

  const std::string &getName() const noexcept { return name_; }

  int getStrength() const noexcept { return strength_; }

  int getBrewTime() const noexcept { return brewTime_; }

  std::string getDescription() const {
    return name_ + " - Strength: " + std::to_string(strength_) +
           "%, Water: " + std::to_string(waterAmount_) +
           "ml, Time: " + std::to_string(brewTime_) + "ms";
  }

 private:
  std::string name_;
  int strength_;     // Coffee strength percentage (0-100)
  int waterAmount_;  // Water amount in milliliters
  int brewTime_;     // Brewing time in milliseconds
};
