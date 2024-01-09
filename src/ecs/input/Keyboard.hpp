/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** Keyboard
*/

#pragma once

#include "Input.hpp"

enum KeyboardInput{
  NULL_KEY = 0, // Key: NULL_KEY, used for no key pressed
  // Alphanumeric keys
  A = 1,              // Key: A | a
  B = 2,              // Key: B | b
  C = 3,              // Key: C | c
  D = 4,              // Key: D | d
  E = 5,              // Key: E | e
  F = 6,              // Key: F | f
  G = 7,              // Key: G | g
  H = 8,              // Key: H | h
  I = 9,              // Key: I | i
  J = 10,             // Key: J | j
  K = 11,             // Key: K | k
  L = 12,             // Key: L | l
  M = 13,             // Key: M | m
  N = 14,             // Key: N | n
  O = 15,             // Key: O | o
  P = 16,             // Key: P | p
  Q = 17,             // Key: Q | q
  R = 18,             // Key: R | r
  S = 19,             // Key: S | s
  T = 20,             // Key: T | t
  U = 21,             // Key: U | u
  V = 22,             // Key: V | v
  W = 23,             // Key: W | w
  X = 24,             // Key: X | x
  Y = 25,             // Key: Y | y
  Z = 26,             // Key: Z | z
  ZERO = 27,          // Key: 0
  ONE = 28,           // Key: 1
  TWO = 29,           // Key: 2
  THREE = 30,         // Key: 3
  FOUR = 31,          // Key: 4
  FIVE = 32,          // Key: 5
  SIX = 33,           // Key: 6
  SEVEN = 34,         // Key: 7
  EIGHT = 35,         // Key: 8
  NINE = 36,          // Key: 9
  APOSTROPHE = 37,    // Key: '
  COMMA = 38,         // Key: ,
  MINUS = 39,         // Key: -
  PERIOD = 40,        // Key: .
  SLASH = 41,         // Key: /
  SEMICOLON = 42,     // Key: ;
  EQUAL = 43,         // Key: =
  LEFT_BRACKET = 44,  // Key: [
  BACKSLASH = 45,     // Key: '\'
  RIGHT_BRACKET = 46, // Key: ]
  GRAVE = 47,         // Key: `
  // Function keys
  SPACE = 48,         // Key: Space
  ESCAPE = 49,        // Key: Esc
  ENTER = 50,         // Key: Enter
  TAB = 51,           // Key: Tab
  BACKSPACE = 52,     // Key: Backspace
  INSERT = 53,        // Key: Ins
  DELETE = 54,        // Key: Del
  RIGHT = 55,         // Key: Cursor right
  LEFT = 56,          // Key: Cursor left
  DOWN = 57,          // Key: Cursor down
  UP = 58,            // Key: Cursor up
  PAGE_UP = 59,       // Key: Page up
  PAGE_DOWN = 60,     // Key: Page down
  HOME = 61,          // Key: Home
  END = 62,           // Key: End
  CAPS_LOCK = 63,     // Key: Caps lock
  SCROLL_LOCK = 64,   // Key: Scroll down
  NUM_LOCK = 65,      // Key: Num lock
  PRINT_SCREEN = 66,  // Key: Print screen
  PAUSE = 67,         // Key: Pause
  F1 = 68,            // Key: F1
  F2 = 69,            // Key: F2
  F3 = 70,            // Key: F3
  F4 = 71,            // Key: F4
  F5 = 72,            // Key: F5
  F6 = 73,            // Key: F6
  F7 = 74,            // Key: F7
  F8 = 75,            // Key: F8
  F9 = 76,            // Key: F9
  F10 = 77,           // Key: F10
  F11 = 78,           // Key: F11
  F12 = 79,           // Key: F12
  LEFT_SHIFT = 80,    // Key: Shift left
  LEFT_CONTROL = 81,  // Key: Control left
  LEFT_ALT = 82,      // Key: Alt left
  LEFT_SUPER = 83,    // Key: Super left
  RIGHT_SHIFT = 84,   // Key: Shift right
  RIGHT_CONTROL = 85, // Key: Control right
  RIGHT_ALT = 86,     // Key: Alt right
  RIGHT_SUPER = 87,   // Key: Super right
  KB_MENU = 88,       // Key: KB menu
  // Keypad keys
  KP_0 = 89,         // Key: Keypad 0
  KP_1 = 90,         // Key: Keypad 1
  KP_2 = 91,         // Key: Keypad 2
  KP_3 = 92,         // Key: Keypad 3
  KP_4 = 93,         // Key: Keypad 4
  KP_5 = 94,         // Key: Keypad 5
  KP_6 = 95,         // Key: Keypad 6
  KP_7 = 96,         // Key: Keypad 7
  KP_8 = 97,         // Key: Keypad 8
  KP_9 = 98,         // Key: Keypad 9
  KP_DECIMAL = 99,   // Key: Keypad .
  KP_DIVIDE = 100,   // Key: Keypad /
  KP_MULTIPLY = 101, // Key: Keypad *
  KP_SUBTRACT = 102, // Key: Keypad -
  KP_ADD = 103,      // Key: Keypad +
  KP_ENTER = 104,    // Key: Keypad Enter
  KP_EQUAL = 105,    // Key: Keypad =
  // Android key buttons
  BACK = 106,       // Key: Android back button
  MENU = 107,       // Key: Android menu button
  VOLUME_UP = 108,  // Key: Android volume up button
  VOLUME_DOWN = 109 // Key: Android volume down button
};

struct KeyBoard {
  unsigned int scancode;
  KeyboardInput keycode;
  ButtonState state;
};

KeyboardInput raylibToCevyKeyboard(int input) {
  switch (input) {
  case 0:
    return KeyboardInput::NULL_KEY;
  case 39:
    return KeyboardInput::APOSTROPHE;
  case 44:
    return KeyboardInput::COMMA;
  case 45:
    return KeyboardInput::MINUS;
  case 46:
    return KeyboardInput::PERIOD;
  case 47:
    return KeyboardInput::SLASH;
  case 48:
    return KeyboardInput::ZERO;
  case 49:
    return KeyboardInput::ONE;
  case 50:
    return KeyboardInput::TWO;
  case 51:
    return KeyboardInput::THREE;
  case 52:
    return KeyboardInput::FOUR;
  case 53:
    return KeyboardInput::FIVE;
  case 54:
    return KeyboardInput::SIX;
  case 55:
    return KeyboardInput::SEVEN;
  case 56:
    return KeyboardInput::EIGHT;
  case 57:
    return KeyboardInput::NINE;
  case 59:
    return KeyboardInput::SEMICOLON;
  case 61:
    return KeyboardInput::EQUAL;
  case 65:
    return KeyboardInput::A;
  case 66:
    return KeyboardInput::B;
  case 67:
    return KeyboardInput::C;
  case 68:
    return KeyboardInput::D;
  case 69:
    return KeyboardInput::E;
  case 70:
    return KeyboardInput::F;
  case 71:
    return KeyboardInput::G;
  case 72:
    return KeyboardInput::H;
  case 73:
    return KeyboardInput::I;
  case 74:
    return KeyboardInput::J;
  case 75:
    return KeyboardInput::K;
  case 76:
    return KeyboardInput::L;
  case 77:
    return KeyboardInput::M;
  case 78:
    return KeyboardInput::N;
  case 79:
    return KeyboardInput::O;
  case 80:
    return KeyboardInput::P;
  case 81:
    return KeyboardInput::Q;
  case 82:
    return KeyboardInput::R;
  case 83:
    return KeyboardInput::S;
  case 84:
    return KeyboardInput::T;
  case 85:
    return KeyboardInput::U;
  case 86:
    return KeyboardInput::V;
  case 87:
    return KeyboardInput::W;
  case 88:
    return KeyboardInput::X;
  case 89:
    return KeyboardInput::Y;
  case 90:
    return KeyboardInput::Z;
  case 91:
    return KeyboardInput::LEFT_BRACKET;
  case 92:
    return KeyboardInput::BACKSLASH;
  case 93:
    return KeyboardInput::RIGHT_BRACKET;
  case 96:
    return KeyboardInput::GRAVE;
  case 32:
    return KeyboardInput::SPACE;
  case 256:
    return KeyboardInput::ESCAPE;
  case 257:
    return KeyboardInput::ENTER;
  case 258:
    return KeyboardInput::TAB;
  case 259:
    return KeyboardInput::BACKSPACE;
  case 260:
    return KeyboardInput::INSERT;
  case 261:
    return KeyboardInput::DELETE;
  case 262:
    return KeyboardInput::RIGHT;
  case 263:
    return KeyboardInput::LEFT;
  case 264:
    return KeyboardInput::DOWN;
  case 265:
    return KeyboardInput::UP;
  case 266:
    return KeyboardInput::PAGE_UP;
  case 267:
    return KeyboardInput::PAGE_DOWN;
  case 268:
    return KeyboardInput::HOME;
  case 269:
    return KeyboardInput::END;
  case 280:
    return KeyboardInput::CAPS_LOCK;
  case 281:
    return KeyboardInput::SCROLL_LOCK;
  case 282:
    return KeyboardInput::NUM_LOCK;
  case 283:
    return KeyboardInput::PRINT_SCREEN;
  case 284:
    return KeyboardInput::PAUSE;
  case 290:
    return KeyboardInput::F1;
  case 291:
    return KeyboardInput::F2;
  case 292:
    return KeyboardInput::F3;
  case 293:
    return KeyboardInput::F4;
  case 294:
    return KeyboardInput::F5;
  case 295:
    return KeyboardInput::F6;
  case 296:
    return KeyboardInput::F7;
  case 297:
    return KeyboardInput::F8;
  case 298:
    return KeyboardInput::F9;
  case 299:
    return KeyboardInput::F10;
  case 300:
    return KeyboardInput::F11;
  case 301:
    return KeyboardInput::F12;
  case 340:
    return KeyboardInput::LEFT_SHIFT;
  case 341:
    return KeyboardInput::LEFT_CONTROL;
  case 342:
    return KeyboardInput::LEFT_ALT;
  case 343:
    return KeyboardInput::LEFT_SUPER;
  case 344:
    return KeyboardInput::RIGHT_SHIFT;
  case 345:
    return KeyboardInput::RIGHT_CONTROL;
  case 346:
    return KeyboardInput::RIGHT_ALT;
  case 347:
    return KeyboardInput::RIGHT_SUPER;
  case 348:
    return KeyboardInput::KB_MENU;
  case 320:
    return KeyboardInput::KP_0;
  case 321:
    return KeyboardInput::KP_1;
  case 322:
    return KeyboardInput::KP_2;
  case 323:
    return KeyboardInput::KP_3;
  case 324:
    return KeyboardInput::KP_4;
  case 325:
    return KeyboardInput::KP_5;
  case 326:
    return KeyboardInput::KP_6;
  case 327:
    return KeyboardInput::KP_7;
  case 328:
    return KeyboardInput::KP_8;
  case 329:
    return KeyboardInput::KP_9;
  case 330:
    return KeyboardInput::KP_DECIMAL;
  case 331:
    return KeyboardInput::KP_DIVIDE;
  case 332:
    return KeyboardInput::KP_MULTIPLY;
  case 333:
    return KeyboardInput::KP_SUBTRACT;
  case 334:
    return KeyboardInput::KP_ADD;
  case 335:
    return KeyboardInput::KP_ENTER;
  case 336:
    return KeyboardInput::KP_EQUAL;
  default:
    return KeyboardInput::NULL_KEY;
  };
};
