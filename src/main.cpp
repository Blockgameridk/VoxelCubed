#define RAYLIB_NUKLEAR_IMPLEMENTATION
#include "raylib.h"
#include "iostream"
#include "SceneManager.h"
#include "MainMenuScene.h"
#include "GameScene.h"



int main(void) {
    
    const int screenWidth = 800;
    const int screenHeight = 450;
    
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    
    InitWindow(screenWidth,screenHeight, "VoxelGame");
    
    std::shared_ptr<SceneManager> sm = std::make_shared<SceneManager>();
    
    sm->AddScene("MainMenu", std::make_unique<MainMenuScene>(sm));
    
    sm->AddScene("GameScene", std::make_unique<GameScene>(sm));
    
    sm->SwitchScene("MainMenu");
    
    while (!WindowShouldClose()) {
        
        sm->Tick();
        
    }
    
    CloseWindow();
    
}