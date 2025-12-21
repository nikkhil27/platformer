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
    float velocityX;
    bool isOnGround;
}Player;


int main(void){
    InitWindow(SCREEN_WIDTH,SCREEN_HEIGHT,"My Mario Platoformer");
    SetTargetFPS(60);
    
    Player player = {
        .position = {100,GROUND_Y - 60},
        .size = {40,60},
        .velocityY = 0,
        .velocityX = 4.0f,
        .isOnGround = true
    };

    while(!WindowShouldClose()){

        if((IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_UP)) && player.isOnGround){
            player.velocityY = -JUMP_FORCE;
            player.isOnGround = false;
        }

        player.velocityY += GRAVITY;
        player.position.y += player.velocityY;

        if(player.position.y + player.size.y >= GROUND_Y){
            player.position.y = GROUND_Y - player.size.y;
            player.velocityY = 0;
            player.isOnGround = true;
        }

        if(IsKeyDown(KEY_LEFT)){
            player.position.x -= player.velocityX;
        }
        if(IsKeyDown(KEY_RIGHT)){
            player.position.x += player.velocityX;
        }

        if(player.position.x < 0){player.position.x = 0;}

        if(player.position.x + player.size.x > SCREEN_WIDTH){player.position.x = SCREEN_WIDTH - player.size.x;}

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawRectangle(
            player.position.x,
            player.position.y,
            player.size.x,
            player.size.y,
            BLUE
        );
        DrawRectangle(0, GROUND_Y, SCREEN_WIDTH, 5, DARKGRAY);
        DrawText("Press SPACE to jump", 20, 20, 20, BLACK);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}