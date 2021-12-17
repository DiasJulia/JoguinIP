#include "raylib.h"
#include <stdio.h>

#define NUM_FRAMES 3

#define VELOCITY 0.5f

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [textures] example - sprite button");

    Texture2D button = LoadTexture("resources/btn-bg.png");             // Load button texture
    Texture2D texture = LoadTexture("resources/unknown.png");           // background texture
    Texture2D runner = LoadTexture("resources/personagens/runner.png"); // Runner texture

    // Define frame rectangle for drawing
    float frameHeight = (float)button.height / NUM_FRAMES;
    Rectangle sourceRec = {0, 0, (float)button.width, frameHeight};
    Rectangle sourceRecRunner = {0, 0, (float)runner.width/5.8, (float)runner.height};

    // Define button bounds on screen
    Rectangle btnBounds = {screenWidth / 2.0f - button.width / 2.0f, screenHeight / 2.0f - button.height / NUM_FRAMES / 2.0f, (float)button.width, frameHeight};

    int btnState = 0;       // Button state: 0-NORMAL, 1-MOUSE_HOVER, 2-PRESSED
    bool gameState = false; // Button action should be activated

    Vector2 mousePoint = {0.0f, 0.0f};

    //Animations
    float runnerWidth = (float)runner.width/6.0;
    int runnerFrames = 6;
    float runnertimer= 0.0f;
    int runnerFrame = 0;

    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        mousePoint = GetMousePosition();
        gameState = false;

        // Check button state
        if (CheckCollisionPointRec(mousePoint, btnBounds))
        {
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                btnState = 2;
            else
                btnState = 1;

            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                gameState = true;
        }
        else
            btnState = 0;

        while (gameState && !WindowShouldClose())
        {
            //Game Here
            BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawTexture(texture, 0, 0, WHITE);
            DrawText("O JOGO COMEÇOU", 200, 100, 50, WHITE);

            runnertimer += GetFrameTime();

            if(runnertimer >= 0.2f){
                runnertimer = 0.0f;
                runnerFrame +=1;
            }

            runnerFrame = runnerFrame % runnerFrames;
            sourceRecRunner.x = runnerWidth * runnerFrame;

            DrawTextureRec(
                runner,
                sourceRecRunner,
                (Vector2){btnBounds.x, btnBounds.y},
                WHITE); // Draw button frame

            EndDrawing();
        }

        // Calculate button frame rectangle to draw depending on button state
        sourceRec.y = btnState * frameHeight;
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);
        DrawTexture(texture, 0, 0, WHITE);

        DrawTextureRec(button, sourceRec, (Vector2){btnBounds.x, btnBounds.y}, WHITE); // Draw button frame
        DrawText("INICIAR O JOGO", 200, 100, 50, WHITE);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    CloseWindow(); // Close window and OpenGL context

    return 0;
}
