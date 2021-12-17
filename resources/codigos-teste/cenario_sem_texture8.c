#include "raylib.h"

#define MAX_BUILDINGS   100

int main2(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "buscando o cenário perfeito.teste8");

//    Image image = LoadImage("cyberpunk_street_background.png");     // Loaded in CPU memory (RAM)
//   Texture2D texture = LoadTextureFromImage(image);          // Image converted to texture, GPU memory (VRAM)
//    UnloadImage(image); //tentar achar uma forma de encaixar a textura na tela
    
    //se tirar os comentários de cima tem que tirar o comentário
    // que descarrega a textura no fim do programa

    Rectangle player = { 400, 280, 40, 40 };
    Rectangle buildings[MAX_BUILDINGS] = { 0 };
    Color buildColors[MAX_BUILDINGS] = { 0 };
 //   Rectangle buildingsLights[MAX_BUILDINGS] = { 0 };
            //pensar numa forma de adcionar luzes aleatórias aos prédios
            
    int spacing = 0;

    for (int i = 0; i < MAX_BUILDINGS; i++) //gerando prédios de tamanhos e cores diferentes
    {
        float width = (float)GetRandomValue(50, 200), height = (float)GetRandomValue(100, 500);
        buildings[i].width = width;
        buildings[i].height = height;
        buildings[i].y = screenHeight - 130.0f - buildings[i].height;
        buildings[i].x = -6000.0f + spacing;

        spacing += (int)buildings[i].width;

        buildColors[i] = (Color){ 0, GetRandomValue(82, 110), GetRandomValue(172, 220), 255 };
    }

    Camera2D camera = { 0 };
    camera.target = (Vector2){ player.x + 30.0f, player.y + 30.0f };
    camera.offset = (Vector2){ screenWidth/2.0f, screenHeight/2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 0.5f;

    SetTargetFPS(60);                   // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())        // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------

        // Player movement  (adicionar jump e slide)
        if (IsKeyDown(KEY_RIGHT)) player.x += 50;
        else if (IsKeyDown(KEY_LEFT)) player.x -= 50;

        // Camera target follows player
        camera.target = (Vector2){ player.x + 20, player.y + 20 };

        // Camera rotation controls (sem rotação de câmera)
/*         if (IsKeyDown(KEY_A)) camera.rotation--;
        else if (IsKeyDown(KEY_S)) camera.rotation++;

        // Limit camera rotation to 80 degrees (-40 to 40)
        if (camera.rotation > 40) camera.rotation = 40;
        else if (camera.rotation < -40) camera.rotation = -40; */

        // Camera zoom controls
        camera.zoom += ((float)GetMouseWheelMove()*0.05f);

        if (camera.zoom > 3.0f) camera.zoom = 3.0f;
        else if (camera.zoom < 0.1f) camera.zoom = 0.1f;

        // Camera reset (zoom and rotation)
        if (IsKeyPressed(KEY_R))
        {
            camera.zoom = 0.5f;
  //          camera.rotation = 0.0f;
        }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(GetColor(0x052c46ff));
 //           DrawTexture(texture, screenWidth/2 - texture.width/2, screenHeight/2 - texture.height/2, WHITE);
            BeginMode2D(camera);

                DrawRectangle(-6000, 320, 13000, 8000, BLACK);

                for (int i = 0; i < MAX_BUILDINGS; i++) DrawRectangleRec(buildings[i], buildColors[i]);

                DrawRectangleRec(player, MAROON);

            EndMode2D();

            DrawRectangle(0, 0, screenWidth, 5, RED);
            DrawRectangle(0, 5, 5, screenHeight - 10, RED);
            DrawRectangle(screenWidth - 5, 5, 5, screenHeight - 10, RED);
            DrawRectangle(0, screenHeight - 5, screenWidth, 5, RED);

            DrawText("Controles do joguinho:", 20, 20, 10, WHITE);
            DrawText("- Setinhas para locomoção (direita ou equerda)", 25, 40, 10, WHITE);
            DrawText("- Scroll do mouse aproxima ou afasta", 25, 60, 10, WHITE);
            DrawText("- R volta ao zoom original", 25, 80, 10, WHITE);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }
//    UnloadTexture(texture);
    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
