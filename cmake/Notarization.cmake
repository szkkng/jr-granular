execute_process(
    COMMAND
        xcrun notarytool submit --wait --apple-id "$ENV{APPLE_DEVELOPER_ID}" --team-id "$ENV{APPLE_TEAM_ID}" --password
        "$ENV{APPLE_PASSWORD}" "${CPACK_PACKAGE_FILES}"
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
