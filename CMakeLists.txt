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
project(jr-granular VERSION 1.0.2)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64")
set(CMAKE_OSX_DEPLOYMENT_TARGET "10.13")
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set(PRODUCT_NAME "JR-Granular")
set(TARGET_NAME "${PROJECT_NAME}")

add_subdirectory(lib/JUCE)

juce_add_plugin(${TARGET_NAME}
    COMPANY_NAME "Kengo"
    COMPANY_WEBSITE "https://kengo.dev"
    COMPANY_EMAIL "hi@kengo.dev"
    BUNDLE_ID "dev.kengo.${PROJECT_NAME}"
    PLUGIN_MANUFACTURER_CODE "Kngo"
    PLUGIN_CODE "Jrgr"
    FORMATS Standalone VST3 AU
    COPY_PLUGIN_AFTER_BUILD TRUE
    PRODUCT_NAME "${PRODUCT_NAME}"
)

add_subdirectory(export)
add_subdirectory(src)

target_compile_definitions(${TARGET_NAME} PUBLIC JUCE_WEB_BROWSER=0 JUCE_USE_CURL=0 JUCE_VST3_CAN_REPLACE_VST2=0)

juce_add_binary_data(binary_data SOURCES res/UbuntuRegular.ttf)

target_link_libraries(
    ${TARGET_NAME}
    PRIVATE
        binary_data
        juce::juce_audio_utils
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

enable_testing()
include(Pluginval)
add_pluginval_tests(${TARGET_NAME}_VST3)

if(DEFINED ENV{CI})
    include(Codesign)
    codesign_plugins(${TARGET_NAME})
endif()

if(APPLE)
    install(TARGETS ${TARGET_NAME}_VST3 LIBRARY DESTINATION "VST3" COMPONENT VST3)
    install(TARGETS ${TARGET_NAME}_AU LIBRARY DESTINATION "Components" COMPONENT AU)
elseif(WIN32)
    install(TARGETS ${TARGET_NAME}_VST3 LIBRARY DESTINATION "." COMPONENT VST3)
endif()

set(PACKAGE_NAME "${PRODUCT_NAME}")
set(PACKAGE_DIRECTORY "${CMAKE_SOURCE_DIR}/dist/jr-granular")
set(PACKAGE_FILE_NAME "${PRODUCT_NAME}")
get_target_property(vendor ${TARGET_NAME} JUCE_COMPANY_NAME)
get_target_property(contact ${TARGET_NAME} JUCE_COMPANY_EMAIL)
set(CPACK_PACKAGE_NAME "${PACKAGE_NAME}")
set(CPACK_PACKAGE_DIRECTORY "${PACKAGE_DIRECTORY}")
set(CPACK_PACKAGE_FILE_NAME "${PACKAGE_FILE_NAME}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "This is ${PACKAGE_FILE_NAME} package.")
set(CPACK_PACKAGE_VENDOR "${vendor}")
set(CPACK_PACKAGE_CONTACT "${contact}")
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/res/license.txt")
set(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/res/readme.txt")
set(CPACK_VERBATIM_VARIABLES TRUE)

if(APPLE)
    set(CPACK_GENERATOR productbuild)
    set(CPACK_COMPONENTS_ALL "VST3" "AU")
    set(CPACK_PACKAGING_INSTALL_PREFIX "/Library/Audio/Plug-Ins")
    set(CPACK_PRODUCTBUILD_IDENTITY_NAME "Developer ID Installer")
    set(CPACK_PRODUCTBUILD_RESOURCES_DIR "${CMAKE_SOURCE_DIR}/res")
    set(CPACK_PRODUCTBUILD_BACKGROUND "background-light.png")
    set(CPACK_PRODUCTBUILD_BACKGROUND_DARKAQUA "background-dark.png")
elseif(WIN32)
    set(CPACK_GENERATOR INNOSETUP)
    set(CPACK_INSTALL_CMAKE_PROJECTS "${CMAKE_CURRENT_BINARY_DIR};${PROJECT_NAME};VST3;/")
    set(CPACK_PACKAGE_INSTALL_DIRECTORY ".")
    set(CPACK_INNOSETUP_INSTALL_ROOT "{commoncf64}/VST3")
    set(CPACK_INNOSETUP_ARCHITECTURE x64)
    set(CPACK_INNOSETUP_ALLOW_CUSTOM_DIRECTORY ON)
    set(CPACK_INNOSETUP_USE_MODERN_WIZARD ON)
    set(CPACK_INNOSETUP_SETUP_Uninstallable no)
    set(CPACK_INNOSETUP_SETUP_UsePreviousAppDir no)
    set(CPACK_INNOSETUP_SETUP_DirExistsWarning no)
    set(CPACK_INNOSETUP_SETUP_AppendDefaultDirName no)
endif()

include(CPack)

if(APPLE)
    cpack_add_component(${TARGET_NAME}_VST3 DISPLAY_NAME "VST3" DESCRIPTION "VST3 plugin" INSTALL_TYPES full)
    cpack_add_component(${TARGET_NAME}_AU DISPLAY_NAME "AU" DESCRIPTION "AU plugin" INSTALL_TYPES full)
    cpack_add_install_type(full)
endif()

include(CIEnv)
