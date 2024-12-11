set(vst3_path VST3${CPACK_PACKAGING_INSTALL_PREFIX}/VST3/${CPACK_JUCE_PRODUCT_NAME}.vst3)
set(au_path AU${CPACK_PACKAGING_INSTALL_PREFIX}/Components/${CPACK_JUCE_PRODUCT_NAME}.component)

execute_process(
    COMMAND
        codesign -s "Developer ID Application" --force --strict --timestamp --options runtime --verbose ${vst3_path}
        ${au_path}
    WORKING_DIRECTORY ${CPACK_TEMPORARY_DIRECTORY}
    COMMAND_ECHO STDOUT
    COMMAND_ERROR_IS_FATAL ANY
)
