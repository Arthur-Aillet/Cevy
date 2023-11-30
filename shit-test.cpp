#include "shit-test.hpp"
#include <iostream>

struct Menu {};
struct Playing {};
struct Playing2 {};

using State = Enum;

int main()
{
    {
        State state = State::ctor<Playing>();
        std::cout << char(state.dbg()) << std::endl;
        std::cout << state.dbg() << std::endl;
        State state2 = State::ctor<Menu>();
        std::cout << state2.dbg() << std::endl;
    }
}
