#ifndef PLAYER_H
#define PLAYER_H

#include <stdbool.h>   
#include "raylib.h"    

typedef enum {
    PLAYER_IDLE,
    PLAYER_RUN,
    PLAYER_JUMP,
    PLAYER_FALL,
} PlayerState;

typedef struct Player {
    Rectangle rect;
    float velocityY;
    float velocityX;
    bool isOnGround;
    PlayerState state;
    bool facingRight;
    int currentFrame;      // Which frame we're on
    float frameTimer;      // Time accumulator for animation
} Player;
#endif