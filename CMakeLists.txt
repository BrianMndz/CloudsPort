# Example Audio Plugin CMakeLists.txt
cmake_minimum_required(VERSION 3.24)

set(PROJECT_NAME CloudsPort)

# Change here the project name
project(${PROJECT_NAME} VERSION 0.0.1)

set (CMAKE_CXX_STANDARD 17)
set (COMPANY_NAME "Company Name")
set (CMAKE_XCODE_GENERATE_SCHEME OFF)
set (CMAKE_EXPORT_COMPILE_COMMANDS on)
set (CMAKE_OSX_ARCHITECTURES "x86_64;arm64")
set (CMAKE_OSX_DEPLOYMENT_TARGET "10.15")

add_subdirectory(Modules/JUCE)                    # If you've put JUCE in a subdirectory called JUCE

# If you are building a VST2 or AAX plugin, CMake needs to be told where to find these SDKs on your
# system. This setup should be done before calling `juce_add_plugin`.

set_property(GLOBAL PROPERTY USE_FOLDERS YES)
option(JUCE_ENABLE_MODULE_SOURCE_GROUPS "Show all module sources in IDE projects" ON)

# juce_set_vst2_sdk_path(...)
# juce_set_aax_sdk_path(...)

# Source Files
macro (setSourceFiles sourceFiles)
    file (GLOB_RECURSE SOURCES
            "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
    )
    set (${sourceFiles} ${SOURCES})
endmacro()

add_subdirectory(Source)
