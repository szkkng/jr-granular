set(vst3_path
    VST3${CPACK_PACKAGING_INSTALL_PREFIX}/VST3/${CPACK_JUCE_PRODUCT_NAME}.vst3
)
set(au_path
    AU${CPACK_PACKAGING_INSTALL_PREFIX}/Components/${CPACK_JUCE_PRODUCT_NAME}.component
)

# CPACK_PRE_BUILD_SCRIPTS or CPACK_POST_BUILD_SCRIPTS continues despite error https://gitlab.kitware.com/cmake/cmake/-/issues/26232
execute_process(
    COMMAND
        codesign -s "Developer ID Application" --force --strict --timestamp
        --options runtime --verbose ${vst3_path} ${au_path}
    WORKING_DIRECTORY ${CPACK_TEMPORARY_DIRECTORY}
    COMMAND_ECHO STDOUT
    COMMAND_ERROR_IS_FATAL ANY
)
