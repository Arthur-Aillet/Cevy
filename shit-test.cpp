#include "shit-test.hpp"
#include <iostream>

struct Menu {};
struct Playing {};
struct Playing2 {};

// using State = Enum;

template<typename T>
class Member : public Enum2, public T {
public:
    Member() : Enum2(Enum2::create<T>()) {};
    // using Args...;
    // (using Args() : Enum2(Enum2::create<Args>()) {};)...
};

template<typename... Args>
class Enum3 : public Args... {
    // using Member<Args>...;
};

using State3 = Enum3<Menu, Playing>;

// enum class STATES {
//     EPlaying,
//     EMenu,
// };

using State = size_t;

template<typename T>
struct DEFAULT_STATES {
    ENUM_FROM(T, EQuitting);
    ENUM_FROM(T, EBooting);
};

struct USER_STATES {
    ENUM_FROM(size_t, EPlaying);
    ENUM_FROM(size_t, EMenu);
};

using State4 = Enum3<DEFAULT_STATES<State>, USER_STATES>;

int main()
{
    // {
    //     State state = State::ctor<Playing>();
    //     std::cout << char(state.dbg()) << std::endl;
    //     std::cout << state.dbg() << std::endl;
    //     State state2 = State::ctor<Menu>();
    //     std::cout << state2.dbg() << std::endl;
    // }
    {
        // State2 state;
        Enum3 state;
        Member<State3::Menu> t;

        State4::EPlaying;

        std::cout << t.dbg() << std::endl;

        auto s = Member<Playing>();
    }
}
