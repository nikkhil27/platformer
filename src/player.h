#ifndef PLAYER_H
#define PLAYER_H

#include <stdbool.h>   
#include "raylib.h"    

typedef struct Player {
    Vector2 position;
    Vector2 size;
    float velocityY;
    float velocityX;
    bool isOnGround;
} Player;

#endif