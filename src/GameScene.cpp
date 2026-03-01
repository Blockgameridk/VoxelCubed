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
        currentRay = GetMouseRay(screenCenter, *world->playerCam);
    
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
    
    UpdateCamera(world->playerCam.get(), CAMERA_FREE);
    
    BeginDrawing();
    
    ClearBackground(SKYBLUE);
    
    BeginMode3D(*world->playerCam);
    
    world->renderWorld();

    Vector3 cubeVec = world->getBlockVectorFromRay(currentRay);
        
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
