set(DESTINATION_INCLUDE_DIR "${CMAKE_INSTALL_INCLUDEDIR}")

install(
        DIRECTORY "bcos-c-sdk"
        DESTINATION "${DESTINATION_INCLUDE_DIR}"
        FILES_MATCHING PATTERN "*.h"
)