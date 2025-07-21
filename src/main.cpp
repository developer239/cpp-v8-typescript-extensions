#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <string_view>

#include <libplatform/libplatform.h>
#include <v8.h>

#include "V8Bindings.h"

std::string compileTypeScript(std::string_view tsFile) {
    std::string command = "./bin/esbuild ";
    command.append(tsFile);
    command.append(" --bundle --format=iife");

    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command.c_str(), "r"), pclose);
    if (!pipe) {
        return "";
    }

    std::stringstream buffer;
    std::array<char, 128> temp{};
    while (fgets(temp.data(), temp.size(), pipe.get()) != nullptr) {
        buffer << temp.data();
    }

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
    generateTypeDefinitions("scripts/types.d.ts");
    generateTypeDefinitions("../scripts/types.d.ts");

    // Initialize V8
    v8::V8::InitializeICUDefaultLocation("");
    v8::V8::InitializeExternalStartupData("");

    std::unique_ptr<v8::Platform> platform = v8::platform::NewDefaultPlatform();
    v8::V8::InitializePlatform(platform.get());
    v8::V8::Initialize();

    // Create isolate
    v8::Isolate::CreateParams createParams;
    createParams.array_buffer_allocator = v8::ArrayBuffer::Allocator::NewDefaultAllocator();
    auto *isolate = v8::Isolate::New(createParams); {
        v8::Isolate::Scope isolateScope(isolate);
        v8::HandleScope handleScope(isolate);

        // Create context
        auto context = v8::Context::New(isolate);
        v8::Context::Scope contextScope(context);

        // Initialize bindings
        V8Bindings bindings(isolate, context);
        bindings.Initialize();

        // Compile TypeScript
        std::cout << "Compiling TypeScript..." << std::endl;
        auto jsCode = compileTypeScript("scripts/index.ts");

        // Execute JavaScript
        std::cout << "\nRunning script:\n" << std::endl;
        std::cout << "================================" << std::endl;

        auto source = v8::String::NewFromUtf8(isolate, jsCode.c_str()).ToLocalChecked();
        auto script = v8::Script::Compile(context, source).ToLocalChecked();
        script->Run(context).ToLocalChecked();

        std::cout << "================================" << std::endl;
        std::cout << "\nScript completed successfully!" << std::endl;
    }

    // Cleanup
    isolate->Dispose();
    v8::V8::Dispose();
    v8::V8::DisposePlatform();
    delete createParams.array_buffer_allocator;

    return 0;
}
