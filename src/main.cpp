#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <string_view>

#include <libplatform/libplatform.h>
#include <v8.h>

#include "V8Bindings.h"

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

// Simple class to manage V8 runtime state
class V8Runtime {
public:
    V8Runtime() : isolate_(nullptr) {}

    ~V8Runtime() {
        if (isolate_) {
            isolate_->Dispose();
        }
    }

    // Initialize V8 environment and bindings
    void initialize() {
        // Create isolate
        v8::Isolate::CreateParams createParams;
        createParams.array_buffer_allocator = v8::ArrayBuffer::Allocator::NewDefaultAllocator();
        isolate_ = v8::Isolate::New(createParams);
        allocator_ = createParams.array_buffer_allocator;

        // Enter isolate scope and create context
        v8::Isolate::Scope isolateScope(isolate_);
        v8::HandleScope handleScope(isolate_);

        // Create and persist context
        auto context = v8::Context::New(isolate_);
        context_.Reset(isolate_, context);

        // Enter context scope and initialize bindings
        v8::Context::Scope contextScope(context);
        bindings_ = std::make_unique<V8Bindings>(isolate_, context);
        bindings_->Initialize();
    }

    // Execute JavaScript code
    bool executeScript(const std::string& jsCode) {
        if (!isolate_) {
            std::cerr << "V8 runtime not initialized!" << std::endl;
            return false;
        }

        // Re-establish scopes for script execution
        v8::Isolate::Scope isolateScope(isolate_);
        v8::HandleScope handleScope(isolate_);
        auto context = context_.Get(isolate_);
        v8::Context::Scope contextScope(context);

        try {
            // Execute JavaScript
            std::cout << "\nRunning script:\n" << std::endl;
            std::cout << "================================" << std::endl;

            auto source = v8::String::NewFromUtf8(isolate_, jsCode.c_str()).ToLocalChecked();
            auto script = v8::Script::Compile(context, source).ToLocalChecked();

            auto result = script->Run(context);
            if (result.IsEmpty()) {
                std::cerr << "Script execution failed!" << std::endl;
                return false;
            } else {
                std::cout << "================================" << std::endl;
                std::cout << "\nScript completed successfully!" << std::endl;
                return true;
            }
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            return false;
        }
    }

    v8::ArrayBuffer::Allocator* getAllocator() { return allocator_; }

private:
    v8::Isolate* isolate_;
    v8::Persistent<v8::Context> context_;
    std::unique_ptr<V8Bindings> bindings_;
    v8::ArrayBuffer::Allocator* allocator_;
};

int main() {
    // Generate TypeScript definitions
    generateTypeDefinitions("../scripts/types.d.ts");

    // Initialize V8 platform
    v8::V8::InitializeICUDefaultLocation("");
    v8::V8::InitializeExternalStartupData("");

    std::unique_ptr<v8::Platform> platform = v8::platform::NewDefaultPlatform();
    v8::V8::InitializePlatform(platform.get());
    v8::V8::Initialize();

    // Create and initialize V8 runtime
    V8Runtime runtime;
    runtime.initialize();

    // At this point, V8 is initialized and bindings are ready
    // We can do other setup here if needed...

    // Later, when we're ready to execute scripts:
    try {
        // Read JavaScript file
        std::cout << "Loading JavaScript from ../scripts/index.js..." << std::endl;
        std::string jsCode = readFile("../scripts/index.js");

        // Execute the script
        runtime.executeScript(jsCode);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        std::cerr << "\nMake sure to compile TypeScript first:" << std::endl;
        std::cerr << "  cd scripts && npx tsc" << std::endl;
    }

    // Cleanup
    delete runtime.getAllocator();
    v8::V8::Dispose();
    v8::V8::DisposePlatform();

    return 0;
}
