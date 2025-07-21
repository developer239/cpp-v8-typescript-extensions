cmake_minimum_required(VERSION 3.20)
project(v8_typescript_demo)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add custom CMake modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

# Find V8 JavaScript engine
find_package(V8 REQUIRED)

# V8 Configuration flags
add_definitions(-DV8_COMPRESS_POINTERS)
add_definitions(-DV8_COMPRESS_POINTERS_IN_ISOLATE_CAGE)
add_definitions(-DV8_31BIT_SMIS_ON_64BIT_ARCH)
add_definitions(-DV8_ENABLE_SANDBOX)

# Platform-specific configuration
if(CMAKE_SYSTEM_PROCESSOR STREQUAL "arm64")
    add_definitions(-DV8_TARGET_ARCH_ARM64)
endif()

# Download esbuild for TypeScript compilation
include(FetchContent)
FetchContent_Declare(
    esbuild
    URL https://registry.npmjs.org/@esbuild/darwin-arm64/-/darwin-arm64-0.19.5.tgz
    DOWNLOAD_EXTRACT_TIMESTAMP ON
)
FetchContent_MakeAvailable(esbuild)

# Copy esbuild binary to build directory
file(COPY ${esbuild_SOURCE_DIR}/bin/esbuild
     DESTINATION ${CMAKE_BINARY_DIR}/bin
     FILE_PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ)

# Main executable
add_executable(v8_demo
    src/main.cpp
    src/models/CoffeeMachine.h
    src/models/Recipe.h
    src/bindings/V8ObjectWrapper.h
    src/bindings/CoffeeMachineBinding.h
    src/bindings/RecipeBinding.h
    src/bindings/GlobalFunctions.h
    src/V8Bindings.h
)

# Include directories
target_include_directories(v8_demo PRIVATE ${V8_INCLUDE_DIRS})

# Link libraries
target_link_libraries(v8_demo PRIVATE ${V8_LIBRARIES})

# Set RPATH for V8 libraries on macOS
if(APPLE)
    set_target_properties(v8_demo PROPERTIES
        BUILD_RPATH "/opt/homebrew/lib;/usr/local/lib"
        INSTALL_RPATH "/opt/homebrew/lib;/usr/local/lib"
    )
endif()

# Copy scripts to build directory for runtime access
file(COPY scripts DESTINATION ${CMAKE_BINARY_DIR})

# Enable compile commands for better IDE support
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
