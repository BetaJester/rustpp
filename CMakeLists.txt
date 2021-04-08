cmake_minimum_required(VERSION 3.19)

project(rustpp)

option(RUSTPP_BUILD_EXAMPLES "Build the rustpp examples" OFF)
option(RUSTPP_BUILD_TESTS "Build the rustpp tests" OFF)

include(FetchContent)
FetchContent_Declare(pwropt		GIT_REPOSITORY https://github.com/BetaJester/pwropt.git		GIT_TAG main)
FetchContent_MakeAvailable(pwropt)

add_library(rustpp INTERFACE "include/rustpp/rustpp.hpp" "include/rustpp/Result.hpp"  "include/rustpp/match.hpp" "include/rustpp/types.hpp" "include/rustpp/config.hpp")
pwropt_target_compile_options(rustpp INTERFACE pwr_lefticus pwr_werror)
pwropt_target_cxx_latest(rustpp INTERFACE)
target_include_directories(rustpp INTERFACE "include")

if(${RUSTPP_BUILD_EXAMPLES})
	add_subdirectory("examples")
endif()

if(${RUSTPP_BUILD_TESTS})
	enable_testing()
	add_subdirectory("tests")
endif()