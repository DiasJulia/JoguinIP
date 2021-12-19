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

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    SetConfigFlags(FLAG_MSAA_4X_HINT);  // NOTE: Try to enable MSAA 4X

    InitWindow(screenWidth, screenHeight, "teste do jogo / testando");

    InitAudioDevice(); // Initialize audio device

    Sound fxButton = LoadSound("resources/buttonfx.wav");     // Load button sound
    Texture2D button = LoadTexture("resources/btn-bg.png");   // Load button texture
    Texture2D texture = LoadTexture("resources/unknown.png"); // background texture
    Texture2D runner = LoadTexture("resources/personagens/runner9.png"); // Runner texture
    Texture2D caixote = LoadTexture("resources/caixote.png");

    Music theme = LoadMusicStream("resources/musica_inicial.mp3");
    theme.looping = true;
    float pitch = 1.0f;
    Music music = LoadMusicStream("resources/musiquinha.mp3");
    music.looping = true;
    pitch = 1.0f;

    Rectangle sourceRecRunner = {0, 0, (float)runner.width / 11.2, (float)runner.height};

    // Define frame rectangle for drawing
    float frameHeight = (float)button.height / NUM_FRAMES;
    Rectangle sourceRec = {0, 0, (float)button.width, frameHeight};

    // Define button bounds on screen
    Rectangle btnBounds = {screenWidth / 2.0f - button.width / 2.0f, screenHeight / 2.0f - button.height / NUM_FRAMES / 2.0f, (float)button.width, frameHeight};

    int btnState = 0;       // Button state: 0-NORMAL, 1-MOUSE_HOVER, 2-PRESSED
    bool btnAction = false; // Button action should be activated
    bool fase = 0; //responsável por terminar o loop de codigo quando a fase terminar
    bool botaoClicado = 0; //responsável por manter a tea inicial enquanto o botão n for clicado

    Vector2 mousePoint = {0.0f, 0.0f};

    //Game Configuration

    // Initialize physics and default physics bodies
    InitPhysics();
    
    //proporcao barreira e chao
    //em cima *2 + x
    //em baixo *1.5 + x/2

    // Create floor and walls rectangle physics body
    PhysicsBody floor = CreatePhysicsBodyRectangle((Vector2){screenWidth / 2.0f, (float)screenHeight}, (float)screenWidth * 2 + 2000, 100, 10);
    //PhysicsBody floor2 = CreatePhysicsBodyRectangle((Vector2){screenWidth / 2.0f + (float)screenWidth * 2 + 2000, (float)screenHeight}, (float)screenWidth * 2 + 1650, 100, 10);
    PhysicsBody plataforma = CreatePhysicsBodyRectangle((Vector2){screenWidth + 1600, (float)screenHeight - 150}, 100, 20, 10);
    PhysicsBody plataforma2 = CreatePhysicsBodyRectangle((Vector2){screenWidth + 1800, (float)screenHeight - 300}, 100, 20, 10);
    PhysicsBody plataforma3 = CreatePhysicsBodyRectangle((Vector2){screenWidth + 2000, (float)screenHeight}, 100, 20, 10);
    PhysicsBody floor2 = CreatePhysicsBodyRectangle((Vector2){screenWidth + 3300, (float)screenHeight + 100}, 2000, 100, 10);
    PhysicsBody caixa = CreatePhysicsBodyRectangle((Vector2){screenWidth / 2.0f + 1000, (float)screenHeight - 100}, 100, 100, 10);
    PhysicsBody caixa2 = CreatePhysicsBodyRectangle((Vector2){screenWidth / 2.0f + 500, (float)screenHeight - 100}, 100, 100, 10);
    PhysicsBody naoSeiONome = CreatePhysicsBodyRectangle((Vector2){screenWidth / 2.0f + 1500, (float)screenHeight - 225}, 50, 300, 10);
    PhysicsBody naoSeiONome2 = CreatePhysicsBodyRectangle((Vector2){screenWidth + 2900, (float)screenHeight - 125}, 50, 300, 10);
    PhysicsBody caixa3 = CreatePhysicsBodyRectangle((Vector2){screenWidth + 2600, (float)screenHeight}, 100, 100, 10);
    PhysicsBody naoSeiONome3 = CreatePhysicsBodyRectangle((Vector2){screenWidth + 3200, (float)screenHeight - 125}, 50, 300, 10);
    PhysicsBody caixa4 = CreatePhysicsBodyRectangle((Vector2){screenWidth + 3600, (float)screenHeight}, 100, 100, 10);
    PhysicsBody naoSeiONome4 = CreatePhysicsBodyRectangle((Vector2){screenWidth + 3800, (float)screenHeight - 75}, 50, 200, 10);

    // Disable dynamics to floor and walls physics bodies
    naoSeiONome4->enabled = false;
    caixa4->enabled = false;
    naoSeiONome3->enabled = false;
    caixa3->enabled = false;
    naoSeiONome2->enabled = false;
    floor2->enabled = false;
    plataforma->enabled = false;
    plataforma2->enabled = false;
    plataforma3->enabled = false;
    caixa2->enabled = false;
    naoSeiONome->enabled = false;
    caixa->enabled = false;
    floor->enabled = false;
    //floor2->enabled = false;

    int isShortened = 0;
    int timeElapsed = 0;
    int timeSlide = 0;

    // Create movement physics body
    PhysicsBody body = CreatePhysicsBodyRectangle((Vector2){screenWidth / 2.0f, screenHeight / 2.0f}, 50, 50, 1);
    PhysicsBody follower = CreatePhysicsBodyRectangle((Vector2){screenWidth / 2.0f, screenHeight / 2.0f}, 50, 50, 1);
    body->freezeOrient = true; // Constrain body rotation to avoid little collision torque amounts
    follower->freezeOrient = true;

    Camera2D camera = {0};
    camera.target = body->position;
    camera.offset = (Vector2){screenWidth / 2.0f, screenHeight / 2.0f + 50};
    camera.rotation = 0.0f;
    camera.zoom = 0.5f;

    SetTargetFPS(60);
    //Animations
    float runnerWidth = (float)runner.width / 10.4;
    int runnerFrames = 6;
    float runnertimer = 0.0f;
    int runnerFrame = 0;

    PlayMusicStream(theme);

    float timePlayed = 0.0f;
    bool pause = false;

    while (!botaoClicado) // Detect window close button or ESC key
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
            botaoClicado = 1;
            PlaySound(fxButton);
            UnloadTexture(button); // Unload button texture
            UnloadTexture(texture);
            UnloadMusicStream(theme);
            Texture2D cenario = LoadTexture("resources/cidade.png");
            Texture2D bricks = LoadTexture("resources/bricks.png");
            Texture2D plataforma = LoadTexture("resources/plataforma.png");

            PlayMusicStream(music);
            timePlayed = 0.0f;
            pause = false;

            //The game
            while (!WindowShouldClose())
            {
                UpdateMusicStream(music);
                SetMusicPitch(music, pitch);

                // Get timePlayed scaled to bar dimensions
                timePlayed = GetMusicTimePlayed(music)/GetMusicTimeLength(music)*(screenWidth - 40);

                camera.target = body->position;
                // Update
                //----------------------------------------------------------------------------------
                UpdatePhysics(); // Update physics system
                timeElapsed++;
                if (isShortened)
                {
                    timeSlide++;
                    if (timeSlide > 30)
                    {
                        aumentar(&body);
                        isShortened = 0;
                        timeSlide = 0;
                    }
                }

                if (body->position.y > (float)screenHeight + 2000) // Reset physics input
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
                
                if(body->position.x > screenWidth + 4300)
                    fase = 1;

                // Draw
                //----------------------------------------------------------------------------------
                BeginDrawing();
                
                BeginMode2D(camera);

                ClearBackground(BLACK);

                //desenhar as coisas do ambiente antes do personagem
                DrawTexture(cenario, -screenWidth / 2.0f, -screenHeight / 2.0f -250, WHITE);
                DrawTexture(caixote, screenWidth / 2.0f + 950, (float)screenHeight - 150, WHITE);
                DrawTexture(caixote, screenWidth + 3550, (float)screenHeight - 50, WHITE);
                DrawTexture(caixote, screenWidth + 2550, (float)screenHeight - 50, WHITE);
                DrawTexture(caixote, screenWidth / 2.0f + 450, (float)screenHeight - 150, WHITE);
                DrawTexture(bricks, screenWidth / 2.0f - ((float)screenWidth * 2 + 2000)/2, (float)screenHeight - 50, WHITE);
                DrawTexture(bricks, screenWidth + 2300, (float)screenHeight + 50, WHITE);
              
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

                        DrawLineV(vertexA, vertexB, BLANK); // Draw a line between two vertex positions
                    }
                }
                    
            runnertimer += GetFrameTime();

            if (runnertimer >= 0.2f)
            {
                runnertimer = 0.0f;
                runnerFrame += 1;
            }

            runnerFrame = runnerFrame % runnerFrames;
            if (isShortened)
            {
                sourceRecRunner.x = runnerWidth * 6;
                DrawTextureRec(runner, sourceRecRunner, (Vector2){body->position.x - 40, body->position.y - 100}, WHITE);
            }
            else
            {

            //    DrawText(TextFormat("Score: %i", body->velocity.y), 10, 10, 10, WHITE);
                if(body->velocity.y<0){
                    sourceRecRunner.x = runnerWidth * 7.2;
                } else
                if ( !body->isGrounded)
                {
                    sourceRecRunner.x = runnerWidth * 8.4;
                } else
                {
                    sourceRecRunner.x = runnerWidth * runnerFrame;
                }
                DrawTextureRec(runner, sourceRecRunner, (Vector2){body->position.x - 40, body->position.y - 100}, WHITE);
            }
           //     DrawText("Use 'ARROWS' to move player", 10, 10, 10, WHITE);
            //    DrawText("Press 'R' to reset example", 10, 30, 10, WHITE);

                EndDrawing();
                //----------------------------------------------------------------------------------
            }
        UnloadTexture(cenario);
        UnloadTexture(bricks);
        }
        else
        {   
            UpdateMusicStream(theme);

            SetMusicPitch(theme, pitch);

            // Get timePlayed scaled to bar dimensions
            timePlayed = GetMusicTimePlayed(theme)/GetMusicTimeLength(theme)*(screenWidth - 40);

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

    UnloadMusicStream(music);
    CloseAudioDevice(); // Close audio device
    UnloadTexture(runner);
    UnloadTexture(caixote);
    ClosePhysics();


    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}