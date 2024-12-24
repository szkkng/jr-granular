include_guard(GLOBAL)

include(FetchContent)

set(pluginval_download_path "https://github.com/Tracktion/pluginval/releases/download/v1.0.4")

if(APPLE)
    set(pluginval_url "${pluginval_download_path}/pluginval_macOS.zip")
    set(pluginval_hash "SHA256=3c4c533bda0c5059eea3ddaea752d757ee2025041f0f47e6bcb0e87f6082b29f")
elseif(WIN32)
    set(pluginval_url "${pluginval_download_path}/pluginval_Windows.zip")
    set(pluginval_hash "SHA256=c08e61ce3b96db41636f8ec7e76f4c7e2c13ebdac7fa1b5a1f52b4f32ec715ab")
elseif(UNIX)
    set(pluginval_url "${pluginval_download_path}/pluginval_Linux.zip")
    set(pluginval_hash "SHA256=c01c49d8063965c4c2dea8324468336768f5c9139e0b1caebde14c2400b55352")
endif()

FetchContent_Declare(pluginval URL ${pluginval_url} URL_HASH ${pluginval_hash})
FetchContent_MakeAvailable(pluginval)

if(APPLE)
    set(pluginval_executable "${pluginval_SOURCE_DIR}/Contents/MacOS/pluginval")
elseif(WIN32)
    set(pluginval_executable "${pluginval_SOURCE_DIR}/pluginval.exe")
elseif(UNIX)
    set(pluginval_executable "${pluginval_SOURCE_DIR}/pluginval")
endif()

if(NOT EXISTS "${pluginval_executable}")
    message(FATAL_ERROR "pluginval executable not found at ${pluginval_executable}")
endif()

function(add_pluginval_tests TARGET)
    get_target_property(target_path ${TARGET} JUCE_PLUGIN_ARTEFACT_FILE)
    add_test(
        NAME pluginval_${TARGET}
        COMMAND "${pluginval_executable}" --strictness-level 10 --validate ${target_path}
    )
    set_tests_properties(pluginval_${TARGET} PROPERTIES REQUIRED_FILES ${target_path})

    message(STATUS "pluginval tests added for target: ${TARGET}")
endfunction()
