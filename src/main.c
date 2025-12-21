#include <stdio.h>
#include "raylib.h"
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 450

#define GRAVITY 0.6f
#define JUMP_FORCE 12.0f
#define GROUND_Y 380

typedef struct Player{
    Vector2 position;
    Vector2 size;
    float velocityY;
    bool isOnGround;
}Player;


int main(void){
    InitWindow(SCREEN_WIDTH,SCREEN_HEIGHT,"My Mario Platoformer");
    SetTargetFPS(60);
    
    Player player = {
        .position = {100,GROUND_Y},
        .size = {40,60},
        .velocityY = 0,
        .isOnGround = true
    };

    while(!WindowShouldClose()){

        if(IsKeyDown(KEY_SPACE) && player.isOnGround){
            player.velocityY = -JUMP_FORCE;
            player.isOnGround = false;
        }

        player.velocityY += GRAVITY;
        player.position.y += player.velocityY;

        if(player.position.y >= GROUND_Y){
            player.position.y = GROUND_Y;
            player.velocityY = 0;
            player.isOnGround = true;
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawRectangle(
            player.position.x,
            player.position.y,
            player.size.x,
            player.size.y,
            BLUE
        );
        DrawRectangle(0, GROUND_Y + player.size.y, SCREEN_WIDTH, 5, DARKGRAY);
        DrawText("Press SPACE to jump", 20, 20, 20, BLACK);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}