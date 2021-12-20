//Includes
#include "raylib.h"
#define NUM_FRAMES 3
#define PHYSAC_IMPLEMENTATION
#include "extras/physac.h"
#include <stdio.h>
#include <stdlib.h>

#define VELOCITY 0.5f
#define screenWidth 800
#define screenHeight 450

const float pitch = 1.0f;
float timePlayed = 0.0f;

Camera2D camera = {0};

//Basic Movement Functions

void abaixar(PhysicsBody *body)
{
    Vector2 position = (*body)->position;
    int isGrounded = (*body)->isGrounded;
    DestroyPhysicsBody(*body);

    *body = CreatePhysicsBodyRectangle(position, 50, 40, 1);
    (*body)->isGrounded = isGrounded;
}

void aumentar(PhysicsBody *body)
{
    Vector2 position = (*body)->position;
    int isGrounded = (*body)->isGrounded;
    DestroyPhysicsBody(*body);

    *body = CreatePhysicsBodyRectangle(position, 50, 80, 1);
    (*body)->isGrounded = isGrounded;
}

void setCamera(PhysicsBody body)
{
    camera.target = body->position;
    camera.offset = (Vector2){screenWidth / 2.0f, screenHeight / 2.0f + 50};
    camera.rotation = 0.0f;
    camera.zoom = 0.5f;
}

void setMusic(Music music, float *timePlayed)
{
    PlayMusicStream(music);
    SetMusicVolume(music, (float)0.2);
    music.looping = true;
    *timePlayed = 0.0f;
}

void playMusic(Music music, float *timePlayed){
    UpdateMusicStream(music);
    SetMusicPitch(music, pitch);
    *timePlayed = GetMusicTimePlayed(music) / GetMusicTimeLength(music) * (screenWidth - 40);
}

char *mostrarTempo(float tempoRestante)
{
    int minutos = tempoRestante / 60;
    int segundos = ((int)tempoRestante) % 60;
    char *texto = (char *)calloc(20, sizeof(char));
    if (texto == NULL)
        exit(1);

    sprintf(texto, "0%d:%d", minutos, segundos);

    return texto;
}

//Transitions

void initialScreen(Sound fxButton)
{
    Texture2D background = LoadTexture("resources/img/unknown.png"); // background texture
    Texture2D button = LoadTexture("resources/img/btn-bg.png"); // Load button texture
    Music theme = LoadMusicStream("resources/musica_inicial.mp3");
    setMusic(theme, &timePlayed);

    float frameHeight = (float)button.height / NUM_FRAMES;
    Rectangle sourceRec = {0, 0, (float)button.width, frameHeight};

    int btnState = 0;
    bool btnAction = false;

    Vector2 mousePoint = {0.0f, 0.0f};

    Rectangle btnBounds = {screenWidth / 2.0f - button.width / 2.0f, screenHeight / 2.0f - button.height / NUM_FRAMES / 2.0f, (float)button.width, frameHeight};

    while (!WindowShouldClose() && !btnAction)
    {
        playMusic(theme, &timePlayed);
        mousePoint = GetMousePosition();
        btnAction = false;
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
        sourceRec.y = btnState * frameHeight;

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawTexture(background, 0, 0, WHITE);
        DrawTextureRec(button, sourceRec, (Vector2){btnBounds.x, btnBounds.y}, WHITE); // Draw button frame
        DrawText("SEJA BEM-VINDO AO", 225, 100, 30, WHITE);
        DrawText("DANIEL PASSA A GENTE!", 200, 150, 30, WHITE);
        //DrawText("Espaço: Pular", 300, 100, 50, WHITE);
        EndDrawing();
    }

    if (btnAction)
        PlaySound(fxButton);

    UnloadTexture(button);
    UnloadTexture(background);
    UnloadMusicStream(theme);
    //UnloadSound(fxButton);  n dá para dar unload aqui, pois o som acaba antes da gente ouvir
}

void gameOver()
{
    Sound mario = LoadSound("resources/mario.wav");
    PlaySound(mario);
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawText("VOCE PERDEU", 165, 130, 75, WHITE);
        DrawText("Acm não efetuou o pagamento a tempo\nAGORA NAO TEMOS MAIS PROFESSOR DE IP", 210, 320, 25, WHITE);
        EndDrawing();
    }
    UnloadSound(mario);
    CloseAudioDevice();
    CloseWindow();
}

void congratulations()
{
    Texture2D daniel = LoadTexture("resources/personagens/dan-.png");
    Sound ratinho = LoadSound("resources/ratinho.wav");
    PlaySound(ratinho);
    while (!WindowShouldClose()) //tela de transição
    {
        DrawTexture(daniel, -30, 130, WHITE);

        BeginDrawing();
        ClearBackground(BLACK);
        DrawText("CONGRATULATIONS!!", 160, 85, 45, WHITE);
        DrawText("O PAGAMENTO FOI FEITO", 190, 200, 30, WHITE);
        DrawText("COM SUCESSO!!", 280, 230, 30, YELLOW);
        DrawText("O AGIOTA ZE DANIEL ESTA SATISFEITO", 140, 330, 25, WHITE);
        EndDrawing();
    }
    UnloadSound(ratinho);
    UnloadTexture(daniel);
}

void preGame(char *message, Sound somBotao)
{
    Music transicao = LoadMusicStream("resources/musica_transicao.mp3");
    setMusic(transicao, &timePlayed);
    int framesCounter = 0;

    while (!WindowShouldClose() && !IsKeyPressed(KEY_ENTER))
    {
        playMusic(transicao, &timePlayed);

        if (IsKeyDown(KEY_SPACE))
            framesCounter += 8;
        else
            framesCounter += 5;

        BeginDrawing();

        ClearBackground(BLACK);

        DrawText(TextSubtext(message, 0, framesCounter / 10), 30, 50, 23.12, MAROON);
        DrawText("[ESPAÇO] DAR O GAS E [ENTER] PARA JOGAR!", 171, 300, 20, LIGHTGRAY);

        EndDrawing();
    }
    UnloadMusicStream(transicao);
    UnloadSound(somBotao); //fechando nessa função para dar tempo de tocar o som
}

//phases

int faseUm()
{
    InitPhysics();

    PhysicsBody plataforma = CreatePhysicsBodyRectangle((Vector2){screenWidth + 1600, (float)screenHeight - 150}, 100, 20, 10);
    PhysicsBody plataforma2 = CreatePhysicsBodyRectangle((Vector2){screenWidth + 1800, (float)screenHeight - 300}, 100, 20, 10);
    PhysicsBody plataforma3 = CreatePhysicsBodyRectangle((Vector2){screenWidth + 2000, (float)screenHeight}, 100, 20, 10);
    PhysicsBody floor = CreatePhysicsBodyRectangle((Vector2){screenWidth / 2.0f, (float)screenHeight}, (float)screenWidth * 2 + 2000, 100, 10);
    PhysicsBody floor2 = CreatePhysicsBodyRectangle((Vector2){screenWidth + 3300, (float)screenHeight + 100}, 2000, 100, 10);
    PhysicsBody caixa = CreatePhysicsBodyRectangle((Vector2){screenWidth / 2.0f + 1000, (float)screenHeight - 100}, 100, 100, 10);
    PhysicsBody caixa2 = CreatePhysicsBodyRectangle((Vector2){screenWidth / 2.0f + 500, (float)screenHeight - 100}, 100, 100, 10);
    PhysicsBody caixa3 = CreatePhysicsBodyRectangle((Vector2){screenWidth + 2600, (float)screenHeight}, 100, 100, 10);
    PhysicsBody caixa4 = CreatePhysicsBodyRectangle((Vector2){screenWidth + 3600, (float)screenHeight}, 100, 100, 10);
    PhysicsBody taxi = CreatePhysicsBodyRectangle((Vector2){screenWidth / 2.0f + 1500, (float)screenHeight - 250}, 50, 300, 10);
    PhysicsBody taxi2 = CreatePhysicsBodyRectangle((Vector2){screenWidth + 2900, (float)screenHeight - 150}, 50, 300, 10);
    PhysicsBody taxi3 = CreatePhysicsBodyRectangle((Vector2){screenWidth + 3200, (float)screenHeight - 150}, 50, 300, 10);
    PhysicsBody taxi4 = CreatePhysicsBodyRectangle((Vector2){screenWidth + 3800, (float)screenHeight - 150}, 50, 300, 10);

    taxi->enabled = false;
    taxi2->enabled = false;
    taxi3->enabled = false;
    taxi4->enabled = false;
    floor->enabled = false;
    floor2->enabled = false;
    plataforma->enabled = false;
    plataforma2->enabled = false;
    plataforma3->enabled = false;
    caixa->enabled = false;
    caixa2->enabled = false;
    caixa3->enabled = false;
    caixa4->enabled = false;

    Texture2D box = LoadTexture("resources/img/caixote.png");
    Texture2D background = LoadTexture("resources/img/city-5.png");
    Texture2D ground = LoadTexture("resources/img/floor12.png");
    Texture2D taxiTexture = LoadTexture("resources/img/taxi.png");
    Texture2D runner = LoadTexture("resources/personagens/runner9.png"); // Runner texture
    Texture2D ufo = LoadTexture("resources/img/ufo.png");
    Texture2D heart = LoadTexture("resources/img/heart.png");

    int isShortened = 0;
    int timeElapsed = 0;
    int timeSlide = 0;
    float timeRemaining;
    float timeFase1 = 40.0f;
    char *timer = NULL;
    int lifes = 3;

    Rectangle sourceRecRunner = {0, 0, (float)runner.width / 11.25, (float)runner.height};

    //Animations
    float runnerWidth = (float)runner.width / 10.45;
    int runnerFrames = 6;
    float runnertimer = 0.0f;
    int runnerFrame = 0;

    PhysicsBody body = CreatePhysicsBodyRectangle((Vector2){screenWidth / 2.0f, screenHeight / 2.0f}, 50, 80, 1);
    body->freezeOrient = true;

    setCamera(body);

  //  Music theme = LoadMusicStream("resources/musica_inicial.mp3");

   // playMusic(theme);

    while (!WindowShouldClose())
    {
        if (timeElapsed == 0)
            timeRemaining = timeFase1;

        camera.target = body->position;

        if (lifes == 0 || ((int)timeRemaining) % 60 == 0)
        {
            free(timer);
            UnloadTexture(runner);
            UnloadTexture(ufo);
            UnloadTexture(background);
            UnloadTexture(ground);
            UnloadTexture(box);
            UnloadTexture(taxiTexture);

            int bodiesCount = GetPhysicsBodiesCount();
            for (int i = 0; i < bodiesCount; i++)
                DestroyPhysicsBody(GetPhysicsBody(i));

            ClosePhysics();

            return 0;
        }

        UpdatePhysics();
        timeElapsed++;

        //Abaixar
        if (IsKeyPressed(KEY_DOWN) && body->isGrounded)
        {
            abaixar(&body);
            isShortened = 1;
        }

        //Pular
        if (IsKeyDown(KEY_SPACE) && body->isGrounded)
            body->velocity.y = -VELOCITY * 4;

        if (isShortened)
        {
            body->velocity.x = VELOCITY / 1.5;
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
            lifes--;
        }

        if (body->position.x > screenWidth + 4300)
        {
            free(timer);
            UnloadTexture(runner);
            UnloadTexture(ufo);
            UnloadTexture(background);
            UnloadTexture(ground);
            UnloadTexture(box);
            UnloadTexture(taxiTexture);

            int bodiesCount = GetPhysicsBodiesCount();
            for (int i = 0; i < bodiesCount; i++)
                DestroyPhysicsBody(GetPhysicsBody(i));

            ClosePhysics();

            return 1;
        }

        BeginDrawing();

        BeginMode2D(camera);

        ClearBackground(GetColor(0x052c46ff));

        DrawTexture(background, body->position.x - 800, -(float)screenHeight - 250, WHITE);
        DrawTexture(ground, screenWidth / 2.0f - ((float)screenWidth * 2 + 2000) / 2, (float)screenHeight - 50, WHITE);
        DrawTexture(ground, screenWidth + 2300, (float)screenHeight + 50, WHITE);
        DrawTexture(box, screenWidth / 2.0f + 950, (float)screenHeight - 150, WHITE);
        DrawTexture(box, screenWidth + 3550, (float)screenHeight - 50, WHITE);
        DrawTexture(box, screenWidth + 2550, (float)screenHeight - 50, WHITE);
        DrawTexture(box, screenWidth / 2.0f + 450, (float)screenHeight - 150, WHITE);
        DrawTexture(ufo, screenWidth + 1550, (float)screenHeight - 190, WHITE);
        DrawTexture(ufo, screenWidth + 1750, (float)screenHeight - 340, WHITE);
        DrawTexture(ufo, screenWidth + 1950, (float)screenHeight - 40, WHITE);

        DrawTexture(taxiTexture, screenWidth / 2.0f + 1345, (float)screenHeight - 400 - 180, WHITE);
        DrawTexture(taxiTexture, screenWidth + 2745, (float)screenHeight - 300 - 180, WHITE);
        DrawTexture(taxiTexture, screenWidth + 3045, (float)screenHeight - 300 - 180, WHITE);
        DrawTexture(taxiTexture, screenWidth + 3645, (float)screenHeight - 300 - 180, WHITE);

        for (int i = 0; i < lifes; i++)
            DrawTexture(heart, body->position.x - 800 + (i + 1) * 50, body->position.y - 550, WHITE);

        Font fontStart = LoadFont("resources/fonts/mecha.png");
        if (timeElapsed <= 30)
        {
            DrawTextEx(fontStart, "3", (Vector2){screenWidth / 2, screenHeight / 2 - 150}, 110, 0.0f, YELLOW);
        }
        else if (timeElapsed <= 60)
        { //2
            DrawTextEx(fontStart, "2", (Vector2){screenWidth / 2, screenHeight / 2 - 150}, 110, 0.0f, YELLOW);
        }
        else if (timeElapsed <= 90)
        { //1
            DrawTextEx(fontStart, "1", (Vector2){screenWidth / 2, screenHeight / 2 - 150}, 110, 0.0f, YELLOW);
        }
        else
        {
            body->velocity.x = VELOCITY;
            timeRemaining -= GetFrameTime();
            timer = mostrarTempo(timeRemaining);
            DrawTextEx(fontStart, "START", (Vector2){screenWidth / 2, screenHeight / 2 - 150}, 95, 1.5f, YELLOW);
            DrawText(timer, body->position.x + 650, body->position.y + 250, 50, YELLOW);
        }

        //Animation load
        runnertimer += GetFrameTime();

        if (runnertimer >= 0.2f)
        {
            runnertimer = 0.0f;
            runnerFrame += 1;
        }

        runnerFrame = runnerFrame % runnerFrames;
        if (isShortened)
        {
            sourceRecRunner.width = (float)runner.width / 9.0;
            sourceRecRunner.x = runnerWidth * 6;
            DrawTextureRec(runner, sourceRecRunner, (Vector2){body->position.x - 40, body->position.y - 95}, WHITE);
        }
        else
        {
            sourceRecRunner.width = (float)runner.width / 11.2;

            if (body->velocity.y < 0)
            {
                sourceRecRunner.x = runnerWidth * 7.2;
            }
            else if (!body->isGrounded)
            {
                sourceRecRunner.x = runnerWidth * 8.4;
            }
            else
            {
                sourceRecRunner.x = runnerWidth * runnerFrame;
            }

            DrawTextureRec(runner, sourceRecRunner, (Vector2){body->position.x - 40, body->position.y - 90}, WHITE);
        }
        EndDrawing();
    }

    free(timer);
    UnloadTexture(runner);
    UnloadTexture(ufo);
    UnloadTexture(background);
    UnloadTexture(ground);
    UnloadTexture(box);
    UnloadTexture(taxiTexture);

    int bodiesCount = GetPhysicsBodiesCount();
    for (int i = 0; i < bodiesCount; i++)
        DestroyPhysicsBody(GetPhysicsBody(i));

    CloseAudioDevice();
    ClosePhysics();
    CloseWindow();
    return 0;
}

int faseDois()
{
    InitPhysics();

    PhysicsBody afloor = CreatePhysicsBodyRectangle((Vector2){screenWidth / 2.0f, (float)screenHeight}, (float)screenWidth * 2 + 2000, 100, 10);
    PhysicsBody aplataforma = CreatePhysicsBodyRectangle((Vector2){screenWidth + 1600, (float)screenHeight - 150}, 100, 20, 10);
    PhysicsBody aplataforma2 = CreatePhysicsBodyRectangle((Vector2){screenWidth + 1850, (float)screenHeight - 180}, 80, 20, 10);
    PhysicsBody aplataforma3 = CreatePhysicsBodyRectangle((Vector2){screenWidth + 2050, (float)screenHeight - 210}, 60, 20, 10);
    PhysicsBody aplataforma4 = CreatePhysicsBodyRectangle((Vector2){screenWidth + 2250, (float)screenHeight - 240}, 40, 20, 10);
    PhysicsBody aplataforma7 = CreatePhysicsBodyRectangle((Vector2){screenWidth + 3300, (float)screenHeight - 180}, 120, 20, 30);
    PhysicsBody aplataforma9 = CreatePhysicsBodyRectangle((Vector2){screenWidth + 3950, (float)screenHeight - 120}, 800, 20, 30);
    PhysicsBody api = CreatePhysicsBodyRectangle((Vector2){screenWidth + 2750, (float)screenHeight - 240}, 600, 20, 10);
    PhysicsBody abloco = CreatePhysicsBodyRectangle((Vector2){screenWidth + 2750, (float)screenHeight - 400}, 50, 200, 10);
    PhysicsBody acaixa = CreatePhysicsBodyRectangle((Vector2){screenWidth + 4200, (float)screenHeight - 280}, 50, 200, 30);
    PhysicsBody acaixa2 = CreatePhysicsBodyRectangle((Vector2){screenWidth + 3920, (float)screenHeight - 280}, 50, 200, 30);
    PhysicsBody abox = CreatePhysicsBodyRectangle((Vector2){screenWidth + 600, (float)screenHeight - 240}, 70, 260, 30);
    PhysicsBody abox2 = CreatePhysicsBodyRectangle((Vector2){screenWidth + 1000, (float)screenHeight - 240}, 70, 260, 30);
    PhysicsBody abox3 = CreatePhysicsBodyRectangle((Vector2){screenWidth / 2.0f + ((float)screenWidth * 2 + 2000)/2 - 50, (float)screenHeight - 100}, 100, 100, 30);

    abox->enabled = false;
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
    abox3->enabled = false;

    Texture2D runner = LoadTexture("resources/personagens/runner.png");
    Texture2D heart = LoadTexture("resources/img/heart.png");
    Texture2D jungle = LoadTexture("resources/img/selvamaismaiscerta.png"); //background da jungle
    Texture2D arvore = LoadTexture("resources/img/macaconaarvore2.png");
    Texture2D chao = LoadTexture("resources/img/chao-de-grama.png");
    Texture2D bush = LoadTexture("resources/img/moita.png");

    int isShortened = 0;
    int timeElapsed = 0;
    int timeSlide = 0;
    float timeRemaining;
    float timeFase1 = 40.0f;
    char *timer = NULL;
    int lifes = 3;

    Rectangle sourceRecRunner = {0, 0, (float)runner.width / 11.25, (float)runner.height};

    //Animations
    float runnerWidth = (float)runner.width / 10.45;
    int runnerFrames = 6;
    float runnertimer = 0.0f;
    int runnerFrame = 0;

    int bodiesCount = GetPhysicsBodiesCount();

    PhysicsBody body = CreatePhysicsBodyRectangle((Vector2){screenWidth / 2.0f, screenHeight / 2.0f}, 50, 80, 1);
    body->freezeOrient = true;

    setCamera(body);

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

    while (!WindowShouldClose())
    {
        if (timeElapsed == 0)
            timeRemaining = timeFase1;

        camera.target = body->position;

        if (lifes == 0 || ((int)timeRemaining) % 60 == 0)
        {
            free(timer);
            UnloadTexture(runner);

            int bodiesCount = GetPhysicsBodiesCount();
            for (int i = 0; i < bodiesCount; i++)
                DestroyPhysicsBody(GetPhysicsBody(i));

            ClosePhysics();

            return 0;
        }

        UpdatePhysics();
        timeElapsed++;

        //Abaixar
        if (IsKeyPressed(KEY_DOWN) && body->isGrounded)
        {
            abaixar(&body);
            isShortened = 1;
        }

        //Pular
        if (IsKeyDown(KEY_SPACE) && body->isGrounded)
            body->velocity.y = -VELOCITY * 4;

        if (isShortened)
        {
            body->velocity.x = VELOCITY / 1.5;
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
            lifes--;
        }

        if (body->position.x > screenWidth + 4300)
        {
            free(timer);
            UnloadTexture(runner);

            int bodiesCount = GetPhysicsBodiesCount();
            for (int i = 0; i < bodiesCount; i++)
                DestroyPhysicsBody(GetPhysicsBody(i));

            ClosePhysics();

            return 1;
        }

        BeginDrawing();

        BeginMode2D(camera);

        ClearBackground(GetColor(0x052c46ff));
        
        DrawTexture(jungle, body->position.x - 800, -(float)screenHeight - 100, WHITE);
        DrawTexture(arvore, screenWidth + 500, (float)screenHeight - 450, WHITE);
        DrawTexture(arvore, screenWidth + 900, (float)screenHeight - 450, WHITE);
        DrawTexture(bush,  screenWidth / 2.0f + ((float)screenWidth * 2 + 2000)/2 - 150, (float)screenHeight - 200, WHITE);
        //DrawTexture(chao, screenWidth / 2.0f, (float)screenHeight, WHITE);
        
        /*PhysicsBody afloor = CreatePhysicsBodyRectangle((Vector2){screenWidth / 2.0f, (float)screenHeight}, (float)screenWidth * 2 + 2000, 100, 10)
    PhysicsBody aplataforma = CreatePhysicsBodyRectangle((Vector2){screenWidth + 1600, (float)screenHeight - 150}, 100, 20, 10);
    PhysicsBody aplataforma2 = CreatePhysicsBodyRectangle((Vector2){screenWidth + 1850, (float)screenHeight - 180}, 80, 20, 10);
    PhysicsBody aplataforma3 = CreatePhysicsBodyRectangle((Vector2){screenWidth + 2050, (float)screenHeight - 210}, 60, 20, 10);
    PhysicsBody aplataforma4 = CreatePhysicsBodyRectangle((Vector2){screenWidth + 2250, (float)screenHeight - 240}, 40, 20, 10);
    PhysicsBody aplataforma7 = CreatePhysicsBodyRectangle((Vector2){screenWidth + 3300, (float)screenHeight - 180}, 120, 20, 30);
    PhysicsBody aplataforma9 = CreatePhysicsBodyRectangle((Vector2){screenWidth + 3950, (float)screenHeight - 120}, 800, 20, 30);
    PhysicsBody api = CreatePhysicsBodyRectangle((Vector2){screenWidth + 2750, (float)screenHeight - 240}, 600, 20, 10);
    PhysicsBody abloco = CreatePhysicsBodyRectangle((Vector2){screenWidth + 2750, (float)screenHeight - 400}, 50, 200, 10);
    PhysicsBody acaixa = CreatePhysicsBodyRectangle((Vector2){screenWidth + 4200, (float)screenHeight - 280}, 50, 200, 30);
    PhysicsBody acaixa2 = CreatePhysicsBodyRectangle((Vector2){screenWidth + 3920, (float)screenHeight - 280}, 50, 200, 30);
    PhysicsBody abox = CreatePhysicsBodyRectangle((Vector2){screenWidth + 600, (float)screenHeight - 240}, 70, 260, 30);
    PhysicsBody abox2 = CreatePhysicsBodyRectangle((Vector2){screenWidth + 1000, (float)screenHeight - 240}, 70, 260, 30);
    PhysicsBody abox3 = CreatePhysicsBodyRectangle((Vector2){screenWidth / 2.0f + ((float)screenWidth * 2 + 2000)/2 - 50, (float)screenHeight - 100}, 100, 100, 30);*/
        
        

        for (int i = 0; i < lifes; i++)
            DrawTexture(heart, body->position.x - 800 + (i + 1) * 50, body->position.y - 550, WHITE);

        Font fontStart = LoadFont("resources/fonts/mecha.png");
        if (timeElapsed <= 30)
        {
            DrawTextEx(fontStart, "3", (Vector2){screenWidth / 2, screenHeight / 2 - 150}, 110, 0.0f, YELLOW);
        }
        else if (timeElapsed <= 60)
        { //2
            DrawTextEx(fontStart, "2", (Vector2){screenWidth / 2, screenHeight / 2 - 150}, 110, 0.0f, YELLOW);
        }
        else if (timeElapsed <= 90)
        { //1
            DrawTextEx(fontStart, "1", (Vector2){screenWidth / 2, screenHeight / 2 - 150}, 110, 0.0f, YELLOW);
        }
        else
        {
            body->velocity.x = VELOCITY;
            timeRemaining -= GetFrameTime();
            timer = mostrarTempo(timeRemaining);
            DrawTextEx(fontStart, "START", (Vector2){screenWidth / 2, screenHeight / 2 - 150}, 95, 1.5f, YELLOW);
            DrawText(timer, body->position.x + 650, body->position.y + 250, 50, YELLOW);
        }

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

        //Animation load
        runnertimer += GetFrameTime();

        if (runnertimer >= 0.2f)
        {
            runnertimer = 0.0f;
            runnerFrame += 1;
        }

        runnerFrame = runnerFrame % runnerFrames;
        if (isShortened)
        {
            sourceRecRunner.width = (float)runner.width / 9.0;
            sourceRecRunner.x = runnerWidth * 6;
            DrawTextureRec(runner, sourceRecRunner, (Vector2){body->position.x - 40, body->position.y - 95}, WHITE);
        }
        else
        {
            sourceRecRunner.width = (float)runner.width / 11.2;

            if (body->velocity.y < 0)
            {
                sourceRecRunner.x = runnerWidth * 7.2;
            }
            else if (!body->isGrounded)
            {
                sourceRecRunner.x = runnerWidth * 8.4;
            }
            else
            {
                sourceRecRunner.x = runnerWidth * runnerFrame;
            }

            DrawTextureRec(runner, sourceRecRunner, (Vector2){body->position.x - 40, body->position.y - 90}, WHITE);
        }
        EndDrawing();
    }

    free(timer);
    UnloadTexture(runner);

    bodiesCount = GetPhysicsBodiesCount();
    for (int i = 0; i < bodiesCount; i++)
        DestroyPhysicsBody(GetPhysicsBody(i));

    CloseAudioDevice();
    ClosePhysics();
    CloseWindow();
    return 0;
}

//main

int main()
{
    SetConfigFlags(FLAG_MSAA_4X_HINT); // NOTE: Try to enable MSAA 4X
    InitWindow(screenWidth, screenHeight, "Daniel dá nota pra nois");
    InitAudioDevice(); // Initialize audio device
    Sound fxButton = LoadSound("resources/img/buttonfx.wav");

    SetTargetFPS(60);

    initialScreen(fxButton);

    preGame("ACM se encontra em apuros e precisa de sua ajuda.\nO agiota Ze Daniel perdeu a calma com os atrasos do pagamento\ne se encontra cobrando o imediato pagamento.\nGuie ACM e efetue o pagamento antes que o tempo acabe.\nCuidado, o caminho consegue ser tortuoso e incerto.\n", fxButton);

    /*if (!faseUm())
       gameOver();*/
    if (!faseDois())
        gameOver();
    else congratulations();

    congratulations(); 
    CloseAudioDevice();
    CloseWindow();
}