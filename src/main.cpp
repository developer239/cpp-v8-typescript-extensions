#include "V8Platform.h"
#include "V8Runtime.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>

#include <v8.h>

std::string readFile(const std::string& filepath) {
  std::ifstream file(filepath);
  if (!file.is_open()) {
    throw std::runtime_error("Failed to open file: " + filepath);
  }

  std::stringstream buffer;
  buffer << file.rdbuf();
  return buffer.str();
}

void generateTypeDefinitions(std::string_view outputPath) {
  std::ofstream file(outputPath.data());
  file << R"(// Auto-generated TypeScript definitions for V8 bindings

/**
 * Waits for the specified number of milliseconds.
 * @param milliseconds The number of milliseconds to wait
 * @returns A promise that resolves after the specified delay
 */
declare function wait(milliseconds: number): Promise<void>;

/**
 * Represents a coffee machine that can brew recipes.
 */
declare class CoffeeMachine {
    /**
     * Creates a new coffee machine instance.
     * @param name The name of the coffee machine
     */
    constructor(name: string);

    /**
     * Turns on the coffee machine.
     */
    turnOn(): void;

    /**
     * Turns off the coffee machine.
     */
    turnOff(): void;

    /**
     * Brews coffee using the specified recipe.
     * @param recipe The recipe to brew
     * @returns A promise that resolves with a success message when brewing is complete
     */
    brew(recipe: Recipe): Promise<string>;

    /**
     * Gets the name of the coffee machine.
     * @returns The machine name
     */
    getName(): string;
}

/**
 * Represents a coffee recipe with brewing parameters.
 */
declare class Recipe {
    /**
     * Creates a new recipe.
     * @param name The name of the recipe
     * @param strength The coffee strength (0-100)
     * @param waterAmount The amount of water in milliliters
     * @param brewTime The brewing time in milliseconds
     */
    constructor(name: string, strength: number, waterAmount: number, brewTime: number);

    /**
     * Gets the recipe name.
     * @returns The recipe name
     */
    getName(): string;

    /**
     * Gets the coffee strength.
     * @returns The strength percentage (0-100)
     */
    getStrength(): number;

    /**
     * Gets the brewing time.
     * @returns The brewing time in milliseconds
     */
    getBrewTime(): number;

    /**
     * Gets a formatted description of the recipe.
     * @returns A string describing the recipe parameters
     */
    getDescription(): string;
}

/**
 * Console object for logging.
 */
declare const console: {
    /**
     * Logs messages to the console.
     * @param args The values to log
     */
    log(...args: any[]): void;
};
)";
  file.close();
}

int main() {
  // Generate TypeScript definitions
  generateTypeDefinitions("../scripts/types.d.ts");

  // Initialize V8 platform (RAII handles cleanup)
  V8Platform platform;

  // Create and initialize V8 runtime
  V8Runtime runtime;
  runtime.initialize();

  // Load and execute script when ready
  try {
    std::cout << "Loading JavaScript from ../scripts/index.js..." << std::endl;
    const std::string jsCode = readFile("../scripts/index.js");
    runtime.executeScript(jsCode);
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    std::cerr << "\nMake sure to compile TypeScript first:" << std::endl;
    std::cerr << "  cd scripts && npx -p typescript tsc" << std::endl;
  }

  return 0;
}
