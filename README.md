# V8 TypeScript C++ Integration Demo

This project demonstrates the integration of Google's V8 JavaScript engine with C++ applications, showcasing how to create native bindings that allow TypeScript/JavaScript code to interact seamlessly with C++ objects.

## Overview

The demo implements a coffee machine simulation where C++ classes are exposed to JavaScript through V8 bindings. This pattern is useful for applications that need to provide scripting capabilities with modern JavaScript features.

## Technical Architecture

```
src/
├── models/                    # C++ business logic
│   ├── CoffeeMachine.h       # Coffee machine implementation
│   └── Recipe.h              # Recipe data model
├── bindings/                 # V8 binding layer
│   ├── V8ObjectWrapper.h     # Smart pointer-based object lifecycle management
│   ├── CoffeeMachineBinding.h
│   ├── RecipeBinding.h
│   └── GlobalFunctions.h     # Global JS functions (wait, console)
└── V8Bindings.h             # Binding aggregator

scripts/
├── index.ts                  # TypeScript demonstration
└── types.d.ts               # Auto-generated TypeScript definitions
```

## Key Features

### Native C++ Object Integration
C++ objects are wrapped using `shared_ptr` for proper memory management. V8's garbage collector handles JavaScript references while the wrapper ensures C++ objects are properly destroyed.

```cpp
// C++ class
class Recipe {
    Recipe(std::string_view name, int strength, int waterAmount, int brewTime);
    const std::string& getName() const noexcept;
    int getStrength() const noexcept;
    int getBrewTime() const noexcept;
};
```

```typescript
// TypeScript usage
const espresso = new Recipe("Espresso", 100, 30, 2000);
const result = await coffeeMachine.brew(espresso);
```

### Asynchronous Operations
The binding layer converts C++ operations into JavaScript promises, enabling async/await patterns:

```typescript
async function brewCoffee(recipe: Recipe) {
    coffeeMachine.turnOn();
    await wait(500);  // Promise-based delay
    const result = await coffeeMachine.brew(recipe);  // C++ operation as Promise
    coffeeMachine.turnOff();
}
```

### Type Safety
Auto-generated TypeScript definitions provide compile-time type checking and IDE support:

```typescript
declare class CoffeeMachine {
    constructor(name: string);
    turnOn(): void;
    turnOff(): void;
    brew(recipe: Recipe): Promise<string>;
    getName(): string;
}
```

## V8 vs Lua Comparison

### When to Use V8

V8 is particularly suitable when:

1. **Modern JavaScript Features Required**
   - Native async/await support
   - ES6+ features (destructuring, arrow functions, classes)
   - Built-in JSON handling
   - Array methods (map, filter, reduce)

2. **TypeScript Integration Desired**
   - Static type checking
   - Superior IDE support with IntelliSense
   - Refactoring capabilities
   - Type definitions for APIs

3. **Complex Object Interactions**
   - Direct mapping of C++ objects to JavaScript
   - Shared pointer management for object lifecycle
   - Natural OOP patterns

4. **Developer Ecosystem**
   - Team familiar with JavaScript/TypeScript
   - Need to share code between frontend and embedded scripting
   - Access to npm ecosystem (with appropriate bridging)

### When to Consider Lua

Lua remains advantageous for:

1. **Resource Constraints**
   - Minimal memory footprint (~200KB vs V8's ~10MB)
   - Faster startup time
   - Lower CPU overhead

2. **Simplicity Requirements**
   - Simpler integration API
   - Fewer dependencies
   - Easier to embed in restricted environments

3. **Legacy Systems**
   - Existing Lua codebase
   - Well-established Lua bindings
   - Team expertise in Lua

### Performance Considerations

| Aspect | V8 | Lua |
|--------|----|----|
| Memory Usage | ~10MB baseline | ~200KB baseline |
| Startup Time | 50-100ms | <5ms |
| JIT Compilation | Advanced (TurboFan) | Simple (LuaJIT) |
| Async Support | Native | Coroutine-based |
| Object Passing | Direct with wrapping | Table conversion |

## Building and Running

### Prerequisites
- CMake 3.20+
- C++20 compatible compiler
- V8 development libraries
- Node.js (for TypeScript compilation via esbuild)

### Build Instructions
```bash
mkdir build && cd build
cmake ..
make
./v8_demo
```

```bash
cd scripts
npx -p typescript tsc
```

## Implementation Notes

### Memory Management
The binding layer uses `shared_ptr` throughout because V8's garbage collector controls object lifetime from the JavaScript side. Multiple JavaScript references may exist to the same C++ object, making `unique_ptr` unsuitable.

### Thread Safety
The current implementation uses synchronous delays for simplicity. Production systems should integrate with proper event loops (libuv, custom event loop) for true asynchronous operations.

### Error Handling
V8 exceptions are properly propagated to JavaScript as Promise rejections or thrown errors, maintaining JavaScript error handling paradigms.

## Use Cases

This integration pattern is valuable for:
- Game engines requiring scriptable behavior
- Embedded applications with user-defined logic
- Desktop applications with plugin systems
- IoT devices with customizable automation

## Future Enhancements

- Integration with libuv for proper async I/O
- Performance profiling integration
