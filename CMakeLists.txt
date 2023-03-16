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

if(NOT MSVC)
    set(CMAKE_CXX_FLAGS "-pthread -fPIC -fvisibility=hidden -fvisibility-inlines-hidden -fexceptions")
endif()

set(CMAKE_OSX_DEPLOYMENT_TARGET "12" CACHE STRING "Minimum OS X deployment version")

# export windows dll symbol
if(WIN32)
    message(STATUS "Compile on Windows")
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS "ON")
endif()

# URL_BASE download domain name
include(FetchContent)

if(NOT DEFINED URL_BASE)
    set(URL_BASE "github.com")

    # set(URL_BASE "github.com.cnpmjs.org")
endif()

# include(FetchContent)
# set(BCOS_CMAKE_SCRIPTS_DIR ${CMAKE_CURRENT_BINARY_DIR}/bcos-cmake-scripts)
# FetchContent_Declare(bcos-cmake-scripts
# GIT_REPOSITORY https://${URL_BASE}/FISCO-BCOS/bcos-cmake-scripts.git
# GIT_TAG 364f603f4ec112cb79840ea4026243399d7c419b
# SOURCE_DIR ${BCOS_CMAKE_SCRIPTS_DIR}
# )
# FetchContent_MakeAvailable(bcos-cmake-scripts)
set(JAVA_SDK_JNI_CMAKE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/cmake" CACHE PATH "The cmake path for the library")

# list(APPEND CMAKE_MODULE_PATH ${BCOS_CMAKE_SCRIPTS_DIR})
list(APPEND CMAKE_MODULE_PATH ${JAVA_SDK_JNI_CMAKE_DIR})

# include hunter
include(Options)
set(ARCH_NATIVE OFF)

if("${ARCHITECTURE}" MATCHES "aarch64")
    set(ARCH_NATIVE ON)
endif()

include(HunterGate)
HunterGate(
    URL "https://${URL_BASE}/FISCO-BCOS/hunter/archive/2b5b25ecffaba74393740bf0b8758c4e8d9297cf.tar.gz"
    SHA1 "5d8365a327be260cf24d2b125a71ee7069aca28f"
    FILEPATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/config.cmake"
)

# project name
project(bcos-c-sdk VERSION "3.3.0")

# Debug, Release, RelWithDebInfo, MinSizeRel
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

include_directories(${CMAKE_INSTALL_INCLUDEDIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include(InstallDependencies)

# basic settings
include(Options)
configure_project()
include(CompilerSettings)
include(BuildInfoGenerator)
include(InstallConfig)

# install the include files for hash
include(InstallInclude)

file(GLOB_RECURSE SOURCES "bcos-c-sdk/*.h" "bcos-c-sdk/*.cpp" "bcos-c-sdk/*.c")

# rust static library linking requirements for macos
if(APPLE)
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -framework Security -framework Foundation")
else()
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--no-as-needed -ldl")
endif()

# static lib
set(BCOS_C_SDK_STATIC_TARGET "bcos-c-sdk-static")

# dynamic lib
set(BCOS_C_SDK_TARGET "bcos-c-sdk")

# add suffix for arm
if(CMAKE_HOST_SYSTEM_PROCESSOR STREQUAL "arm64" OR CMAKE_HOST_SYSTEM_PROCESSOR STREQUAL "aarch64")
    message(STATUS "Building arm architecture, CMAKE_HOST_SYSTEM_PROCESSOR => ${CMAKE_HOST_SYSTEM_PROCESSOR}")
    set(BCOS_C_SDK_TARGET "bcos-c-sdk-aarch64")
    set(BCOS_C_SDK_STATIC_TARGET "bcos-c-sdk-static-aarch64")
endif()

add_library(${BCOS_C_SDK_STATIC_TARGET} ${SOURCES})
target_link_libraries(${BCOS_C_SDK_STATIC_TARGET} PUBLIC bcos-cpp-sdk::bcos-cpp-sdk)

add_library(${BCOS_C_SDK_TARGET} SHARED ${SOURCES})
target_link_libraries(${BCOS_C_SDK_TARGET} PUBLIC bcos-cpp-sdk::bcos-cpp-sdk)

add_dependencies(${BCOS_C_SDK_STATIC_TARGET} BuildInfo.h)
add_dependencies(${BCOS_C_SDK_TARGET} BuildInfo.h)

# build sample exec
if(BUILD_SAMPLE)
    add_subdirectory(sample)
endif()

# build sample exec
if(BUILD_JNI)
    # find JNI
    find_package(JNI REQUIRED)
    include_directories(${JNI_INCLUDE_DIRS})
    add_subdirectory(bindings/java/jni/src/main/c)
endif()

# ut and binary exe
# if (TESTS)
# enable_testing()
# set(CTEST_OUTPUT_ON_FAILURE TRUE)
# add_subdirectory(test)
# endif()

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
