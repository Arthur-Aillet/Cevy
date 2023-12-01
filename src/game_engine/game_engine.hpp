#pragma once

#include "raylib.h"
#include "camera.hpp"

namespace Cevy {
    class GameEngine {
        private:
            int screenWidth = 800;
            int screenHeight = 450;
            int windowWidth;
            int windowHeight;
            bool debugMode = false;
            Camera camera;
        public:
            void Fullscreen (void);
            void DebugWindow (void);
            GameEngine();
            void update(void);
    };
}
