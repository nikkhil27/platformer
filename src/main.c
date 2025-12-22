#include <stdio.h>
#include "raylib.h"
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 450
#include "player.h"
#define GRAVITY 1200.0f
#define JUMP_FORCE 500.0f
#define GROUND_Y 380

typedef struct Platform{
    Vector2 position;
    Vector2 size;
}Platform;

Platform platform = {
    .position = {0, SCREEN_HEIGHT - 80},
    .size = {SCREEN_WIDTH, 90}
};


int main(void){
    InitWindow(SCREEN_WIDTH,SCREEN_HEIGHT,"My Mario Platformer");
    SetTargetFPS(60);
    
    Player player = {
        .position = {100,GROUND_Y - 60},
        .size = {40,60},
        .velocityY = 0,
        .velocityX = 4.0f,
        .isOnGround = true
    };

    while(!WindowShouldClose()){

        float dt = GetFrameTime();

        if((IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_UP)) && player.isOnGround){
            player.velocityY = -JUMP_FORCE;
            player.isOnGround = false;
        }

        player.velocityY += GRAVITY * dt;
        player.position.y += player.velocityY * dt;
        player.isOnGround = false;
        // AABB collision check (player vs platform)
        if (
            player.position.x < platform.position.x + platform.size.x &&
            player.position.x + player.size.x > platform.position.x &&
            player.position.y < platform.position.y + platform.size.y &&
            player.position.y + player.size.y > platform.position.y
        ) {
            // Falling down onto platform
            if (player.velocityY > 0) {
                player.position.y = platform.position.y - player.size.y;
                player.velocityY = 0;
                player.isOnGround = true;
            }
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
            platform.position.x,
            platform.position.y,
            platform.size.x,
            platform.size.y,
            DARKGRAY
        );
        DrawRectangle(
            player.position.x,
            player.position.y,
            player.size.x,
            player.size.y,
            BLUE
        );
        DrawText("My Mario Platformer",20,20,20,BLACK);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}