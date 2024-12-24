include_guard(GLOBAL)

function(codesign_plugins TARGET_NAME)
    if(NOT APPLE)
        message(STATUS "[apple_codesign] Codesign configuration skipped: This environment is not macOS.")
        return()
    endif()

    get_target_property(active_targets ${TARGET_NAME} JUCE_ACTIVE_PLUGIN_TARGETS)
    foreach(sub_target IN LISTS active_targets)
        get_target_property(plugin_path ${sub_target} JUCE_PLUGIN_ARTEFACT_FILE)
        add_custom_command(
            TARGET ${sub_target}
            POST_BUILD
            COMMAND
                codesign --sign "Developer ID Application" --force --strict --timestamp --options runtime --verbose
                ${plugin_path}
            COMMENT "Codesigning target: ${sub_target} at path: ${plugin_path}"
        )
    endforeach()
endfunction()
