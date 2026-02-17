cmake_minimum_required(VERSION 3.15)
project(TeamsBICSandbox VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# WebView2 SDK for macOS
if(APPLE)
    set(WEBVIEW2_FRAMEWORK_PATH "${CMAKE_SOURCE_DIR}/external/webview2-macos/MSWebView2Staging.framework")
    if(EXISTS "${WEBVIEW2_FRAMEWORK_PATH}")
        message(STATUS "WebView2 framework found: ${WEBVIEW2_FRAMEWORK_PATH}")
        include_directories("${WEBVIEW2_FRAMEWORK_PATH}/Headers")
    else()
        message(FATAL_ERROR "WebView2 framework not found at: ${WEBVIEW2_FRAMEWORK_PATH}")
    endif()
endif()

# Legacy webview library (will be removed)
# include_directories(${CMAKE_SOURCE_DIR}/vendor/webview/core/include)

# Video source files (common)
set(VIDEO_SOURCES
    src/video/video_participant.cpp
    src/video/video_renderer.cpp
    src/video/video_grid_manager.cpp
    src/video/webview_helper.cpp  # Stubbed for now - needs WebView2 update
)

# Window management source files (platform-specific)
set(WINDOW_SOURCES)

# Platform-specific video sources and helpers
if(APPLE)
    list(APPEND VIDEO_SOURCES
        src/video/platform/macos/video_renderer_macos.mm
        src/platform/macos/platform_helper_macos.mm
        src/video/platform/macos/drag_overlay_view_macos.mm
    )
    list(APPEND WINDOW_SOURCES
        src/window/platform/macos/child_window_macos.mm
        src/window/platform/macos/window_manager_macos.mm
        # src/window/webview_factory.cpp  # Removed - was for old webview library
        src/webview/platform/macos/webview2_wrapper_macos.mm
    )
    # Set Objective-C++ properties for .mm files
    set_source_files_properties(
        src/main.mm
        src/app.mm
        src/video/platform/macos/video_renderer_macos.mm
        src/platform/macos/platform_helper_macos.mm
        src/video/platform/macos/drag_overlay_view_macos.mm
        src/window/platform/macos/child_window_macos.mm
        src/window/platform/macos/window_manager_macos.mm
        src/webview/platform/macos/webview2_wrapper_macos.mm
        PROPERTIES COMPILE_FLAGS "-fobjc-arc"
    )
elseif(WIN32)
    list(APPEND VIDEO_SOURCES
        src/video/platform/windows/video_renderer_windows.cpp
        src/platform/windows/platform_helper_windows.cpp
    )
elseif(UNIX)
    list(APPEND VIDEO_SOURCES
        src/video/platform/linux/video_renderer_linux.cpp
        src/platform/linux/platform_helper_linux.cpp
    )
endif()

# Source files
set(SOURCES
    src/main.mm
    src/app.mm
    ${VIDEO_SOURCES}
    ${WINDOW_SOURCES}
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Platform-specific configurations
if(APPLE)
    # macOS: Link against WebKit and Cocoa frameworks
    find_library(WEBKIT_FRAMEWORK WebKit REQUIRED)
    find_library(COCOA_FRAMEWORK Cocoa REQUIRED)

    # Video frameworks for macOS
    find_library(AVFOUNDATION_FRAMEWORK AVFoundation REQUIRED)
    find_library(COREMEDIA_FRAMEWORK CoreMedia REQUIRED)
    find_library(QUARTZCORE_FRAMEWORK QuartzCore REQUIRED)
    find_library(UNIFORMTYPEIDENTIFIERS_FRAMEWORK UniformTypeIdentifiers REQUIRED)

    # WebView2 framework
    set(WEBVIEW2_FRAMEWORK "${CMAKE_SOURCE_DIR}/external/webview2-macos/MSWebView2Staging.framework")

    target_link_libraries(${PROJECT_NAME} PRIVATE
        ${WEBKIT_FRAMEWORK}
        ${COCOA_FRAMEWORK}
        ${AVFOUNDATION_FRAMEWORK}
        ${COREMEDIA_FRAMEWORK}
        ${QUARTZCORE_FRAMEWORK}
        ${UNIFORMTYPEIDENTIFIERS_FRAMEWORK}
        ${WEBVIEW2_FRAMEWORK}
    )

    # Enable ARC and set macOS deployment target
    target_compile_options(${PROJECT_NAME} PRIVATE -fobjc-arc)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        MACOSX_BUNDLE FALSE
    )

elseif(WIN32)
    # Windows: Link against required libraries
    target_link_libraries(${PROJECT_NAME} PRIVATE
        ole32
        oleaut32
        uuid
        gdi32
        user32
    )

    # Download WebView2 SDK if needed
    set(WEBVIEW2_VERSION "1.0.2210.55")
    set(WEBVIEW2_DIR "${CMAKE_SOURCE_DIR}/vendor/webview2")
    if(NOT EXISTS "${WEBVIEW2_DIR}")
        message(STATUS "WebView2 SDK will be used from system installation")
    endif()

    # Set Windows subsystem
    if(MSVC)
        set_target_properties(${PROJECT_NAME} PROPERTIES
            WIN32_EXECUTABLE TRUE
        )
    endif()

elseif(UNIX)
    # Linux: Find GTK3 and WebKit2GTK
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
    pkg_check_modules(WEBKIT2 REQUIRED webkit2gtk-4.0)

    # Include directories
    target_include_directories(${PROJECT_NAME} PRIVATE
        ${GTK3_INCLUDE_DIRS}
        ${WEBKIT2_INCLUDE_DIRS}
    )

    # Link libraries
    target_link_libraries(${PROJECT_NAME} PRIVATE
        ${GTK3_LIBRARIES}
        ${WEBKIT2_LIBRARIES}
    )

    # Link directories
    target_link_directories(${PROJECT_NAME} PRIVATE
        ${GTK3_LIBRARY_DIRS}
        ${WEBKIT2_LIBRARY_DIRS}
    )

    # Compile flags
    target_compile_options(${PROJECT_NAME} PRIVATE
        ${GTK3_CFLAGS_OTHER}
        ${WEBKIT2_CFLAGS_OTHER}
    )
endif()

# Build React application
find_program(NPM_EXECUTABLE npm REQUIRED)

# Custom target to build React app
add_custom_target(build_react
    COMMAND ${NPM_EXECUTABLE} install
    COMMAND ${NPM_EXECUTABLE} run build
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/ui
    COMMENT "Building React application..."
)

# Make the main target depend on React build
add_dependencies(${PROJECT_NAME} build_react)

# Copy React dist files to build directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/ui/dist
        ${CMAKE_BINARY_DIR}/ui
    COMMENT "Copying React build files to binary directory..."
)

# Print build information
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "System: ${CMAKE_SYSTEM_NAME}")
