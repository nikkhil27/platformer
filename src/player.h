#ifndef PLAYER_H
#define PLAYER_H

#include <stdbool.h>   
#include "raylib.h"    

typedef struct Player {
    Rectangle rect;
    float velocityY;
    float velocityX;
    bool isOnGround;
} Player;

#endif