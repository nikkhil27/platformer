#include <stdio.h>
#include "raylib.h"
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 450
#include "player.h"
#define GRAVITY 1200.0f
#define JUMP_FORCE 500.0f
#define GROUND_Y 380
#define MAX_PLATFORMS 10


typedef struct Platform{
    Rectangle rect;
}Platform;

int main(void){
    InitWindow(SCREEN_WIDTH,SCREEN_HEIGHT,"My Mario Platformer");
    SetTargetFPS(60);
    
    Player player = {
        .rect = {100, 300, 40, 40},
        .velocityY = 0,
        .velocityX = 4.0f,
        .isOnGround = true
    };

    Platform platforms[MAX_PLATFORMS];
    int platformCount = 4;

    platforms[0].rect = (Rectangle){0, 400, 800, 50};  
    platforms[1].rect = (Rectangle){200, 320, 120, 20};
    platforms[2].rect = (Rectangle){400, 260, 120, 20};
    platforms[3].rect = (Rectangle){600, 200, 120, 20};

    while(!WindowShouldClose()){

        float dt = GetFrameTime();

        if((IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_UP)) && player.isOnGround){
            player.velocityY = -JUMP_FORCE;
            player.isOnGround = false;
        }

        player.velocityY += GRAVITY * dt;
        float prevY = player.rect.y;
        player.rect.y += player.velocityY * dt;
        player.isOnGround = false;

        if(IsKeyDown(KEY_LEFT)){
            player.rect.x -= player.velocityX;
        }
        if(IsKeyDown(KEY_RIGHT)){
            player.rect.x += player.velocityX;
        }

        if(player.rect.x < 0){player.rect.x = 0;}

        if(player.rect.x + player.rect.width > SCREEN_WIDTH){player.rect.x = SCREEN_WIDTH - player.rect.width;}

        for(int i=0; i< platformCount;i++){
            Platform p = platforms[i];
            
            if(CheckCollisionRecs(player.rect,p.rect)){
                // Falling onto platform
                if (prevY + player.rect.height <= p.rect.y)
                {
                    player.rect.y = p.rect.y - player.rect.height;
                    player.velocityY = 0;
                    player.isOnGround = true;
                }
            }
        }


        BeginDrawing();
        ClearBackground(RAYWHITE);
         DrawRectangleRec(player.rect, BLUE);
        
        for (int i = 0; i < platformCount; i++)
        {
            DrawRectangleRec(platforms[i].rect, DARKGRAY);
        }

        DrawText("My Mario Platformer",20,20,20,BLACK);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}