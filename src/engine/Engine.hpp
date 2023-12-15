/*
** Agartha-Software, 2023
** Cevy
** File description:
** GameEngine
*/

#pragma once

#include "Plugin.hpp"
#include "ecs.hpp"

namespace cevy {
    class Engine : public cevy::ecs::Plugin {
        public:
            void static build(cevy::ecs::App& app);
        /*
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
            Engine();
            void update(void);
        */
    };
}
