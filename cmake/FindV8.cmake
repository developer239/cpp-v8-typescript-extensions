# FindV8.cmake - Find Google V8 JavaScript Engine

# Using homebrew on macOS as an example
if(APPLE)
    find_path(V8_INCLUDE_DIR
            NAMES v8.h
            PATHS
            /opt/homebrew/include
            /usr/local/include
    )

    find_library(V8_BASE_LIBRARY
            NAMES v8_libbase
            PATHS
            /opt/homebrew/lib
            /usr/local/lib
    )

    find_library(V8_PLATFORM_LIBRARY
            NAMES v8_libplatform
            PATHS
            /opt/homebrew/lib
            /usr/local/lib
    )

    find_library(V8_LIBRARY
            NAMES v8
            PATHS
            /opt/homebrew/lib
            /usr/local/lib
    )
endif()

set(V8_LIBRARIES ${V8_LIBRARY} ${V8_BASE_LIBRARY} ${V8_PLATFORM_LIBRARY})
set(V8_INCLUDE_DIRS ${V8_INCLUDE_DIR})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(V8 DEFAULT_MSG V8_LIBRARY V8_INCLUDE_DIR)

mark_as_advanced(V8_INCLUDE_DIR V8_LIBRARY V8_BASE_LIBRARY V8_PLATFORM_LIBRARY)
