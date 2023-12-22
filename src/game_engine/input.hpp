#pragma once

#include "raylib.h"
#include <map>

// namespace cevy {
//     class Keyboard {
//         private:
//             /* data */
//         public:
//         static bool keyPressed(int key) {return (IsKeyPressed(key));};
//         static bool keyReleased(int key) {return (IsKeyPressed(key));};
//         static bool keyDown(int key) {return (IsKeyDown(key));};
//     };
// }

template<typename T>
class Input {
    public:
        enum ButtonState {
            JUST_PRESSED = 0,
            PRESSED = 1,
            JUST_RELEASED = 2,
        };
        typedef enum {
            KEY_NULL            = 0,        // Key: NULL, used for no key pressed
            // Alphanumeric keys
            KEY_APOSTROPHE      = 39,       // Key: '
            KEY_COMMA           = 44,       // Key: ,
            KEY_MINUS           = 45,       // Key: -
            KEY_PERIOD          = 46,       // Key: .
            KEY_SLASH           = 47,       // Key: /
            KEY_ZERO            = 48,       // Key: 0
            KEY_ONE             = 49,       // Key: 1
            KEY_TWO             = 50,       // Key: 2
            KEY_THREE           = 51,       // Key: 3
            KEY_FOUR            = 52,       // Key: 4
            KEY_FIVE            = 53,       // Key: 5
            KEY_SIX             = 54,       // Key: 6
            KEY_SEVEN           = 55,       // Key: 7
            KEY_EIGHT           = 56,       // Key: 8
            KEY_NINE            = 57,       // Key: 9
            KEY_SEMICOLON       = 59,       // Key: ;
            KEY_EQUAL           = 61,       // Key: =
            KEY_A               = 65,       // Key: A | a
            KEY_B               = 66,       // Key: B | b
            KEY_C               = 67,       // Key: C | c
            KEY_D               = 68,       // Key: D | d
            KEY_E               = 69,       // Key: E | e
            KEY_F               = 70,       // Key: F | f
            KEY_G               = 71,       // Key: G | g
            KEY_H               = 72,       // Key: H | h
            KEY_I               = 73,       // Key: I | i
            KEY_J               = 74,       // Key: J | j
            KEY_K               = 75,       // Key: K | k
            KEY_L               = 76,       // Key: L | l
            KEY_M               = 77,       // Key: M | m
            KEY_N               = 78,       // Key: N | n
            KEY_O               = 79,       // Key: O | o
            KEY_P               = 80,       // Key: P | p
            KEY_Q               = 81,       // Key: Q | q
            KEY_R               = 82,       // Key: R | r
            KEY_S               = 83,       // Key: S | s
            KEY_T               = 84,       // Key: T | t
            KEY_U               = 85,       // Key: U | u
            KEY_V               = 86,       // Key: V | v
            KEY_W               = 87,       // Key: W | w
            KEY_X               = 88,       // Key: X | x
            KEY_Y               = 89,       // Key: Y | y
            KEY_Z               = 90,       // Key: Z | z
            KEY_LEFT_BRACKET    = 91,       // Key: [
            KEY_BACKSLASH       = 92,       // Key: '\'
            KEY_RIGHT_BRACKET   = 93,       // Key: ]
            KEY_GRAVE           = 96,       // Key: `
            // Function keys
            KEY_SPACE           = 32,       // Key: Space
            KEY_ESCAPE          = 256,      // Key: Esc
            KEY_ENTER           = 257,      // Key: Enter
            KEY_TAB             = 258,      // Key: Tab
            KEY_BACKSPACE       = 259,      // Key: Backspace
            KEY_INSERT          = 260,      // Key: Ins
            KEY_DELETE          = 261,      // Key: Del
            KEY_RIGHT           = 262,      // Key: Cursor right
            KEY_LEFT            = 263,      // Key: Cursor left
            KEY_DOWN            = 264,      // Key: Cursor down
            KEY_UP              = 265,      // Key: Cursor up
            KEY_PAGE_UP         = 266,      // Key: Page up
            KEY_PAGE_DOWN       = 267,      // Key: Page down
            KEY_HOME            = 268,      // Key: Home
            KEY_END             = 269,      // Key: End
            KEY_CAPS_LOCK       = 280,      // Key: Caps lock
            KEY_SCROLL_LOCK     = 281,      // Key: Scroll down
            KEY_NUM_LOCK        = 282,      // Key: Num lock
            KEY_PRINT_SCREEN    = 283,      // Key: Print screen
            KEY_PAUSE           = 284,      // Key: Pause
            KEY_F1              = 290,      // Key: F1
            KEY_F2              = 291,      // Key: F2
            KEY_F3              = 292,      // Key: F3
            KEY_F4              = 293,      // Key: F4
            KEY_F5              = 294,      // Key: F5
            KEY_F6              = 295,      // Key: F6
            KEY_F7              = 296,      // Key: F7
            KEY_F8              = 297,      // Key: F8
            KEY_F9              = 298,      // Key: F9
            KEY_F10             = 299,      // Key: F10
            KEY_F11             = 300,      // Key: F11
            KEY_F12             = 301,      // Key: F12
            KEY_LEFT_SHIFT      = 340,      // Key: Shift left
            KEY_LEFT_CONTROL    = 341,      // Key: Control left
            KEY_LEFT_ALT        = 342,      // Key: Alt left
            KEY_LEFT_SUPER      = 343,      // Key: Super left
            KEY_RIGHT_SHIFT     = 344,      // Key: Shift right
            KEY_RIGHT_CONTROL   = 345,      // Key: Control right
            KEY_RIGHT_ALT       = 346,      // Key: Alt right
            KEY_RIGHT_SUPER     = 347,      // Key: Super right
            KEY_KB_MENU         = 348,      // Key: KB menu
            // Keypad keys
            KEY_KP_0            = 320,      // Key: Keypad 0
            KEY_KP_1            = 321,      // Key: Keypad 1
            KEY_KP_2            = 322,      // Key: Keypad 2
            KEY_KP_3            = 323,      // Key: Keypad 3
            KEY_KP_4            = 324,      // Key: Keypad 4
            KEY_KP_5            = 325,      // Key: Keypad 5
            KEY_KP_6            = 326,      // Key: Keypad 6
            KEY_KP_7            = 327,      // Key: Keypad 7
            KEY_KP_8            = 328,      // Key: Keypad 8
            KEY_KP_9            = 329,      // Key: Keypad 9
            KEY_KP_DECIMAL      = 330,      // Key: Keypad .
            KEY_KP_DIVIDE       = 331,      // Key: Keypad /
            KEY_KP_MULTIPLY     = 332,      // Key: Keypad *
            KEY_KP_SUBTRACT     = 333,      // Key: Keypad -
            KEY_KP_ADD          = 334,      // Key: Keypad +
            KEY_KP_ENTER        = 335,      // Key: Keypad Enter
            KEY_KP_EQUAL        = 336,      // Key: Keypad =
            // Android key buttons
            KEY_BACK            = 4,        // Key: Android back button
            KEY_MENU            = 82,       // Key: Android menu button
            KEY_VOLUME_UP       = 24,       // Key: Android volume up button
            KEY_VOLUME_DOWN     = 25        // Key: Android volume down button
        } KeyboardKey;
        struct KeyBoardInput {
            unsigned int scancode;
            Input::KeyboardKey keycode;
            Input::ButtonState state;
        };
        void update_keys();
        std::map<int, T> just_pressed;
        std::map<int, T> pressed;
        std::map<int, T> just_released;
};

void Input::update_keys() {
    Input::keyCode keyval = GetKeyPressed();
    for (auto const& [key, val] : just_pressed) {
        if (pressed.find(key) != pressed.end()) {
            pressed.insert(val);
        }
    }
    just_pressed.clear();
    while (keyval != Input::KeyCode::KEY_NULL) {
        just_pressed.insert({
            keyval,
            (KeyBoardInput){
                keyval, // to change to scancode
                keyval,
                ButtonState::just_pressed,
            };
        }
        )
        key = getKeyPressed();
    }
    just_released.clear();
    keyval = getKeyReleased();
    while (keyval != Input::KeyCode::KEY_NULL) {
        just_released.insert(pressed.find(keyval));
        pressed.erase(key);
        key = getKeyReleased();
    }
};

// class InputButton {
//     public:
//         enum ButtonState {
//             RELEASED = 0,
//             JUST_PRESSED = 1,
//             PRESSED = 2,
//             JUST_RELEASED = 3,
//         };
//         InputButton() {};
//         ~InputButton() {};
//         void update() {};
//     private:
// };

// class KeyBoardInput : public InputButton {
//     public:
//         enum KeyCode {
//         KEY_NULL            = 0,        // Key: NULL, used for no key pressed
//         KEY_A               = 1,       // Key: A | a
//         KEY_B               = 2,       // Key: B | b
//         KEY_C               = 3,       // Key: C | c
//         KEY_D               = 4,       // Key: D | d
//         KEY_E               = 5,       // Key: E | e
//         KEY_F               = 6,       // Key: F | f
//         KEY_G               = 7,       // Key: G | g
//         KEY_H               = 8,       // Key: H | h
//         KEY_I               = 9,       // Key: I | i
//         KEY_J               = 10,       // Key: J | j
//         KEY_K               = 11,       // Key: K | k
//         KEY_L               = 12,       // Key: L | l
//         KEY_M               = 13,       // Key: M | m
//         KEY_N               = 14,       // Key: N | n
//         KEY_O               = 15,       // Key: O | o
//         KEY_P               = 16,       // Key: P | p
//         KEY_Q               = 17,       // Key: Q | q
//         KEY_R               = 18,       // Key: R | r
//         KEY_S               = 19,       // Key: S | s
//         KEY_T               = 20,       // Key: T | t
//         KEY_U               = 21,       // Key: U | u
//         KEY_V               = 22,       // Key: V | v
//         KEY_W               = 23,       // Key: W | w
//         KEY_X               = 24,       // Key: X | x
//         KEY_Y               = 25,       // Key: Y | y
//         KEY_Z               = 26,       // Key: Z | z
//         KEY_APOSTROPHE      = 27,       // Key: '
//         KEY_COMMA           = 28,       // Key: ,
//         KEY_MINUS           = 29,       // Key: -
//         KEY_PERIOD          = 30,       // Key: .
//         KEY_SLASH           = 31,       // Key: /
//         KEY_ZERO            = 32,       // Key: 0
//         KEY_ONE             = 33,       // Key: 1
//         KEY_TWO             = 34,       // Key: 2
//         KEY_THREE           = 35,       // Key: 3
//         KEY_FOUR            = 36,       // Key: 4
//         KEY_FIVE            = 37,       // Key: 5
//         KEY_SIX             = 38,       // Key: 6
//         KEY_SEVEN           = 39,       // Key: 7
//         KEY_EIGHT           = 40,       // Key: 8
//         KEY_NINE            = 41,       // Key: 9
//         KEY_SEMICOLON       = 42,       // Key: ;
//         KEY_EQUAL           = 43,       // Key: =
//         KEY_LEFT_BRACKET    = 44,       // Key: [
//         KEY_BACKSLASH       = 45,       // Key: '\'
//         KEY_RIGHT_BRACKET   = 46,       // Key: ]
//         KEY_GRAVE           = 47,       // Key: `
//         KEY_SPACE           = 48,       // Key: Space
//         KEY_ESCAPE          = 49,      // Key: Esc
//         KEY_ENTER           = 50,      // Key: Enter
//         KEY_TAB             = 51,      // Key: Tab
//         KEY_BACKSPACE       = 52,      // Key: Backspace
//         KEY_INSERT          = 53,      // Key: Ins
//         KEY_DELETE          = 54,      // Key: Del
//         KEY_RIGHT           = 55,      // Key: Cursor right
//         KEY_LEFT            = 56,      // Key: Cursor left
//         KEY_DOWN            = 57,      // Key: Cursor down
//         KEY_UP              = 58,      // Key: Cursor up
//         KEY_PAGE_UP         = 59,      // Key: Page up
//         KEY_PAGE_DOWN       = 60,      // Key: Page down
//         KEY_HOME            = 61,      // Key: Home
//         KEY_END             = 62,      // Key: End
//         KEY_CAPS_LOCK       = 63,      // Key: Caps lock
//         KEY_SCROLL_LOCK     = 64,      // Key: Scroll down
//         KEY_NUM_LOCK        = 65,      // Key: Num lock
//         KEY_PRINT_SCREEN    = 66,      // Key: Print screen
//         KEY_PAUSE           = 67,      // Key: Pause
//         KEY_F1              = 68,      // Key: F1
//         KEY_F2              = 69,      // Key: F2
//         KEY_F3              = 70,      // Key: F3
//         KEY_F4              = 71,      // Key: F4
//         KEY_F5              = 72,      // Key: F5
//         KEY_F6              = 73,      // Key: F6
//         KEY_F7              = 74,      // Key: F7
//         KEY_F8              = 75,      // Key: F8
//         KEY_F9              = 76,      // Key: F9
//         KEY_F10             = 77,      // Key: F10
//         KEY_F11             = 78,      // Key: F11
//         KEY_F12             = 79,      // Key: F12
//         KEY_LEFT_SHIFT      = 80,      // Key: Shift left
//         KEY_LEFT_CONTROL    = 81,      // Key: Control left
//         KEY_LEFT_ALT        = 82,      // Key: Alt left
//         KEY_LEFT_SUPER      = 83,      // Key: Super left
//         KEY_RIGHT_SHIFT     = 84,      // Key: Shift right
//         KEY_RIGHT_CONTROL   = 85,      // Key: Control right
//         KEY_RIGHT_ALT       = 86,      // Key: Alt right
//         KEY_RIGHT_SUPER     = 87,      // Key: Super right
//         KEY_KB_MENU         = 88,      // Key: KB menu
//         KEY_KP_0            = 89,      // Key: Keypad 0
//         KEY_KP_1            = 90,      // Key: Keypad 1
//         KEY_KP_2            = 91,      // Key: Keypad 2
//         KEY_KP_3            = 92,      // Key: Keypad 3
//         KEY_KP_4            = 93,      // Key: Keypad 4
//         KEY_KP_5            = 94,      // Key: Keypad 5
//         KEY_KP_6            = 95,      // Key: Keypad 6
//         KEY_KP_7            = 96,      // Key: Keypad 7
//         KEY_KP_8            = 97,      // Key: Keypad 8
//         KEY_KP_9            = 98,      // Key: Keypad 9
//         KEY_KP_DECIMAL      = 99,      // Key: Keypad .
//         KEY_KP_DIVIDE       = 100,      // Key: Keypad /
//         KEY_KP_MULTIPLY     = 101,      // Key: Keypad *
//         KEY_KP_SUBTRACT     = 102,      // Key: Keypad -
//         KEY_KP_ADD          = 103,      // Key: Keypad +
//         KEY_KP_ENTER        = 104,      // Key: Keypad Enter
//         KEY_KP_EQUAL        = 105,      // Key: Keypad =
//     };
//         KeyBoardInput(KeyCode Keycode): key(Keycode), InputButton() {};
//         ~KeyBoardInput() {};
//         KeyCode key;
//         ButtonState state = ButtonState::RELEASED;
// };

// void KeyBoardInput::update() {
//     keyCode keycode = KEY_NULL;
//     KeyCode = GetKeyPressed();

//     for (auto key : just_pressed) {
//         keys.
//     }
//     while (keycode != KEY_NULL) {
//         KeyCode = GetKeyPressed();
//     }
// }
// enum MouseButton {
//     LEFT_CLICK = 0,
//     RIGHT_CLICK = 1,
//     MIDDLE_CLICK = 2,
// }
// struct MouseButtonInput {
//     MouseButton button;
//     ButtonState state;
// }

// struct MousePosition {
//     Vector2 position;
//     Vector2 delta;
// }

// struct MouseWheel {
//     double vectical;
//     double horisontal;
// }

// struct MouseInput {
//     MouseButtonInput buttons;
//     MousePosition position;
//     MouseWheel wheel;
// }
