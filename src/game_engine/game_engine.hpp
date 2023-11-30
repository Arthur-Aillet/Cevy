#pragma once

typedef struct drawable_s {
    /* data */
} drawable;

typedef struct controllable_s
{
    /* data */
} controllable;

#include "raylib.h"

class GameEngine {
    private:
        int screenWidth = 800;
        int screenHeight = 450;
    public:
        GameEngine();
        void update(void);
};
