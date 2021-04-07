cmake_minimum_required(VERSION 3.19)

project(rustpp)

option(RUSTPP_BUILD_EXAMPLE "Build the rustpp example" OFF)

include(FetchContent)
FetchContent_Declare(pwropt		GIT_REPOSITORY https://github.com/BetaJester/pwropt.git		GIT_TAG main)
FetchContent_MakeAvailable(pwropt)

add_library(rustpp INTERFACE "include/rustpp/rustpp.hpp")
pwropt_target_compile_options(rustpp INTERFACE pwr_lefticus pwr_werror)
pwropt_target_cxx_latest(rustpp INTERFACE)
target_include_directories(rustpp INTERFACE "include")

if(${RUSTPP_BUILD_EXAMPLE})
	add_subdirectory("example")
endif()