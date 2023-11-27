# ------------------------------------------------------------------------------
# Top-level CMake file for bcos-c-sdk
# ------------------------------------------------------------------------------
# Copyright (C) 2021 bcos-c-sdk
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

cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD 20)

#set(CMAKE_OSX_DEPLOYMENT_TARGET "13" CACHE STRING "Minimum OS X deployment version")

# URL_BASE download domain name
if (NOT DEFINED URL_BASE)
    set(URL_BASE "github.com")
endif ()

if (WIN32)
    set(VCPKG_C_FLAGS -D_WIN32_WINNT=0x0601 -DWINVER=0x0601)
    set(VCPKG_CXX_FLAGS -D_WIN32_WINNT=0x0601 -DWINVER=0x0601)
    add_definitions(-D_WIN32_WINNT=0x0601)
    add_definitions(-DBOOST_USE_WINAPI_VERSION=BOOST_WINAPI_VERSION_WIN7)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS "ON")
endif ()

set(JAVA_SDK_JNI_CMAKE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/cmake" CACHE PATH "The cmake path for the library")

# list(APPEND CMAKE_MODULE_PATH ${BCOS_CMAKE_SCRIPTS_DIR})
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
list(APPEND CMAKE_MODULE_PATH ${JAVA_SDK_JNI_CMAKE_DIR})

include(Options)
set(ARCH_NATIVE OFF)

if ("${ARCHITECTURE}" MATCHES "aarch64")
    set(ARCH_NATIVE ON)
endif ()

set(VCPKG_BUILD_TYPE "Release")
if (NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    find_package(Git REQUIRED)
    execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive -- vcpkg WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
    set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake"
            CACHE STRING "Vcpkg toolchain file")
endif ()

# project name
project(bcos-c-sdk VERSION "3.6.0")

# Debug, Release, RelWithDebInfo, MinSizeRel
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE MinSizeRel)
endif ()

# basic settings
include(Options)
configure_project()
include(CompilerSettings)
include(BuildInfoGenerator)
include(InstallConfig)

# install the include files for hash
include(InstallInclude)
include(IncludeDirectories)
include(GNUInstallDirs)

find_package(Boost COMPONENTS log_setup log serialization chrono system filesystem thread iostreams unit_test_framework)
find_package(OpenSSL REQUIRED)
find_package(Microsoft.GSL CONFIG REQUIRED)
find_package(fmt REQUIRED)
find_package(TBB REQUIRED)
find_package(unofficial-secp256k1 CONFIG REQUIRED)
find_package(jsoncpp CONFIG REQUIRED)
find_package(wedprcrypto REQUIRED)
find_package(HSM-CRYPTO REQUIRED)
find_package(fiscobcos REQUIRED)

file(GLOB_RECURSE SOURCES "bcos-c-sdk/*.h" "bcos-c-sdk/*.cpp" "bcos-c-sdk/*.c")

# rust static library linking requirements for macos
if (APPLE)
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -framework Security -framework Foundation")
else ()
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--no-as-needed -ldl")
endif ()

# static lib
set(BCOS_C_SDK_STATIC_TARGET "bcos-c-sdk-static")

# dynamic lib
set(BCOS_C_SDK_TARGET "bcos-c-sdk")

# add suffix for arm
if (CMAKE_HOST_SYSTEM_PROCESSOR STREQUAL "arm64" OR CMAKE_HOST_SYSTEM_PROCESSOR STREQUAL "aarch64")
    message(STATUS "Building arm architecture, CMAKE_HOST_SYSTEM_PROCESSOR => ${CMAKE_HOST_SYSTEM_PROCESSOR}")
    set(BCOS_C_SDK_TARGET "bcos-c-sdk-aarch64")
    set(BCOS_C_SDK_STATIC_TARGET "bcos-c-sdk-static-aarch64")
endif ()

add_library(${BCOS_C_SDK_STATIC_TARGET} ${SOURCES})
target_include_directories(${BCOS_C_SDK_STATIC_TARGET} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}> $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>)
target_link_libraries(${BCOS_C_SDK_STATIC_TARGET} PUBLIC fiscobcos::bcos-cpp-sdk)

add_library(${BCOS_C_SDK_TARGET} SHARED ${SOURCES})
target_include_directories(${BCOS_C_SDK_TARGET} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}> $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>)
target_link_libraries(${BCOS_C_SDK_TARGET} PUBLIC fiscobcos::bcos-cpp-sdk)

add_dependencies(${BCOS_C_SDK_STATIC_TARGET} BuildInfo.h)
add_dependencies(${BCOS_C_SDK_TARGET} BuildInfo.h)

# build sample exec
if (BUILD_SAMPLE)
    add_subdirectory(sample)
endif ()

# build sample exec
if (BUILD_JNI)
    # find JNI
    find_package(JNI REQUIRED)
    include_directories(${JNI_INCLUDE_DIRS})
    add_subdirectory(${CMAKE_SOURCE_DIR}/bindings/java/jni/src/main/c)
endif ()

# ut and binary exe
if (TESTS)
    enable_testing()
    set(CTEST_OUTPUT_ON_FAILURE TRUE)
    add_subdirectory(test)
endif ()

# for code coverage
if (COVERAGE)
    include(Coverage)
    config_coverage("cov" "'/usr*' '${CMAKE_CURRENT_SOURCE_DIR}/bcos-cmake-scripts*' '${CMAKE_CURRENT_SOURCE_DIR}/test/Main*'")
endif ()

# SET(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib)
install(
        TARGETS ${BCOS_C_SDK_TARGET}
        EXPORT "${TARGETS_EXPORT_NAME}"
        LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
        INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)

install(
        TARGETS ${BCOS_C_SDK_STATIC_TARGET}
        EXPORT "${TARGETS_EXPORT_NAME}"
        LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
        INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)
