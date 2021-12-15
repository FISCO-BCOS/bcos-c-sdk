#------------------------------------------------------------------------------
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
#------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.16)
set(CMAKE_OSX_DEPLOYMENT_TARGET "11.3" CACHE STRING "Minimum OS X deployment version")

# URL_BASE download domain name
include(FetchContent)
if (NOT DEFINED URL_BASE)
    # set(URL_BASE "github.com")
    set(URL_BASE "github.com.cnpmjs.org")
endif()

# -fPIC
set(ENV{CMAKE_C_FLAGS} "${CMAKE_C_FLAGS} -fPIC")
set(ENV{CMAKE_CXX_FLAGS} "${CMAKE_CXX_FLAGS} -fPIC")

set(JAVA_SDK_JNI_CMAKE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/cmake" CACHE PATH "The cmake path for the library")
# list(APPEND CMAKE_MODULE_PATH ${BCOS_CMAKE_SCRIPTS_DIR})
list(APPEND CMAKE_MODULE_PATH ${JAVA_SDK_JNI_CMAKE_DIR})

# include hunter
include(HunterGate)
HunterGate(
    URL https://${URL_BASE}/FISCO-BCOS/hunter/archive/858de38e7c4c654499f36171feb1057f59e88b71.tar.gz
    SHA1 19eeb6de3c56397e7ed6b45bbc57f2b4000e5b97
    FILEPATH "${CMAKE_SOURCE_DIR}/cmake/config.cmake"
)

# project name
project(bcos-c-sdk VERSION "3.0.0")

# Debug, Release, RelWithDebInfo, MinSizeRel
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(${CMAKE_INSTALL_INCLUDEDIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# message(STATUS "CMAKE_INSTALL_INCLUDEDIR ==> ${CMAKE_INSTALL_INCLUDEDIR}")
# message(STATUS "CMAKE_CURRENT_SOURCE_DIR ==> ${CMAKE_CURRENT_SOURCE_DIR}")
# message(STATUS "CMAKE_CURRENT_SOURCE_DIR ==> ${CMAKE_CURRENT_SOURCE_DIR}/include")

hunter_add_package(jsoncpp)
find_package(jsoncpp CONFIG REQUIRED)

# find OpenSSL
hunter_add_package(OpenSSL)
find_package(OpenSSL REQUIRED)

hunter_add_package(Boost COMPONENTS all)
find_package(Boost CONFIG REQUIRED log chrono system filesystem thread iostreams)

# find bcos-boostssl
hunter_add_package(bcos-boostssl)
find_package(bcos-boostssl CONFIG REQUIRED)

# find bcos-cpp-sdk
hunter_add_package(bcos-cpp-sdk)
find_package(bcos-cpp-sdk REQUIRED)

# basic settings
include(Options)
configure_project()
include(CompilerSettings)
include(InstallConfig)
# install the include files for hash
include(InstallInclude)

# install bcos-framework
# include(InstallBcosFrameworkDependencies)

file(GLOB_RECURSE SOURCES "bcos-c-sdk/*.h" "bcos-c-sdk/*.cpp" "bcos-c-sdk/*.c")



# static lib
set(BCOS_C_SDK_TARGET "bcos-c-sdk")
add_library(${BCOS_C_SDK_TARGET} ${SOURCES})
target_link_libraries(${BCOS_C_SDK_TARGET} PUBLIC bcos-cpp-sdk::cpp-sdk)

# dynamic lib
set(BCOS_C_SDK_DY_TARGET "bcos-c-sdk-dy")
add_library(${BCOS_C_SDK_DY_TARGET} SHARED ${SOURCES})
target_link_libraries(${BCOS_C_SDK_DY_TARGET} PUBLIC bcos-cpp-sdk::cpp-sdk)

# build sample exec
if (BUILD_SAMPLE)
    add_subdirectory(sample)
endif()

# ut and binary exe
# if (TESTS)
#    enable_testing()
#    set(CTEST_OUTPUT_ON_FAILURE TRUE)
#    add_subdirectory(test)
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