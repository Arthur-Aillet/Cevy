/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** Keyboard
*/

#pragma once

#include "Input.hpp"

struct KeyBoard {
  unsigned int scancode;
  KeyboardKey keycode;
  ButtonState state;
};

typedef enum {
  KEY_NULL = 0, // Key: NULL, used for no key pressed
  // Alphanumeric keys
  KEY_A = 1,              // Key: A | a
  KEY_B = 2,              // Key: B | b
  KEY_C = 3,              // Key: C | c
  KEY_D = 4,              // Key: D | d
  KEY_E = 5,              // Key: E | e
  KEY_F = 6,              // Key: F | f
  KEY_G = 7,              // Key: G | g
  KEY_H = 8,              // Key: H | h
  KEY_I = 9,              // Key: I | i
  KEY_J = 10,             // Key: J | j
  KEY_K = 11,             // Key: K | k
  KEY_L = 12,             // Key: L | l
  KEY_M = 13,             // Key: M | m
  KEY_N = 14,             // Key: N | n
  KEY_O = 15,             // Key: O | o
  KEY_P = 16,             // Key: P | p
  KEY_Q = 17,             // Key: Q | q
  KEY_R = 18,             // Key: R | r
  KEY_S = 19,             // Key: S | s
  KEY_T = 20,             // Key: T | t
  KEY_U = 21,             // Key: U | u
  KEY_V = 22,             // Key: V | v
  KEY_W = 23,             // Key: W | w
  KEY_X = 24,             // Key: X | x
  KEY_Y = 25,             // Key: Y | y
  KEY_Z = 26,             // Key: Z | z
  KEY_ZERO = 27,          // Key: 0
  KEY_ONE = 28,           // Key: 1
  KEY_TWO = 29,           // Key: 2
  KEY_THREE = 30,         // Key: 3
  KEY_FOUR = 31,          // Key: 4
  KEY_FIVE = 32,          // Key: 5
  KEY_SIX = 33,           // Key: 6
  KEY_SEVEN = 34,         // Key: 7
  KEY_EIGHT = 35,         // Key: 8
  KEY_NINE = 36,          // Key: 9
  KEY_APOSTROPHE = 37,    // Key: '
  KEY_COMMA = 38,         // Key: ,
  KEY_MINUS = 39,         // Key: -
  KEY_PERIOD = 40,        // Key: .
  KEY_SLASH = 41,         // Key: /
  KEY_SEMICOLON = 42,     // Key: ;
  KEY_EQUAL = 43,         // Key: =
  KEY_LEFT_BRACKET = 44,  // Key: [
  KEY_BACKSLASH = 45,     // Key: '\'
  KEY_RIGHT_BRACKET = 46, // Key: ]
  KEY_GRAVE = 47,         // Key: `
  // Function keys
  KEY_SPACE = 48,         // Key: Space
  KEY_ESCAPE = 49,        // Key: Esc
  KEY_ENTER = 50,         // Key: Enter
  KEY_TAB = 51,           // Key: Tab
  KEY_BACKSPACE = 52,     // Key: Backspace
  KEY_INSERT = 53,        // Key: Ins
  KEY_DELETE = 54,        // Key: Del
  KEY_RIGHT = 55,         // Key: Cursor right
  KEY_LEFT = 56,          // Key: Cursor left
  KEY_DOWN = 57,          // Key: Cursor down
  KEY_UP = 58,            // Key: Cursor up
  KEY_PAGE_UP = 59,       // Key: Page up
  KEY_PAGE_DOWN = 60,     // Key: Page down
  KEY_HOME = 61,          // Key: Home
  KEY_END = 62,           // Key: End
  KEY_CAPS_LOCK = 63,     // Key: Caps lock
  KEY_SCROLL_LOCK = 64,   // Key: Scroll down
  KEY_NUM_LOCK = 65,      // Key: Num lock
  KEY_PRINT_SCREEN = 66,  // Key: Print screen
  KEY_PAUSE = 67,         // Key: Pause
  KEY_F1 = 68,            // Key: F1
  KEY_F2 = 69,            // Key: F2
  KEY_F3 = 70,            // Key: F3
  KEY_F4 = 71,            // Key: F4
  KEY_F5 = 72,            // Key: F5
  KEY_F6 = 73,            // Key: F6
  KEY_F7 = 74,            // Key: F7
  KEY_F8 = 75,            // Key: F8
  KEY_F9 = 76,            // Key: F9
  KEY_F10 = 77,           // Key: F10
  KEY_F11 = 78,           // Key: F11
  KEY_F12 = 79,           // Key: F12
  KEY_LEFT_SHIFT = 80,    // Key: Shift left
  KEY_LEFT_CONTROL = 81,  // Key: Control left
  KEY_LEFT_ALT = 82,      // Key: Alt left
  KEY_LEFT_SUPER = 83,    // Key: Super left
  KEY_RIGHT_SHIFT = 84,   // Key: Shift right
  KEY_RIGHT_CONTROL = 85, // Key: Control right
  KEY_RIGHT_ALT = 86,     // Key: Alt right
  KEY_RIGHT_SUPER = 87,   // Key: Super right
  KEY_KB_MENU = 88,       // Key: KB menu
  // Keypad keys
  KEY_KP_0 = 89,         // Key: Keypad 0
  KEY_KP_1 = 90,         // Key: Keypad 1
  KEY_KP_2 = 91,         // Key: Keypad 2
  KEY_KP_3 = 92,         // Key: Keypad 3
  KEY_KP_4 = 93,         // Key: Keypad 4
  KEY_KP_5 = 94,         // Key: Keypad 5
  KEY_KP_6 = 95,         // Key: Keypad 6
  KEY_KP_7 = 96,         // Key: Keypad 7
  KEY_KP_8 = 97,         // Key: Keypad 8
  KEY_KP_9 = 98,         // Key: Keypad 9
  KEY_KP_DECIMAL = 99,   // Key: Keypad .
  KEY_KP_DIVIDE = 100,   // Key: Keypad /
  KEY_KP_MULTIPLY = 101, // Key: Keypad *
  KEY_KP_SUBTRACT = 102, // Key: Keypad -
  KEY_KP_ADD = 103,      // Key: Keypad +
  KEY_KP_ENTER = 104,    // Key: Keypad Enter
  KEY_KP_EQUAL = 105,    // Key: Keypad =
  // Android key buttons
  KEY_BACK = 106,       // Key: Android back button
  KEY_MENU = 107,       // Key: Android menu button
  KEY_VOLUME_UP = 108,  // Key: Android volume up button
  KEY_VOLUME_DOWN = 109 // Key: Android volume down button
} KeyboardKey;

KeyboardKey raylibToCevyKeyboard(int input) {
  switch (input) {
  case 0:
    return KEY_NULL;
  case 39:
    return KEY_APOSTROPHE;
  case 44:
    return KEY_COMMA;
  case 45:
    return KEY_MINUS;
  case 46:
    return KEY_PERIOD;
  case 47:
    return KEY_SLASH;
  case 48:
    return KEY_ZERO;
  case 49:
    return KEY_ONE;
  case 50:
    return KEY_TWO;
  case 51:
    return KEY_THREE;
  case 52:
    return KEY_FOUR;
  case 53:
    return KEY_FIVE;
  case 54:
    return KEY_SIX;
  case 55:
    return KEY_SEVEN;
  case 56:
    return KEY_EIGHT;
  case 57:
    return KEY_NINE;
  case 59:
    return KEY_SEMICOLON;
  case 61:
    return KEY_EQUAL;
  case 65:
    return KEY_A;
  case 66:
    return KEY_B;
  case 67:
    return KEY_C;
  case 68:
    return KEY_D;
  case 69:
    return KEY_E;
  case 70:
    return KEY_F;
  case 71:
    return KEY_G;
  case 72:
    return KEY_H;
  case 73:
    return KEY_I;
  case 74:
    return KEY_J;
  case 75:
    return KEY_K;
  case 76:
    return KEY_L;
  case 77:
    return KEY_M;
  case 78:
    return KEY_N;
  case 79:
    return KEY_O;
  case 80:
    return KEY_P;
  case 81:
    return KEY_Q;
  case 82:
    return KEY_R;
  case 83:
    return KEY_S;
  case 84:
    return KEY_T;
  case 85:
    return KEY_U;
  case 86:
    return KEY_V;
  case 87:
    return KEY_W;
  case 88:
    return KEY_X;
  case 89:
    return KEY_Y;
  case 90:
    return KEY_Z;
  case 91:
    return KEY_LEFT_BRACKET;
  case 92:
    return KEY_BACKSLASH;
  case 93:
    return KEY_RIGHT_BRACKET;
  case 96:
    return KEY_GRAVE;
  case 32:
    return KEY_SPACE;
  case 256:
    return KEY_ESCAPE;
  case 257:
    return KEY_ENTER;
  case 258:
    return KEY_TAB;
  case 259:
    return KEY_BACKSPACE;
  case 260:
    return KEY_INSERT;
  case 261:
    return KEY_DELETE;
  case 262:
    return KEY_RIGHT;
  case 263:
    return KEY_LEFT;
  case 264:
    return KEY_DOWN;
  case 265:
    return KEY_UP;
  case 266:
    return KEY_PAGE_UP;
  case 267:
    return KEY_PAGE_DOWN;
  case 268:
    return KEY_HOME;
  case 269:
    return KEY_END;
  case 280:
    return KEY_CAPS_LOCK;
  case 281:
    return KEY_SCROLL_LOCK;
  case 282:
    return KEY_NUM_LOCK;
  case 283:
    return KEY_PRINT_SCREEN;
  case 284:
    return KEY_PAUSE;
  case 290:
    return KEY_F1;
  case 291:
    return KEY_F2;
  case 292:
    return KEY_F3;
  case 293:
    return KEY_F4;
  case 294:
    return KEY_F5;
  case 295:
    return KEY_F6;
  case 296:
    return KEY_F7;
  case 297:
    return KEY_F8;
  case 298:
    return KEY_F9;
  case 299:
    return KEY_F10;
  case 300:
    return KEY_F11;
  case 301:
    return KEY_F12;
  case 340:
    return KEY_LEFT_SHIFT;
  case 341:
    return KEY_LEFT_CONTROL;
  case 342:
    return KEY_LEFT_ALT;
  case 343:
    return KEY_LEFT_SUPER;
  case 344:
    return KEY_RIGHT_SHIFT;
  case 345:
    return KEY_RIGHT_CONTROL;
  case 346:
    return KEY_RIGHT_ALT;
  case 347:
    return KEY_RIGHT_SUPER;
  case 348:
    return KEY_KB_MENU;
  case 320:
    return KEY_KP_0;
  case 321:
    return KEY_KP_1;
  case 322:
    return KEY_KP_2;
  case 323:
    return KEY_KP_3;
  case 324:
    return KEY_KP_4;
  case 325:
    return KEY_KP_5;
  case 326:
    return KEY_KP_6;
  case 327:
    return KEY_KP_7;
  case 328:
    return KEY_KP_8;
  case 329:
    return KEY_KP_9;
  case 330:
    return KEY_KP_DECIMAL;
  case 331:
    return KEY_KP_DIVIDE;
  case 332:
    return KEY_KP_MULTIPLY;
  case 333:
    return KEY_KP_SUBTRACT;
  case 334:
    return KEY_KP_ADD;
  case 335:
    return KEY_KP_ENTER;
  case 336:
    return KEY_KP_EQUAL;
  default:
    return KEY_NULL;
  };
};
