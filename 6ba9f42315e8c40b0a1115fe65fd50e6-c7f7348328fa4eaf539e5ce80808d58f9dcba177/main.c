

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
          
         

        EndDrawing();
        
        
        
      
    }
    
   
  

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadTexture(background);  // Unload background texture
    UnloadTexture(midground);   // Unload midground texture
    UnloadTexture(foreground);  // Unload foreground texture
   
    CloseWindow();              // Close window and OpenGL context
    //--------------------------------------------------------------------------------------










    return 0;
}
