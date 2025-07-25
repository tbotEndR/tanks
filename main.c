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

#define TURNSPEED 0.05f
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
    const Vector3 xAxis = { 1.0f, 0.0f, 0.0f };
    const Vector3 yAxis = { 0.0f, 1.0f, 0.0f };
    const Vector3 zAxis = {0.0f, 0.0f, 1.0f};
    const Vector3 center = {0.0f, 0.0f, 0.0f};

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
    Vector3 bodyPosition = { 0 };
    Quaternion bodyRotation = QuaternionIdentity();
    Vector3 turretPosition = { 0 };
    Quaternion turretRotation = { 0 };

    //  rotation stuff
    float degAngle = 0.5f;
    float radAngleStep = degAngle * DEG2RAD;
    Quaternion xRotation = QuaternionIdentity();
    Quaternion yRotation = QuaternionIdentity();
    Quaternion zRotation = QuaternionIdentity();
    Quaternion stepRotation = { 0 };
    float quaternionAngle = 0.0f;
    Vector3 quaternionAxis = { 0 };
    float s = 0.0f;

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------

        //  rotate cube
        /*
        xRotation = QuaternionFromAxisAngle(xAxis, radAngleStep);
        yRotation = QuaternionFromAxisAngle(yAxis, radAngleStep);
        zRotation = QuaternionFromAxisAngle(zAxis, radAngleStep);
        stepRotation = QuaternionMultiply(zRotation, QuaternionMultiply(yRotation, xRotation));
        bodyRotation = QuaternionNormalize(QuaternionMultiply(stepRotation, bodyRotation));
        */

        //  get axis and angle from updated rotation
        quaternionAngle = (2.0f * acosf(Clamp(bodyRotation.w, -1.0f, 1.0f))) * RAD2DEG;
        s = sqrt(1 - (bodyRotation.w*bodyRotation.w));
        if (s < 0.001f) quaternionAxis = (Vector3) {1.0f, 0.0f, 0.0f};
        else {
            quaternionAxis.x = bodyRotation.x / s;
            quaternionAxis.y = bodyRotation.y / s;
            quaternionAxis.y = bodyRotation.y / s;
        }

        //  raycast from mouse to collide with invisible quad to get world position
        mouseScreenPosition = GetMousePosition();
        mouseRay = GetScreenToWorldRay(GetMousePosition(), camera);
        collision = GetRayCollisionBox(mouseRay, mousePointerBox);

        //  body angle
        if (IsKeyDown(KEY_A)) {
            yRotation = QuaternionFromAxisAngle(yAxis, TURNSPEED);
            bodyRotation = QuaternionNormalize(QuaternionMultiply(yRotation, bodyRotation));
        }
        if (IsKeyDown(KEY_D)) {
            yRotation = QuaternionFromAxisAngle(yAxis, -TURNSPEED);
            bodyRotation = QuaternionNormalize(QuaternionMultiply(yRotation, bodyRotation));
        }

        /*
        //  movement
        if (IsKeyDown(KEY_W)) {
            bodyPositionVect = Vector3Add(bodyPositionVect, Vector3Scale(bodyDirectionVect, DRIVESPEED));
        }
        if (IsKeyDown(KEY_S)) {
            bodyPositionVect = Vector3Add(bodyPositionVect, Vector3Scale(bodyDirectionVect, -DRIVESPEED));
        }
            */

        if (IsKeyPressed(KEY_R)) {
            bodyPosition =  center;
            bodyRotation =QuaternionIdentity();
        }


        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginMode3D(camera);
                DrawGrid(30, 1.0f);
                rlPushMatrix();
                    rlTranslatef(bodyPosition.x, bodyPosition.y, bodyPosition.z);
                    rlRotatef(quaternionAngle, quaternionAxis.x, quaternionAxis.y, quaternionAxis.z);
                    DrawCubeWires(center, 5.0f, 5.0f, 5.0f, BLUE);
                rlPopMatrix();

            EndMode3D();

            DrawText(TextFormat("%.2f|%.2f", mouseScreenPosition.x, mouseScreenPosition.y), mouseScreenPosition.x+10, mouseScreenPosition.y-10, 20, GREEN);
            //DrawText(TextFormat("turretToMouseVect: {%.2f, %.2f, %.2f}", turretToMouseVect.x, turretToMouseVect.y, turretToMouseVect.z), 10, 20, 10, BLACK);
            //DrawText(TextFormat("bodyDirectionVect: {%.2f, %.2f, %.2f}", bodyDirectionVect.x, bodyDirectionVect.y, bodyDirectionVect.z), 10, 40, 10, BLACK);
            //DrawText(TextFormat("bodyPositionVect: {%.2f, %.2f, %.2f}", bodyPositionVect.x, bodyPositionVect.y, bodyPositionVect.z), 10, 60, 10, BLACK);
            //DrawText(TextFormat("bodyRotation {%.2f |%.2f | %.2f | %.2f}", bodyRotation.w, bodyRotation.x, bodyRotation.y, bodyRotation.z), 10, 80, 10, BLACK);

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