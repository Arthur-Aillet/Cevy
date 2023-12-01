#pragma once

#include "raylib.h"

namespace Cevy {
    class GameEngine {
        private:
            int screenWidth = 800;
            int screenHeight = 450;
        public:
            GameEngine();
            void update(void);
    };
}
