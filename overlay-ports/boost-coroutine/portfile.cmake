# Boost.Coroutine is deprecated since Boost 1.83.0.
#
# The standard vcpkg port builds with Boost.Build (b2) and expects
# boost-context to provide a library named boost_context-vc140-mt-gd.lib,
# but vcpkg's cmake-based boost-context port produces boost_context.lib.
# This mismatch causes an "Unable to find file" error on Windows.
#
# This overlay port installs only the headers (no library build) which is
# sufficient because nothing in the cpp-sdk dependency tree actually links
# against Boost.Coroutine at runtime.

vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO boostorg/coroutine
    REF boost-1.83.0
    SHA512 aa2c1f22ac750d9a7f55b6b1c97c0a3db8e409bfbedf6722e77f7c60669db7b8c647c710bf512b2523f1aae819edc647331069e6e25abc5434a16ab4ee1c3105
    HEAD_REF develop
)

# Install headers only — skip the problematic b2 library build
file(INSTALL "${SOURCE_PATH}/include/" DESTINATION "${CURRENT_PACKAGES_DIR}/include")

file(INSTALL "${SOURCE_PATH}/LICENSE_1_0.txt"
     DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}" RENAME copyright)
