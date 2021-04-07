cmake_minimum_required(VERSION 3.19)

project(rustpp_example)

include(FetchContent)
FetchContent_Declare(pwropt		GIT_REPOSITORY https://github.com/BetaJester/pwropt.git		GIT_TAG main)
FetchContent_Declare(fmt        GIT_REPOSITORY https://github.com/fmtlib/fmt.git			GIT_TAG master)
FetchContent_MakeAvailable(pwropt fmt)

add_executable(rustpp_example "main.cpp")
pwropt_target_compile_options(rustpp_example PUBLIC pwr_lefticus pwr_werror)
pwropt_target_cxx_latest(rustpp_example PUBLIC)
target_link_libraries(rustpp_example PUBLIC rustpp fmt)