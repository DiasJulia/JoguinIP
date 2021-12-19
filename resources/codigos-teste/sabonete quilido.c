#include "raylib.h"
#define NUM_FRAMES 3
#define PHYSAC_IMPLEMENTATION
#include "extras/physac.h"

#define VELOCITY 0.5f

void abaixar(PhysicsBody *body){
    Vector2 position = (*body)->position;
    int isGrounded = (*body)->isGrounded;
    DestroyPhysicsBody(*body);

    *body = CreatePhysicsBodyRectangle(position, 50, 25, 1);
    (*body)->isGrounded = isGrounded;
}

void aumentar(PhysicsBody *body){
    Vector2 position = (*body)->position;
    int isGrounded = (*body)->isGrounded;
    DestroyPhysicsBody(*body);

    *body = CreatePhysicsBodyRectangle(position, 50, 50, 1);
    (*body)->isGrounded = isGrounded;
}

int main12(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [textures] example - sprite button");

    InitAudioDevice(); // Initialize audio device

    Sound fxButton = LoadSound("resources/img/button.wav");     // Load button sound
    Texture2D button = LoadTexture("resources/img/btn-bg.png");   // Load button texture
    Texture2D texture = LoadTexture("resources/img/unknown.png"); // background texture
    Texture2D runner = LoadTexture("resources/personagens/runner.png"); // Runner texture

    Rectangle sourceRecRunner = {0, 0, (float)runner.width/6.05, (float)runner.height};

    // Define frame rectangle for drawing
    float frameHeight = (float)button.height / NUM_FRAMES;
    Rectangle sourceRec = {0, 0, (float)button.width, frameHeight};

    // Define button bounds on screen
    Rectangle btnBounds = {screenWidth / 2.0f - button.width / 2.0f, screenHeight / 2.0f - button.height / NUM_FRAMES / 2.0f, (float)button.width, frameHeight};

    int btnState = 0;       // Button state: 0-NORMAL, 1-MOUSE_HOVER, 2-PRESSED
    bool btnAction = false; // Button action should be activated

    Vector2 mousePoint = {0.0f, 0.0f};

    //Game Configuration

    // Initialize physics and default physics bodies
    InitPhysics();
    
    //proporcao barreira e chao
    //em cima *2 + x
    //em baixo *1.5 + x/2

    // Create floor and walls rectangle physics body
    PhysicsBody afloor = CreatePhysicsBodyRectangle((Vector2){screenWidth / 2.0f, (float)screenHeight}, (float)screenWidth * 2 + 2000, 100, 10);
    //PhysicsBody floor2 = CreatePhysicsBodyRectangle((Vector2){screenWidth / 2.0f + (float)screenWidth * 2 + 2000, (float)screenHeight}, (float)screenWidth * 2 + 1650, 100, 10);
    PhysicsBody aplataforma = CreatePhysicsBodyRectangle((Vector2){screenWidth + 1600, (float)screenHeight - 150}, 100, 20, 10);
    PhysicsBody aplataforma2 = CreatePhysicsBodyRectangle((Vector2){screenWidth + 1850, (float)screenHeight - 180}, 80, 20, 10);
    PhysicsBody aplataforma3 = CreatePhysicsBodyRectangle((Vector2){screenWidth + 2050, (float)screenHeight - 210}, 60, 20, 10);
    PhysicsBody aplataforma4 = CreatePhysicsBodyRectangle((Vector2){screenWidth + 2250, (float)screenHeight - 240}, 40, 20, 10);
    PhysicsBody api = CreatePhysicsBodyRectangle((Vector2){screenWidth + 2750, (float)screenHeight - 240}, 600, 20, 10);
    PhysicsBody abloco = CreatePhysicsBodyRectangle((Vector2){screenWidth + 2750, (float)screenHeight - 320}, 60, 70, 10);
    PhysicsBody aplataforma7 = CreatePhysicsBodyRectangle((Vector2){screenWidth + 3300, (float)screenHeight - 180}, 120, 20, 30);
    PhysicsBody aplataforma9 = CreatePhysicsBodyRectangle((Vector2){screenWidth + 3950, (float)screenHeight - 120}, 800, 20, 30);
    PhysicsBody acaixa = CreatePhysicsBodyRectangle((Vector2){screenWidth + 4200, (float)screenHeight - 260}, 50, 200, 30);
    PhysicsBody acaixa2 = CreatePhysicsBodyRectangle((Vector2){screenWidth + 4020, (float)screenHeight - 260}, 150, 200, 30);
    PhysicsBody abox = CreatePhysicsBodyRectangle((Vector2){screenWidth + 400, (float)screenHeight - 210}, 70, 260, 30);
    PhysicsBody abox2 = CreatePhysicsBodyRectangle((Vector2){screenWidth + 600, (float)screenHeight - 210}, 70, 260, 30);
   



    

    // Disable dynamics to floor and walls physics bodies
    aplataforma->enabled = false;
    aplataforma2->enabled = false;
    aplataforma3->enabled = false;
    aplataforma4->enabled = false;
    aplataforma7->enabled = false;
    aplataforma9->enabled = false;
    afloor->enabled = false;
    api->enabled = false;
    acaixa->enabled = false;
    acaixa2->enabled = false;
    abloco->enabled = false;
    abox->enabled = false;
    abox2->enabled = false;
    
    
    //floor2->enabled = false;

    int isShortened = 0;
    int timeElapsed = 0;

    // Create movement physics body
    PhysicsBody body = CreatePhysicsBodyRectangle((Vector2){screenWidth / 2.0f, screenHeight / 2.0f}, 50, 50, 1);
    PhysicsBody follower = CreatePhysicsBodyRectangle((Vector2){screenWidth / 2.0f, screenHeight / 2.0f}, 50, 50, 1);
    body->freezeOrient = true; // Constrain body rotation to avoid little collision torque amounts
    follower->freezeOrient = true;

    Camera2D camera = {0};
    camera.target = body->position;
    camera.offset = (Vector2){screenWidth / 2.0f, screenHeight / 2.0f};
    camera.rotation = 0.0f;
    camera.zoom = 0.5f;

    SetTargetFPS(60);
    //Animations
    float runnerWidth = (float)runner.width/6.0;
    int runnerFrames = 6;
    float runnertimer= 0.0f;
    int runnerFrame = 0;

    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        mousePoint = GetMousePosition();
        btnAction = false;

        // Check button state
        if (CheckCollisionPointRec(mousePoint, btnBounds))
        {
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                btnState = 2;
            else
                btnState = 1;

            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                btnAction = true;
        }
        else
            btnState = 0;

        // Calculate button frame rectangle to draw depending on button state
        sourceRec.y = btnState * frameHeight;
        //----------------------------------------------------------------------------------

        //Check if button is clicked
        if (btnAction)
        {
            PlaySound(fxButton);
            UnloadTexture(button); // Unload button texture

            //The game
            while (!WindowShouldClose()) // Detect window close button or ESC key
            {
                camera.target = body->position;
                // Update
                //----------------------------------------------------------------------------------
                UpdatePhysics(); // Update physics system
                timeElapsed++;
                if(isShortened && timeElapsed % 50 == 0){
                    aumentar(&body);
                    isShortened = 0;
                    timeElapsed = 0;
                }

                if (IsKeyPressed(KEY_R)) // Reset physics input
                {
                    // Reset movement physics body position, velocity and rotation
                    body->position = (Vector2){screenWidth / 2.0f, screenHeight / 2.0f};
                    body->velocity = (Vector2){0, 0};
                    SetPhysicsBodyRotation(body, 0);
                }

                if(IsKeyPressed(KEY_DOWN) && !isShortened && body->isGrounded){
                    abaixar(&body);
                    isShortened = 1;                   
                }

                follower->position = (Vector2){body->position.x - 500, body->position.y};

                // Horizontal movement input
                body->velocity.x = VELOCITY;
                follower->velocity.x = VELOCITY;
                
                if(isShortened == 1 && body->isGrounded){
                    body->velocity.x = VELOCITY/1.5;
                }

                // Vertical movement input checking if player physics body is grounded
                if (IsKeyDown(KEY_SPACE) && body->isGrounded)
                    body->velocity.y = -VELOCITY * 4;
                //----------------------------------------------------------------------------------

                // Draw
                //----------------------------------------------------------------------------------
                BeginDrawing();

                BeginMode2D(camera);

                ClearBackground(BLACK);

                DrawFPS(screenWidth - 90, screenHeight - 30);

                // Draw created physics bodies
                int bodiesCount = GetPhysicsBodiesCount();
                for (int i = 0; i < bodiesCount; i++)
                {
                    PhysicsBody body = GetPhysicsBody(i);

                    int vertexCount = GetPhysicsShapeVerticesCount(i);
                    for (int j = 0; j < vertexCount; j++)
                    {
                        // Get physics bodies shape vertices to draw lines
                        // Note: GetPhysicsShapeVertex() already calculates rotation transformations
                        Vector2 vertexA = GetPhysicsShapeVertex(body, j);

                        int jj = (((j + 1) < vertexCount) ? (j + 1) : 0); // Get next vertex or first to close the shape
                        Vector2 vertexB = GetPhysicsShapeVertex(body, jj);

                        DrawLineV(vertexA, vertexB, GREEN); // Draw a line between two vertex positions
                    }
                }
                    
                runnertimer += GetFrameTime();

                if(runnertimer >= 0.2f){
                    runnertimer = 0.0f;
                    runnerFrame +=1;
                }

                runnerFrame = runnerFrame % runnerFrames;
                sourceRecRunner.x = runnerWidth * runnerFrame;

                DrawTextureRec(runner, sourceRecRunner, (Vector2){body->position.x - 40, body->position.y - 100}, WHITE);

                DrawText("Use 'ARROWS' to move player", 10, 10, 10, WHITE);
                DrawText("Press 'R' to reset example", 10, 30, 10, WHITE);

                EndDrawing();
                //----------------------------------------------------------------------------------
            }
            ClosePhysics();       // Unitialize physics

            CloseAudioDevice(); // Close audio device

            CloseWindow(); // Close window and OpenGL context
        }
        else
        {

            // Draw
            //----------------------------------------------------------------------------------
            BeginDrawing();

            ClearBackground(RAYWHITE);
            DrawTexture(texture, 0, 0, WHITE);

            DrawTextureRec(button, sourceRec, (Vector2){btnBounds.x, btnBounds.y}, WHITE); // Draw button frame
            DrawText("INICIAR O JOGO", 200, 100, 50, WHITE);

            //DrawText("Espaço: Pular", 300, 100, 50, WHITE);

            EndDrawing();
            //----------------------------------------------------------------------------------
        }
    }

    CloseAudioDevice(); // Close audio device
    UnloadTexture(texture);
    UnloadTexture(button);
    UnloadTexture(runner);
    ClosePhysics();


    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
