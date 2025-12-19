#include <stdio.h>
#include "raylib.h"

typedef struct Player{
    float x;
    float y;
    float width;
    float height;
    float speed;
}Player;


int main(void){
    const int screenWidth = 800;
    const int screenHeight = 450;
    InitWindow(screenWidth,screenHeight,"My Mario Platoformer");
    SetTargetFPS(60);
    
    Player player;
    player.width = 50;
    player.height = 20;
    player.x = screenWidth/2 - player.width/2;
    player.y = screenHeight - player.height - 10;
    player.speed = 5;

    while(!WindowShouldClose()){

        if(IsKeyDown(KEY_LEFT)){
            player.x -= player.speed;
        }
        if(IsKeyDown(KEY_RIGHT)){
            player.x += player.speed;
        }
        if (player.x < 0)
            player.x = 0;

        if (player.x > screenWidth - player.width)
            player.x = screenWidth - player.width;

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawRectangle(
            (int)player.x,
            (int)player.y,
            (int)player.height,
            (int)player.width,
            BLUE
        );
        EndDrawing();
    }
    CloseWindow();
    return 0;
}