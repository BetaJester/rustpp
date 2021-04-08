#include <catch2/catch.hpp>
#include <rustpp/rustpp.hpp>

using namespace rustpp;

auto gives_error() -> Result<u32, String> {

    return Err<String>{ "Something went wrong" };

}

auto gives_ok() -> Result<u32, String> {

    return 128u;

}

auto closure_errlen(String s) -> u32 {
    return static_cast<u32>(s.size()); // 20.
}


auto unhandled() {

    gives_error();

}

auto matchy() {

    match{ gives_ok() } (
        [](Ok<u32> &) {},
        [](Err<String> &) {}
    );

}

TEST_CASE("Result ok state is handled", "[result,okresult]") {

    auto res = gives_ok();

    REQUIRE(res.is_ok());
    REQUIRE_FALSE(res.is_err());

    REQUIRE(res.unwrap_or(9) == 128u);

    REQUIRE(res.unwrap_or_else(closure_errlen) == 128u);

    REQUIRE_NOTHROW(res.expect("whoopsie daisy") == 128u);

    REQUIRE_NOTHROW(res.unwrap() == 128u);

}

TEST_CASE("Result err state is handled", "[result,errresult]") {

    auto res = gives_error();

    REQUIRE_FALSE(res.is_ok());
    REQUIRE(res.is_err());

    REQUIRE(res.unwrap_or(9) == 9);

    REQUIRE(res.unwrap_or_else(closure_errlen) == 20);

    REQUIRE_THROWS(res.expect("whoopsie daisy"));

    REQUIRE_THROWS(res.unwrap());

}

TEST_CASE("Result unhandled is unhandled", "[result,unhandled]") {

    REQUIRE_THROWS(unhandled());

}


TEST_CASE("Result match his handled", "[result,match]") {

    REQUIRE_NOTHROW(matchy());

}