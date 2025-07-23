/*******************************************************************************************
*
*   raylib [core] example - Initialize 3d camera mode
*
*   Example complexity rating: [★☆☆☆] 1/4
*
*   Example originally created with raylib 1.0, last time updated with raylib 1.0
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2014-2025 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"
#include "rlgl.h"
#include "raymath.h"

#define TURNSPEED 0.5f
#define DRIVESPEED 0.1f

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;
    const Vector3 yAxis = { 0.0f, 1.0f, 0.0f };
    const Vector3 zAxis = {0.0f, 0.0f, 1.0f};

    InitWindow(screenWidth, screenHeight, "raylib [core] example - 3d camera mode");

    // Define the camera to look into our 3d world
    Camera3D camera = { 0 };
    camera.position = (Vector3){ 0.0f, 20.0f, 10.0f };  // Camera position
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };      // Camera looking at point
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 60.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera mode type

    
    //  mouse input
    Ray mouseRay = { 0 };
    RayCollision collision = { 0 };
    BoundingBox mousePointerBox = {(Vector3) {-20.0f, 1.0f, -20.0f }, (Vector3) {20.0f, 1.0f, 20.0f }};
    Vector2 mouseScreenPosition = {0.0f, 0.0f};

    //  player body
    Vector3 bodyPositionVect = { 0 };
    Vector3 bodyDirectionVect = { 0 };
    Vector3 turretPositionVect = { 0 };
    Vector3 turretToMouseVect = { 0 };
    float bodyRotation = 0.0f;
    float radBodyRotation = bodyRotation;
    float turretRotation= 0.0f;

    

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------
        turretPositionVect = (Vector3) {bodyPositionVect.x, bodyPositionVect.y+1.0f, bodyPositionVect.z};
        //  raycast from mouse to collide with invisible quad to get world position
        mouseScreenPosition = GetMousePosition();
        mouseRay = GetScreenToWorldRay(GetMousePosition(), camera);
        collision = GetRayCollisionBox(mouseRay, mousePointerBox);

        //  calculate rotation angle of the turret
        turretToMouseVect = Vector3Normalize(Vector3Subtract(turretPositionVect, collision.point));
        turretRotation = Vector3Angle(turretToMouseVect, zAxis) * RAD2DEG;
        if (turretToMouseVect.x < 0) turretRotation *=-1;

        //  body angle
        if (IsKeyDown(KEY_A)) {
            bodyRotation -= TURNSPEED;
            if (bodyRotation >= 360) bodyRotation = 0;
            radBodyRotation = bodyRotation * DEG2RAD;
            bodyDirectionVect.x = cos(radBodyRotation);
            bodyDirectionVect.z = sin(radBodyRotation);
        }
        if (IsKeyDown(KEY_D)) {
            bodyRotation += TURNSPEED;
            if (bodyRotation < 0) bodyRotation = 360;
            radBodyRotation = bodyRotation * DEG2RAD;
            bodyDirectionVect.x = cos(radBodyRotation);
            bodyDirectionVect.z = sin(radBodyRotation);
        }

        

        //  movement
        if (IsKeyDown(KEY_W)) {
            bodyPositionVect = Vector3Add(bodyPositionVect, Vector3Scale(bodyDirectionVect, DRIVESPEED));
        }
        if (IsKeyDown(KEY_S)) {
            bodyPositionVect = Vector3Add(bodyPositionVect, Vector3Scale(bodyDirectionVect, -DRIVESPEED));
        }

        if (IsKeyPressed(KEY_R)) {
            bodyPositionVect = (Vector3){ 0.0f, 0.0f, 0.0f };
            bodyDirectionVect = (Vector3) {1.0f, 0.0f, 0.0f };
            turretPositionVect = (Vector3){ 0 };
            turretToMouseVect = (Vector3) { 0 };
            bodyRotation = 0.1f;
            turretRotation= 0.0f;
        }



        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginMode3D(camera);

                
                //DrawLine3D(turretPositionVect, collision.point, BLUE);
                DrawLine3D(bodyPositionVect, Vector3Add(bodyPositionVect, Vector3Scale(bodyDirectionVect, 20.0f)), BLUE);
                DrawCube(bodyPositionVect, 1.0f, 0.5f, 1.0f, BLUE);
                DrawCubeWires(bodyPositionVect, 1.0f, 0.5f, 1.0f, RAYWHITE);                

                /*
                rlPushMatrix();
                    rlTranslatef(bodyPositionVect.x, 0.0f, bodyPositionVect.z);
                    rlRotatef(bodyRotation, 0.0f, 1.0f, 0.0f);
                    DrawCube(bodyPositionVect, 2.0f, 1.0f, 2.0f, RED);
                    DrawCubeWires(bodyPositionVect, 2.0f, 1.0f, 2.0f, MAROON);
                rlPopMatrix();
                */

                DrawGrid(30, 1.0f);

            EndMode3D();

            //DrawText(TextFormat("%.2f|%.2f", collision.point.x, collision.point.z), mouseScreenPosition.x+10, mouseScreenPosition.y-10, 20, GREEN);
            //DrawText(TextFormat("turretToMouseVect: {%.2f, %.2f, %.2f}", turretToMouseVect.x, turretToMouseVect.y, turretToMouseVect.z), 10, 20, 10, BLACK);
            DrawText(TextFormat("bodyDirectionVect: {%.2f, %.2f, %.2f}", bodyDirectionVect.x, bodyDirectionVect.y, bodyDirectionVect.z), 10, 40, 10, BLACK);
            DrawText(TextFormat("bodyPositionVect: {%.2f, %.2f, %.2f}", bodyPositionVect.x, bodyPositionVect.y, bodyPositionVect.z), 10, 60, 10, BLACK);
            DrawText(TextFormat("bodyRotation: {%.2f}", bodyRotation), 10, 80, 10, BLACK);


            DrawFPS(5, 5);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}