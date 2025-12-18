#include <stdio.h>
#include "raylib.h"

int main(void){
    const int screenWidth = 800;
    const int screenHeight = 450;
    InitWindow(screenWidth,screenHeight,"My Mario Platoformer");
    SetTargetFPS(60);

    while(!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Hello Mario World!",280,200,20,DARKGRAY);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}