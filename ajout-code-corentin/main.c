#include "raylib.h" 
#include "raymath.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

// Inclusion de nos propres modules
#include "types.h"
#include "level.h"
#include "player.h"
#include "projectile.h"

int main(void){
    // --- 1. Initialisation Fenêtre & Raylib ---
    int screenWidth = GetMonitorWidth(0);
    int screenHeight = GetMonitorHeight(0);
    InitWindow(screenWidth, screenHeight, "JEU REFACTORISE");
    ToggleFullscreen();
    SetTargetFPS(60);   // Essaye de maintenir 60 images/seconde
    DisableCursor();    // Bloque la souris dans la fenêtre pour la visée
    srand(time(NULL));  // Initialise le générateur aléatoire

    // --- 2. Initialisation des Objets ---
    Player player;
    InitPlayer(&player);

    Block blocks[NUM_BLOCKS];
    InitLevel(blocks);

    Projectile projs[MAX_PROJ];
    InitProjectiles(projs);

    // Setup de la cible rouge
    Vector3 ciblePos = {(float)(rand()%20-10), 0.5f, (float)(rand()%20-10)};
    float cibleRadius = 0.5f;
    int score = 0;

    // Setup Caméra Raylib standard
    Camera3D camera = {0};
    camera.up = (Vector3){0,1,0}; // L'axe Y pointe vers le haut
    camera.fovy = 60;              // Champ de vision (Field of View)
    camera.projection = CAMERA_PERSPECTIVE;

    // Fichier de log
    FILE *f = fopen("log.txt", "w");

    // --- 3. Boucle Principale ---
    while(!WindowShouldClose()){ // Tant qu'on n'appuie pas sur Croix ou ESC
        
        // Quitter explicitement
        if(IsKeyPressed(KEY_ESCAPE)) break;

        // --- ETAPE UPDATE (Mise à jour logique) ---
        
        // A. Physique et Mouvement du joueur
        UpdatePlayer(&player, blocks, &camera);

        // B. Gestion des inputs de tir
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            ShootProjectile(projs, player, f);
        }

        // C. Déplacement des balles et vérification collisions balles
        UpdateProjectiles(projs, blocks, &ciblePos, cibleRadius, &score);

        // D. Collision Joueur ramasse Cible (corps à corps)
        if(Vector3Distance(player.pos, ciblePos) <= player.size/2 + cibleRadius){
            score++;
            ciblePos.x = (float)(rand()%20-10);
            ciblePos.z = (float)(rand()%20-10);
        }

        // --- ETAPE DRAW (Rendu graphique) ---
        BeginDrawing();
        ClearBackground(RAYWHITE); // Efface l'écran précédent

        BeginMode3D(camera); // Passe en mode 3D
            DrawLevel(blocks);
            DrawCube(player.pos, player.size, player.size, player.size, BLUE); // Le joueur
            DrawSphere(ciblePos, cibleRadius, RED); // La cible
            DrawProjectiles(projs);
        EndMode3D(); // Fin du mode 3D

        // Interface utilisateur (UI) en 2D par dessus la 3D
        DrawText(TextFormat("Score: %d | FPS: %d", score, GetFPS()), 10, 10, 20, DARKGRAY);
        
        EndDrawing();
    }

    // --- 4. Nettoyage ---
    if(f) fclose(f);
    CloseWindow(); // Ferme la fenêtre OpenGL
    return 0;
}

