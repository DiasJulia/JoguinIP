/*******************************************************************************************
*
*   raylib [textures] example - Background scrolling
*
*   This example has been created using raylib 2.0 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2019 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"
#define NUM_FRAMES  3  
int main(void)
{
    //////////////////////////////////// Initialization //////////////////////////////////////////////
    
    const int screenWidth = 800;
    const int screenHeight = 450;



////////////////////////////////DEFINIÇÕES BACKGROUND ////////////////////////////////////////////////////////////////////
    InitWindow(screenWidth, screenHeight, "raylib [textures] example - background scrolling");
  


    // NOTE: Be careful, background width must be equal or bigger than screen width
    // if not, texture should be draw more than two times for scrolling effect
    Texture2D background = LoadTexture("resources/cyberpunk_street_background.png");
    Texture2D midground = LoadTexture("resources/cyberpunk_street_midground.png");
    Texture2D foreground = LoadTexture("resources/cyberpunk_street_foreground.png");

    float scrollingBack = 0.0f;
    float scrollingMid = 0.0f;
    float scrollingFore = 0.0f;

    SetTargetFPS(60); // Set our game to run at 60 frames-per-second
    
    
   ///////////////////////////////////////DEFINIÇÕES BOTAO////////////////////////////////////////////////////////////////////// 
      InitWindow(screenWidth, screenHeight, "raylib [textures] example - sprite button");

    InitAudioDevice();      // Initialize audio device

    Sound fxButton = LoadSound("resources/buttonfx.wav");   // Load button sound
    Texture2D button = LoadTexture("resources/button.png"); // Load button texture

    // Define frame rectangle for drawing
    float frameHeight = (float)button.height/NUM_FRAMES;
    Rectangle sourceRec = { 0, 0, (float)button.width, frameHeight };

    // Define button bounds on screen
    Rectangle btnBounds = { screenWidth/2.0f - button.width/2.0f, screenHeight/2.0f - button.height/NUM_FRAMES/2.0f, (float)button.width, frameHeight };

    int btnState = 0;               // Button state: 0-NORMAL, 1-MOUSE_HOVER, 2-PRESSED
    bool btnAction = false;         // Button action should be activated

    Vector2 mousePoint = { 0.0f, 0.0f };



    SetTargetFPS(60); 
    
    
    //-------------------------------------------------------------------------------------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        scrollingBack -= 0.1f;
        scrollingMid -= 0.5f;
        scrollingFore -= 1.0f;

        // NOTE: Texture is scaled twice its size, so it sould be considered on scrolling
        if (scrollingBack <= -background.width*2) scrollingBack = 0;
        if (scrollingMid <= -midground.width*2) scrollingMid = 0;
        if (scrollingFore <= -foreground.width*2) scrollingFore = 0;
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        
        
        ///////////////////////////////BOTAO///////////////////////////////////////////
        mousePoint = GetMousePosition();
        btnAction = false;

        // Check button state
        if (CheckCollisionPointRec(mousePoint, btnBounds))
        {
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) btnState = 2;
            else btnState = 1;

            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) btnAction = true;
        }
        else btnState = 0;

        if (btnAction)
        {
            PlaySound(fxButton);

            // TODO: Any desired action
        }

        // Calculate button frame rectangle to draw depending on button state
        sourceRec.y = btnState*frameHeight;
        //----------------------------------------------------------------------------------

      
      
      
        BeginDrawing();
           //////////////////////FUNDO///////////////////////////////////////
            ClearBackground(GetColor(0x052c46ff));


            DrawTextureEx(background, (Vector2){ scrollingBack, 20 }, 0.0f, 2.0f, WHITE);
            DrawTextureEx(background, (Vector2){ background.width*2 + scrollingBack, 20 }, 0.0f, 2.0f, WHITE);

            
            DrawTextureEx(midground, (Vector2){ scrollingMid, 20 }, 0.0f, 2.0f, WHITE);
            DrawTextureEx(midground, (Vector2){ midground.width*2 + scrollingMid, 20 }, 0.0f, 2.0f, WHITE);

            
            DrawTextureEx(foreground, (Vector2){ scrollingFore, 70 }, 0.0f, 2.0f, WHITE);
            DrawTextureEx(foreground, (Vector2){ foreground.width*2 + scrollingFore, 70 }, 0.0f, 2.0f, WHITE);

            /////////////////////TEXTO//////////////////////////////
            DrawText("INICIAR O JOGO", 200, 100, 50, WHITE);
          
          
          ///////////////////////BOTAO///////////////////////////////////
          

            DrawTextureRec(button, sourceRec, (Vector2){ btnBounds.x, btnBounds.y }, WHITE); // Draw button frame
  ClearBackground(RAYWHITE);

        EndDrawing();
        
        
        
      
    }
    
   
  

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadTexture(background);  // Unload background texture
    UnloadTexture(midground);   // Unload midground texture
    UnloadTexture(foreground);  // Unload foreground texture
    UnloadTexture(button);  // Unload button texture
    UnloadSound(fxButton);  // Unload sound
    
     CloseAudioDevice();     // Close audio device
    CloseWindow();              // Close window and OpenGL context
    //--------------------------------------------------------------------------------------










    return 0;
}
