vcpkg_check_linkage(ONLY_STATIC_LIBRARY)

vcpkg_from_github(
        OUT_SOURCE_PATH SOURCE_PATH
        REPO FISCO-BCOS/FISCO-BCOS
        REF 323e5a0d87fef3f698ce8e7d4a9a9e011f59e2ce
        SHA512 1143a423cbe72622843e477a4403883d2ab124ad028a7786ac88430da82036e4bf987ff6efea84342e0b4b3a8f10ae1d85a4bdcfbae11ea0992ea121ea26ac4c
        HEAD_REF master
        PATCHES 
            "${CMAKE_CURRENT_LIST_DIR}/fix-unused-this.patch"
            "${CMAKE_CURRENT_LIST_DIR}/fix-memory-leak.patch"
            "${CMAKE_CURRENT_LIST_DIR}/fix-handler-cleanup.patch"
)

vcpkg_cmake_configure(
        SOURCE_PATH "${SOURCE_PATH}"
        OPTIONS
        -DALLOCATOR=default
        -DTESTS=OFF
        -DWITH_TIKV=OFF
        -DWITH_TARS_SERVICES=OFF
        -DWITH_LIGHTNODE=OFF
        -DWITH_CPPSDK=ON
        -DBUILD_STATIC=OFF
        -DTOOLS=OFF
        -DFULLNODE=OFF
        -DWITH_WASM=OFF
)

vcpkg_cmake_build()
vcpkg_cmake_install()

file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/include")
file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/share")
file(INSTALL "${SOURCE_PATH}/LICENSE" DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}" RENAME copyright)
