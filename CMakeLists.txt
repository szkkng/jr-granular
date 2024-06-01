# ==============================================================================
#
#  Copyright 2022 Suzuki Kengo
# 
#  JR-Granular is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
# 
#  JR-Granular is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
#  GNU General Public License for more details.
# 
#  You should have received a copy of the GNU General Public License
#  along with JR-Granular. If not, see <http://www.gnu.org/licenses/>.
# 
# ==============================================================================

cmake_minimum_required(VERSION 3.25)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64" CACHE STRING "Build architectures for macOS")
set(CMAKE_OSX_DEPLOYMENT_TARGET "10.13" CACHE STRING "Minimum macOS version required")
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

set(formats "VST3" "Standalone")

if (APPLE)
    list(APPEND formats "AU")
endif ()

project(jr-granular VERSION 1.0.2 LANGUAGES CXX)

add_subdirectory(lib/JUCE)

juce_add_plugin(jr-granular
        COMPANY_NAME "Kengo"
        COMPANY_WEBSITE "https://kengo.dev"
        COMPANY_EMAIL "hi@kengo.dev"
        PLUGIN_MANUFACTURER_CODE "Kngo"
        PLUGIN_CODE "Jrgr"
        FORMATS ${formats}
        COPY_PLUGIN_AFTER_BUILD TRUE
        PRODUCT_NAME "JR-Granular")

include_directories(
        "${CMAKE_CURRENT_SOURCE_DIR}/export/rnbo"
        "${CMAKE_CURRENT_SOURCE_DIR}/export/rnbo/common")

target_sources(jr-granular PRIVATE
        export/rnbo_granular.cpp
        export/rnbo/RNBO.cpp
        src/PluginEditor.cpp
        src/PluginProcessor.cpp
        src/ui/EditorContent.cpp
        src/ui/Dial.cpp
        src/ui/EditorLnf.cpp)

target_compile_features(jr-granular PUBLIC cxx_std_20)

target_compile_definitions(jr-granular PUBLIC
        JUCE_DISPLAY_SPLASH_SCREEN=0
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0)

juce_add_binary_data(binary_data SOURCES
        res/UbuntuRegular.ttf)

target_link_libraries(jr-granular PRIVATE
        binary_data
        juce::juce_audio_utils
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)
