#pragma once

#include "raylib.h"

namespace cevy {
    class Keyboard {
        private:
            /* data */
        public:
        static bool keyPressed(int key) {return (IsKeyPressed(key));};
        static bool keyReleased(int key) {return (IsKeyPressed(key));};
        static bool keyDown(int key) {return (IsKeyDown(key));};
    };
}
