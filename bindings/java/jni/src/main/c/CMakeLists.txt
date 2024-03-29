# ------------------------------------------------------------------------------
# Top-level CMake file for java-sdk-jni
# ------------------------------------------------------------------------------
# Copyright (C) 2021 java-sdk-jni
# SPDX-License-Identifier: Apache-2.0
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
# ------------------------------------------------------------------------------

# Debug, Release, RelWithDebInfo, MinSizeRel
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

if (WIN32)
    add_definitions(-D_WIN32_WINNT=0x0601)
    add_definitions(-DBOOST_USE_WINAPI_VERSION=BOOST_WINAPI_VERSION_WIN7)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS "ON")
    set(VCPKG_C_FLAGS -D_WIN32_WINNT=0x0601 -DWINVER=0x0601)
    set(VCPKG_CXX_FLAGS -D_WIN32_WINNT=0x0601 -DWINVER=0x0601)
endif ()

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

include_directories(${CMAKE_INSTALL_INCLUDEDIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# source
file(GLOB_RECURSE SOURCES "jni/*.h" "jni/*.cpp" "jni/*.c")

# rust static library linking requirements for macos
if(APPLE)
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -framework Security -framework Foundation")
else()
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--no-as-needed -ldl")
endif()

# add suffix for macOS M1
if(CMAKE_HOST_SYSTEM_PROCESSOR STREQUAL "arm64" OR CMAKE_HOST_SYSTEM_PROCESSOR STREQUAL "aarch64")
    set(FISCO_BCOS_JNI_TARGET "bcos-sdk-jni-aarch64")
    set(FISCO_BCOS_JNI_STATIC_TARGET "bcos-sdk-jni-static-aarch64")
else()
    set(FISCO_BCOS_JNI_TARGET "bcos-sdk-jni")
    set(FISCO_BCOS_JNI_STATIC_TARGET "bcos-sdk-jni-static")
endif()

add_library(${FISCO_BCOS_JNI_TARGET} SHARED ${SOURCES})
target_link_libraries(${FISCO_BCOS_JNI_TARGET} PUBLIC wedprcrypto::crypto wedprcrypto::zkp fiscobcos::bcos-cpp-sdk ${BCOS_C_SDK_STATIC_TARGET})

add_library(${FISCO_BCOS_JNI_STATIC_TARGET} ${SOURCES})
target_link_libraries(${FISCO_BCOS_JNI_STATIC_TARGET} PUBLIC wedprcrypto::crypto wedprcrypto::zkp fiscobcos::bcos-cpp-sdk ${BCOS_C_SDK_STATIC_TARGET})

SET(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../resources/META-INF/native/)

message(STATUS "CMAKE_INSTALL_INCLUDEDIR => ${CMAKE_INSTALL_INCLUDEDIR}")
message(STATUS "CMAKE_CURRENT_SOURCE_DIR => ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "LIB_DIR_PATH => ${CMAKE_CURRENT_SOURCE_DIR}/../resources/META-INF/native/")

install(
    TARGETS ${FISCO_BCOS_JNI_TARGET}
    EXPORT "${TARGETS_EXPORT_NAME}"
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
    INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)

install(
    TARGETS ${FISCO_BCOS_JNI_STATIC_TARGET}
    EXPORT "${TARGETS_EXPORT_NAME}"
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
    INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)
