# Copyright 2019-2024 Lawrence Livermore National Security, LLC and other YGM
# Project Developers. See the top-level COPYRIGHT file for details.
#
# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.14)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

project(YGM-Application
        VERSION 0.1
        DESCRIPTION "YGM Appliction"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

option(USE_SALTATLAS OFF)
option(USE_KROWKEE OFF)

#
#  Threads
#
find_package(Threads REQUIRED)


#
# Boost
include(setup_boost)
prepare_fetchcontent_boost()
set(FETCHCONTENT_QUIET FALSE)
FetchContent_Declare(
        Boost
        GIT_REPOSITORY https://github.com/boostorg/boost.git
        GIT_TAG boost-1.78.0
        GIT_SUBMODULES ${BOOST_REQD_SUBMODULES}
        GIT_PROGRESS TRUE
        CONFIGURE_COMMAND ""  # tell CMake it's not a cmake project
)
FetchContent_MakeAvailable(Boost)
get_boost_include_dirs()

#
#  MPI
find_package(MPI)

#
#  YGM
if (MPI_CXX_FOUND)
    find_package(YGM QUIET)
    if (NOT YGM_FOUND)
        set(JUST_INSTALL_YGM TRUE)
        FetchContent_Declare(YGM
            GIT_REPOSITORY https://github.com/llnl/ygm.git
            GIT_TAG v0.7-dev
                )
        FetchContent_MakeAvailable(YGM)
    endif ()
endif ()

#
#  SaltAtlas
if (USE_SALTATLAS)
  if (MPI_CXX_FOUND)
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
endif ()

#
# Krowkee
if (USE_KROWKEE)
  if (MPI_CXX_FOUND)
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
    if (NOT MPI_CXX_FOUND)
        message(WARNING "YGM requires MPI; skip building ${exe_name}.")
        return()
    endif ()

    target_link_libraries(${exe_name} PRIVATE MPI::MPI_CXX)
    target_link_libraries(${exe_name} PRIVATE cereal)
    target_link_libraries(${exe_name} PRIVATE Threads::Threads)
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
