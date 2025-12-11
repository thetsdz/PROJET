#include "C:/raylib/include/raylib.h"
#include "C:/raylib/include/raymath.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>

typedef struct {
    Vector3 pos;
    float width;
    float height;
    float depth;
    Color color;
} Block;

#define NUM_BLOCKS 10

// Fonction d'interpolation linéaire
float lerp(float a, float b, float t) {
    return a + t * (b - a);
}

int main(void) {
    // --- Initialisation de la fenêtre ---
    int screenWidth = GetMonitorWidth(0);
    int screenHeight = GetMonitorHeight(0);
    InitWindow(screenWidth, screenHeight, "JEU");
    ToggleFullscreen();
    SetTargetFPS(60);
    DisableCursor();
    srand(time(NULL));

    // --- Cube joueur ---
    Vector3 cubePos = {0, 0.5f, 0};
    float cubeSize = 1.0f;
    float normalHeight = 0.5f;
    float crouchHeight = 0.3f;
    float currentHeight = normalHeight;
    float crouchSpeed = 0.1f;

    // --- Orientation caméra ---
    float yaw = 0.0f;
    float pitch = 0.0f;
    float speed = 0.1f;
    float rotationSpeed = 0.03f;

    // --- Saut et gravité ---
    float velocityY = 0.0f;
    float gravity = 0.02f;
    bool onGround = true;
    float jumpStrength = 1f;

    // --- Cible rouge ---
    Vector3 ciblePos = {(float)(rand()%20-10), 0.5f, (float)(rand()%20-10)};
    float cibleRadius = 0.5f;
    int score = 0;

    // --- Blocs solides ---
    Block blocks[NUM_BLOCKS] = {
        {{2, 0.5f, 2}, 2, 1, 2, DARKGRAY},
        {{-3, 1.0f, 4}, 3, 1, 3, DARKGRAY},
        {{5, 1.5f, -2}, 2, 1, 2, DARKGRAY},
        {{-2, 2.0f, -3}, 4, 1, 2, DARKGRAY},
        {{0, 3.0f, 5}, 2, 1, 2, DARKGRAY},
        // Bordures
        {{0, 1.5f, -25}, 50, 4, 1, GRAY},
        {{0, 1.5f, 25}, 50, 4, 1, GRAY},
        {{-25, 1.5f, 0}, 1, 4, 50, GRAY},
        {{25, 1.5f, 0}, 1, 4, 50, GRAY},
        // Plafond invisible
        {{0, 10.0f, 0}, 50, 1, 50, Fade(GRAY, 0.0f)}
    };

    // --- Caméra 3D ---
    Camera3D camera = {0};
    camera.up = (Vector3){0, 1, 0};
    camera.fovy = 60;
    camera.projection = CAMERA_PERSPECTIVE;

    while(!WindowShouldClose()) {
        if(IsKeyPressed(KEY_ESCAPE)) break;

        // --- Rotation caméra ---
        if(IsKeyDown(KEY_LEFT))  yaw += rotationSpeed;
        if(IsKeyDown(KEY_RIGHT)) yaw -= rotationSpeed;
        Vector2 mouseDelta = GetMouseDelta();
        yaw += mouseDelta.x * 0.003f;
        pitch -= mouseDelta.y * 0.003f;
        if(pitch > 1.5f) pitch = 1.5f;
        if(pitch < -1.5f) pitch = -1.5f;
        Vector3 forward = {sinf(yaw), 0, cosf(yaw)};

        // --- Accroupissement fluide ---
        float targetHeight = normalHeight;
        if (IsKeyDown(KEY_LEFT_SHIFT) && onGround) {
            targetHeight = crouchHeight;
        }
        currentHeight = lerp(currentHeight, targetHeight, crouchSpeed);

        // --- Saut ---
        if(IsKeyPressed(KEY_SPACE) && onGround) {
            velocityY = jumpStrength;
            onGround = false;
        }
        velocityY -= gravity;

        // --- Déplacement tentative ---
        Vector3 nextPos = cubePos;
        if(IsKeyDown(KEY_UP)) {
            nextPos.x += forward.x * speed;
            nextPos.z += forward.z * speed;
        }
        if(IsKeyDown(KEY_DOWN)) {
            nextPos.x -= forward.x * speed;
            nextPos.z -= forward.z * speed;
        }
        nextPos.y += velocityY;

        // --- Collision avec blocs ---
        onGround = false;
        for(int i = 0; i < NUM_BLOCKS; i++) {
            Block b = blocks[i];
            float halfX = b.width/2, halfY = b.height/2, halfZ = b.depth/2;
            float playerHalf = cubeSize/2;
            bool collideX = nextPos.x + playerHalf > b.pos.x - halfX && nextPos.x - playerHalf < b.pos.x + halfX;
            bool collideY = nextPos.y + playerHalf > b.pos.y - halfY && nextPos.y - playerHalf < b.pos.y + halfY;
            bool collideZ = nextPos.z + playerHalf > b.pos.z - halfZ && nextPos.z - playerHalf < b.pos.z + halfZ;
            if(collideX && collideY && collideZ) {
                float top = b.pos.y + halfY;
                float bottom = b.pos.y - halfY;
                if(velocityY < 0 && cubePos.y - playerHalf >= top) {
                    nextPos.y = top + playerHalf;
                    velocityY = 0;
                    onGround = true;
                } else if(velocityY > 0 && cubePos.y + playerHalf <= bottom) {
                    nextPos.y = bottom - playerHalf;
                    velocityY = 0;
                } else {
                    nextPos.x = cubePos.x;
                    nextPos.z = cubePos.z;
                }
            }
        }

        // --- Mise à jour de la position Y ---
        if(nextPos.y <= currentHeight) {
            nextPos.y = currentHeight;
            velocityY = 0;
            onGround = true;
        }
        cubePos = nextPos;

        // --- Collision avec cible ---
        if(Vector3Distance(cubePos, ciblePos) <= cubeSize/2 + cibleRadius) {
            score++;
            ciblePos.x = (float)(rand()%20-10);
            ciblePos.z = (float)(rand()%20-10);
        }

        // --- Caméra ---
        Vector3 camForward = {sinf(yaw)*cosf(pitch), sinf(pitch), cosf(yaw)*cosf(pitch)};
        camera.position = (Vector3){cubePos.x, cubePos.y + 0.5f, cubePos.z};
        camera.target = Vector3Add(camera.position, camForward);

        // --- Rendu ---
        BeginDrawing();
        ClearBackground(SKYBLUE);
        BeginMode3D(camera);
            DrawPlane((Vector3){0, 0, 0}, (Vector2){50, 50}, GREEN);
            for(int i = 0; i < NUM_BLOCKS; i++) {
                DrawCube(blocks[i].pos, blocks[i].width, blocks[i].height, blocks[i].depth, blocks[i].color);
            }
            DrawCube(cubePos, cubeSize, cubeSize, cubeSize, BLUE);
            DrawSphere(ciblePos, cibleRadius, RED);
        EndMode3D();
        DrawText(TextFormat("Score: %d | FPS: %d", score, GetFPS()), 10, 10, 20, DARKGRAY);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}