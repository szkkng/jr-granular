execute_process(
    COMMAND
        xcrun notarytool submit "${CPACK_PACKAGE_FILES}" --keychain-profile "notarytool-password" --wait
    TIMEOUT 120
    COMMAND_ECHO STDOUT
    COMMAND_ERROR_IS_FATAL ANY
)

execute_process(
    COMMAND
        xcrun stapler staple "${CPACK_PACKAGE_FILES}"
    COMMAND_ECHO STDOUT
    COMMAND_ERROR_IS_FATAL ANY
)
