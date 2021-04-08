cmake_minimum_required(VERSION 3.19)

project(rustpp_tests)

include(FetchContent)
FetchContent_Declare(pwropt		GIT_REPOSITORY https://github.com/BetaJester/pwropt.git		GIT_TAG main)
FetchContent_Declare(fmt        GIT_REPOSITORY https://github.com/fmtlib/fmt.git			GIT_TAG master)
FetchContent_Declare(Catch2		GIT_REPOSITORY https://github.com/catchorg/Catch2.git		GIT_TAG v2.x)
FetchContent_MakeAvailable(pwropt fmt Catch2)
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${Catch2_SOURCE_DIR}/contrib")
include(Catch)
include(CTest)

add_executable(rustpp_tests "main.cpp" "types.cpp" "Result.cpp" )
pwropt_target_compile_options(rustpp_tests PUBLIC pwr_lefticus pwr_werror)
pwropt_target_cxx_latest(rustpp_tests PUBLIC)
target_link_libraries(rustpp_tests PUBLIC rustpp fmt::fmt Catch2)

catch_discover_tests(rustpp_tests)