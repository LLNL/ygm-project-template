# Copyright 2019-2024 Lawrence Livermore National Security, LLC and other YGM
# Project Developers. See the top-level COPYRIGHT file for details.
#
# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.14)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

project(YGM-Application
        VERSION 0.9
        DESCRIPTION "YGM Application"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

option(USE_SALTATLAS OFF)
option(USE_KROWKEE OFF)



#
#  YGM
set(YGM_REQUIRE_PARQUET ON)
set(YGM_INSTALL_PARQUET ON)
FetchContent_Declare(YGM
    GIT_REPOSITORY https://github.com/llnl/ygm.git
    GIT_TAG v0.9
        )
FetchContent_MakeAvailable(YGM)

#
#  SaltAtlas
if (USE_SALTATLAS)
  find_package(SALTATLAS QUIET)
  if (NOT SALTATLAS_FOUND)
    set(JUST_INSTALL_SALTATLAS TRUE)
    set(SALTATLAS_USE_HDF5 FALSE)
    FetchContent_Declare(SALTATLAS
      GIT_REPOSITORY https://github.com/llnl/saltatlas.git
      GIT_TAG v0.3-dev
      )
    FetchContent_MakeAvailable(SALTATLAS)
  endif ()
endif ()

#
# Krowkee
if (USE_KROWKEE)
  find_package(KROWKEE QUIET)
  if (NOT KROWKEE_FOUND)
    set(JUST_INSTALL_KROWKEE TRUE)
    FetchContent_Declare(KROWKEE
      GIT_REPOSITORY https://github.com/llnl/krowkee.git
      GIT_TAG develop
      )
    FetchContent_MakeAvailable(KROWKEE)
  endif ()
endif ()

#
# Generate compile_commands.json
#
if (NOT CMAKE_EXPORT_COMPILE_COMMANDS)
  set(CMAKE_EXPORT_COMPILE_COMMANDS On)
  message(STATUS "Setting CMAKE_EXPORT_COMPILE_COMMANDS to On")
endif ()

#
# Default to a Release build
#
if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
  message(STATUS "CMAKE_BUILD_TYPE is set as Release")
endif ()

#
#  Common functions for all YGM Executables
#
function(add_common_compile_options name)
    # Common
    #target_compile_options(${name} PRIVATE -Wall -Wextra -pedantic)

    # Debug
    target_compile_options(${name} PRIVATE $<$<CONFIG:Debug>:-O0>)
    target_compile_options(${name} PRIVATE $<$<CONFIG:Debug>:-g3>)
#    if (Linux)
#        target_compile_options(${name} PRIVATE $<$<CONFIG:Debug>:-pg>)
#    endif ()

    # Release
    target_compile_options(${name} PRIVATE $<$<CONFIG:Release>:-Ofast>)
    target_compile_options(${name} PRIVATE $<$<CONFIG:Release>:-DNDEBUG>)

    # Release with debug info
    target_compile_options(${name} PRIVATE $<$<CONFIG:RelWithDebInfo>:-Ofast>)
    target_compile_options(${name} PRIVATE $<$<CONFIG:RelWithDebInfo>:-g3>)
#    if (Linux)
#        target_compile_options(${name} PRIVATE $<$<CONFIG:RelWithDebInfo>:-pg>)
#    endif ()
endfunction()

#
# Function for setting up a target that uses YGM
#
function(setup_ygm_target exe_name)
    if (${CMAKE_HOST_SYSTEM_NAME} MATCHES "Linux")
        target_link_libraries(${exe_name} PRIVATE rt)
    endif ()
    target_link_libraries(${exe_name} PRIVATE ygm::ygm)
    if (USE_SALTATLAS)
      target_link_libraries(${exe_name} PRIVATE saltatlas)
    endif ()
    if (USE_KROWKEE)
      target_link_libraries(${exe_name} PRIVATE krowkee)
    endif ()
endfunction()

function(add_ygm_executable name)
    add_executable(${name} ${name}.cpp)
    add_common_compile_options(${name})
    target_include_directories(${name} PRIVATE ${PROJECT_SOURCE_DIR}/include ${BOOST_INCLUDE_DIRS})
    target_link_libraries(${name} PRIVATE Boost::json)
    setup_ygm_target(${name})
endfunction()

add_subdirectory(src)

#
# Copy the testing data directory
#
file(COPY data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
