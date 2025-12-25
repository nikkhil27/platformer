#include <stdio.h>
#include <math.h>
#include "raylib.h"
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 450
#include "player.h"
#define GRAVITY 1200.0f
#define JUMP_FORCE 500.0f
#define GROUND_Y 380
#define MAX_PLATFORMS 10
#define LEVEL_WIDTH 2000
#define FRAME_SPEED 0.1f
#define TILE_SIZE 16

// Brown terrain tileset (row 1, col 1 in your image = row 0 in code)
#define TILE_GRASS_LEFT   9   // First column of second row
#define TILE_GRASS_MID    10  // Second column of second row
#define TILE_GRASS_RIGHT  11  // Third column of second row

typedef struct Platform{
    Rectangle rect;
}Platform;

void DrawTiles(Platform platform, Texture2D terrain, float scale){
    int tilesize = TILE_SIZE;
    int scaledTileSize = tilesize * scale;

    int tilesX = (int)(platform.rect.width / scaledTileSize);
    int tilesY = (int)(platform.rect.height / scaledTileSize);

    for(int y = 0; y < tilesY; y++){
        for(int x = 0; x < tilesX; x++){
            Rectangle sourceRec;

            // Top row - grass tiles
            if(y == 0){
                if(x == 0){
                    // Top-left grass corner
                    sourceRec = (Rectangle){tilesize * 0, tilesize * 1, tilesize, tilesize};
                }
                else if(x == tilesX - 1){
                    // Top-right grass corner
                    sourceRec = (Rectangle){tilesize * 2, tilesize * 1, tilesize, tilesize};
                }
                else{
                    // Top middle grass
                    sourceRec = (Rectangle){tilesize * 1, tilesize * 1, tilesize, tilesize};
                }
            }
            // Middle rows - dirt
            else if (y < tilesY - 1) {
                if (x == 0) {
                    sourceRec = (Rectangle){tilesize * 0, tilesize * 2, tilesize, tilesize};
                } else if (x == tilesX - 1) {
                    sourceRec = (Rectangle){tilesize * 2, tilesize * 2, tilesize, tilesize};
                } else {
                    sourceRec = (Rectangle){tilesize * 1, tilesize * 2, tilesize, tilesize};
                }
            }
            // Bottom row
            else {
                if (x == 0) {
                    sourceRec = (Rectangle){tilesize * 0, tilesize * 3, tilesize, tilesize};
                } else if (x == tilesX - 1) {
                    sourceRec = (Rectangle){tilesize * 2, tilesize * 3, tilesize, tilesize};
                } else {
                    sourceRec = (Rectangle){tilesize * 1, tilesize * 3, tilesize, tilesize};
                }
            }
            
            Rectangle destRec = {
                platform.rect.x + x * scaledTileSize,
                platform.rect.y + y * scaledTileSize,
                scaledTileSize,
                scaledTileSize
            };
            
            DrawTexturePro(terrain, sourceRec, destRec, (Vector2){0, 0}, 0.0f, WHITE);
        }
    }
}


int main(void){
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "My Mario Platformer");
    SetTargetFPS(60);

    Player player = {
        .rect = {100, 300, 40, 40},
        .velocityY = 0,
        .velocityX = 300.0f,  // Changed to pixels per second
        .isOnGround = true,
        .state = PLAYER_IDLE,
        .facingRight = true,
        .currentFrame = 0,
        .frameTimer = 0.0f
    };

    Texture2D playerRun = LoadTexture("../assets/Main Characters/Pink Man/Run (32x32).png");
    Texture2D playerFall = LoadTexture("../assets/Main Characters/Pink Man/Fall (32x32).png");
    Texture2D playerJump = LoadTexture("../assets/Main Characters/Pink Man/Jump (32x32).png");
    Texture2D playerIdle = LoadTexture("../assets/Main Characters/Pink Man/Idle (32x32).png");

    Texture2D background = LoadTexture("../assets/Background/Blue.png");
    Texture2D terrain = LoadTexture("../assets/Terrain/Terrain (16x16).png");
    
    int runFrames = 12;
    int fallFrames = 1;
    int JumpFrames = 1;
    int idleFrames = 11;

    Camera2D camera = {0};
    camera.target = (Vector2){ player.rect.x + player.rect.width/2, player.rect.y };
    camera.offset = (Vector2){ SCREEN_WIDTH/2, SCREEN_HEIGHT/2 };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
    
    Platform platforms[MAX_PLATFORMS];
    int platformCount = 7;
    
    float tileScale = 2.5f;
    int scaledTile = TILE_SIZE * tileScale;

    // Ground platform - thicker and extends full level
    platforms[0].rect = (Rectangle){0, 380, LEVEL_WIDTH, scaledTile * 3};

    // Floating Platforms - made wider for better gameplay
    platforms[1].rect = (Rectangle){200, 320, scaledTile * 4, scaledTile * 2};
    platforms[2].rect = (Rectangle){450, 260, scaledTile * 4, scaledTile * 2};
    platforms[3].rect = (Rectangle){700, 200, scaledTile * 4, scaledTile * 2};
    platforms[4].rect = (Rectangle){950, 280, scaledTile * 5, scaledTile * 2};
    platforms[5].rect = (Rectangle){1200, 220, scaledTile * 4, scaledTile * 2};
    platforms[6].rect = (Rectangle){1500, 180, scaledTile * 6, scaledTile * 2};

    while(!WindowShouldClose()){
        float dt = GetFrameTime();

        // Camera follows player on X axis
        camera.target.x = player.rect.x + player.rect.width / 2;

        // Clamp camera so it doesn't show outside level
        if (camera.target.x < camera.offset.x)
            camera.target.x = camera.offset.x;

        if (camera.target.x > LEVEL_WIDTH - camera.offset.x)
            camera.target.x = LEVEL_WIDTH - camera.offset.x;

        // Animation
        player.frameTimer += dt;
        if (player.frameTimer >= FRAME_SPEED) {
            player.frameTimer = 0.0f;
            player.currentFrame++;
            
            int maxFrames = runFrames;
            switch (player.state) {
                case PLAYER_RUN:
                    maxFrames = runFrames;
                    break;
                case PLAYER_FALL:
                    maxFrames = fallFrames;
                    break;
                case PLAYER_JUMP:
                    maxFrames = JumpFrames;
                    break;
                case PLAYER_IDLE:
                    maxFrames = idleFrames;
                    break;
            }
            
            if (player.currentFrame >= maxFrames) {
                player.currentFrame = 0;
            }
        }

        // Jump
        if((IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_UP)) && player.isOnGround){
            player.velocityY = -JUMP_FORCE;
            player.isOnGround = false;
        }

        // Gravity
        player.velocityY += GRAVITY * dt;
        float prevY = player.rect.y;
        player.rect.y += player.velocityY * dt;
        player.isOnGround = false;

        // Horizontal movement
        if (IsKeyDown(KEY_LEFT)) {
            player.rect.x -= player.velocityX * dt;
        }
        if (IsKeyDown(KEY_RIGHT)) {
            player.rect.x += player.velocityX * dt;
        }

        // Update player state
        if (!player.isOnGround) {
            if (player.velocityY < 0) {
                player.state = PLAYER_JUMP;
            } else {
                player.state = PLAYER_FALL;
            }
        } else if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_RIGHT)) {
            player.state = PLAYER_RUN;
            if (IsKeyDown(KEY_LEFT)) player.facingRight = false;
            if (IsKeyDown(KEY_RIGHT)) player.facingRight = true;
        } else {
            player.state = PLAYER_IDLE;
        }

        // Keep player in bounds
        if(player.rect.x < 0) player.rect.x = 0;
        if(player.rect.x + player.rect.width > LEVEL_WIDTH) 
            player.rect.x = LEVEL_WIDTH - player.rect.width;

        // Platform collision
        for(int i = 0; i < platformCount; i++){
            Platform p = platforms[i];
            
            if(CheckCollisionRecs(player.rect, p.rect)){
                if (prevY + player.rect.height <= p.rect.y) {
                    player.rect.y = p.rect.y - player.rect.height;
                    player.velocityY = 0;
                    player.isOnGround = true;
                }
            }
        }

        // Fallback ground
        if (player.rect.y > SCREEN_HEIGHT) {
            player.rect.y = SCREEN_HEIGHT - player.rect.height;
            player.velocityY = 0;
            player.isOnGround = true;
        }

        // RENDERING
        BeginDrawing();
        ClearBackground((Color){135, 206, 235, 255}); // Sky blue
        BeginMode2D(camera);

        // Improved parallax background
        float bgSpeed = 0.3f;  // Slower = more depth
        float bgScale = 3.0f;
        int bgWidth = background.width * bgScale;
        int bgHeight = background.height * bgScale;
        
        // Calculate offset based on camera position
        float parallaxOffset = camera.target.x * bgSpeed;
        
        // Calculate which tiles to draw
        float startX = parallaxOffset - camera.offset.x;
        int firstTile = (int)(startX / bgWidth) - 1;
        int lastTile = firstTile + (int)(SCREEN_WIDTH / bgWidth) + 3;
        
        // Draw seamless background tiles
        for (int i = firstTile; i <= lastTile; i++) {
            Vector2 pos = {
                i * bgWidth - fmod(parallaxOffset, bgWidth),
                0
            };
            DrawTextureEx(background, pos, 0.0f, bgScale, WHITE);
        }
        
        // Draw platforms with tiles
        for (int i = 0; i < platformCount; i++) {
            DrawTiles(platforms[i], terrain, tileScale);
        }
        
        // Draw player
        Texture2D currentTexture;
        int frameCount;

        switch (player.state) {
            case PLAYER_RUN:
                currentTexture = playerRun;
                frameCount = runFrames;
                break;
            case PLAYER_FALL:
                currentTexture = playerFall;
                frameCount = fallFrames;
                break;
            case PLAYER_JUMP:
                currentTexture = playerJump;
                frameCount = JumpFrames;
                break;
            case PLAYER_IDLE:
            default:
                currentTexture = playerIdle;
                frameCount = idleFrames;
        }

        int frameW = currentTexture.width / frameCount;
        Rectangle sourceRec = {
            player.currentFrame * frameW,
            0,
            frameW * (player.facingRight ? 1 : -1),
            currentTexture.height
        };

        DrawTexturePro(
            currentTexture,
            sourceRec,
            player.rect,
            (Vector2){0, 0},
            0.0f,
            WHITE
        );

        EndMode2D();  // IMPORTANT: Close camera mode
        EndDrawing();
    }
    
    UnloadTexture(playerRun);
    UnloadTexture(playerFall);
    UnloadTexture(playerJump);
    UnloadTexture(playerIdle);
    UnloadTexture(background);
    UnloadTexture(terrain);
    CloseWindow();
    return 0;
}