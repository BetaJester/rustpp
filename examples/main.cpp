#include <rustpp/rustpp.hpp>
#include <fmt/ostream.h>

using namespace rustpp;

auto add(u32 a, u32 b) -> u32 {
    return a + b;
}

auto add(u32 a, u32 b, mut<u32&> out) {
    out = a + b;
}

int main() {

    // Mutable or not, must be initialised.
    u32 a = 12;
    u32 b = 24;

    u32 sum1 = add(a, b);
    fmt::print("Normal add: {} + {} == {}\n", a, b, sum1);

    
    // This fails, as wanted. Type must be mutable.
    // u32 sum2 = 0;
    // add(a, b, sum2);
    
    // This fails, as wanted. Passing must be explicit.
    // mut<u32> sum3 = 0;
    // add(a, b, sum3);

    // Explicit and mutable.
    mut<u32> sum4 = 0;
    add(a, b, mut(sum4));

    fmt::print("Exlicit add: {} + {} == {}\n", a, b, sum4);

}