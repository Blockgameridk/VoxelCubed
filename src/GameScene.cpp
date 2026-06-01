#include "GameScene.h"

#include "raymath.h"


#include <fstream>

GameScene::GameScene(std::shared_ptr<SceneManager> sm)
{
   world = new World();
}

GameScene::~GameScene()
{

}

void UpdateCameraStationary(Camera *camera, Player* player) {
    Vector2 delta = GetMouseDelta();
    
    float sensitivity = 0.003f;
    
    player->yaw += delta.x * sensitivity;
    
    player->pitch -= delta.y * sensitivity;
    
    player->pitch = Clamp(player->pitch,-1.5f,1.5f);
    Vector3 forward;
    
    forward.x = cosf(player->pitch) * cosf(player->yaw);
    forward.y = sinf(player->pitch);
    forward.z = cosf(player->pitch) * sinf(player->yaw);

    // Update target to be position + forward vector
    camera->target = Vector3Add(camera->position, forward);
    
    forward = Vector3Normalize(Vector3Subtract(camera->target, camera->position));
    Vector3 up = camera->up; // Standard up vector, usually (0, 1, 0)
    Vector3 right = Vector3Normalize(Vector3CrossProduct(forward, up));
    
        // Calculate movement direction based on keyboard input
    Vector3 moveDir = { 0.0f, 0.0f, 0.0f };

    if (IsKeyDown(KEY_W)) player->Position = Vector3Add(player->Position, Vector3Multiply(forward, {0.1f,0.1f,0.1f})); // Move forward
    if (IsKeyDown(KEY_S)) player->Position = Vector3Subtract(player->Position, Vector3Multiply(forward, {0.1f,0.1f,0.1f})); // Move backward
    if (IsKeyDown(KEY_D)) player->Position = Vector3Add(player->Position, Vector3Multiply(right, {0.1f,0.1f,0.1f})); // Move backward
    if (IsKeyDown(KEY_A)) player->Position = Vector3Subtract(player->Position, Vector3Multiply(right, {0.1f,0.1f,0.1f})); // Move backward
    // Optional: Normalize moveDir so diagonal movement isn't faster
    moveDir = Vector3Normalize(moveDir);
    
}

void GameScene::Tick()
{
    
    world->CalculatePlayerCollisions();
    
    for (int i = 0; i < inventoryKeys.size(); i++) {
        
        if (IsKeyPressed(inventoryKeys[i])) {
            currentSlot = i;
        }
        
    }
    
    if (!IsCursorHidden()) {
        DisableCursor();
    }
    
        Vector2 screenCenter = { GetScreenWidth()/2.0f, GetScreenHeight()/2.0f };
        currentRay = GetMouseRay(screenCenter,*world->playerCam);
        
        currentRay.position = Vector3Add(currentRay.position,world->player->Position);
    
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
       
       world->removeBlockFromRay(currentRay);
    }
    
    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
        
       
       world->placeBlockFromRay(currentRay, world->Inventory[currentSlot].id);
        
    }
    
    
    
    if (IsKeyPressed(KEY_F6)) {
        world->save();
    }
    
    if (IsKeyPressed(KEY_F7)) {
        world->load();
    }
    
    //UpdateCamera(world->playerCam.get(), CAMERA_FREE);
    
    UpdateCameraStationary(world->playerCam.get(), world->player.get());
    
    world->playerCam->position = {0.0f,0.0f,0.0f};
    
    //world->playerCam->target = (Vector3){1.0f,world->playerCam->position.y,0.0f};
    
    BeginDrawing();
    
    ClearBackground(SKYBLUE);
    
    BeginMode3D(*world->playerCam);
    
    world->renderWorld();

    Vector3 cubeVec = Vector3Subtract(world->getBlockVectorFromRay(currentRay),world->player->Position);
    
    std::cout << cubeVec.x << ", " << cubeVec.y << ", " << cubeVec.z << std::endl;
    
    DrawCube(cubeVec,1.01f,1.01f,1.01f,{255,255,255,150});

    //lastcubeVec = cubeVec;
    //DrawRay(currentRay,PURPLE);
    
    EndMode3D();
    
    DrawFPS(0,0);
    
    EndDrawing();
}

void GameScene::Exit()
{
}
