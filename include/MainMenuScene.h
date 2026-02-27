#pragma once

#include "raylib.h"

#include "Scene.h"

#include "SceneManager.h"

#include "raylib-nuklear.h"

class MainMenuScene : public Scene {
    
    private:
    
        bool singleplayerMenu = false;
        
        bool settingsMenu = false;
        
        bool startGame = false;
        
        std::shared_ptr<SceneManager> _sm;
        
        int fontSize = 10;
        
        struct nk_context *ctx;
    public:
    
    MainMenuScene(std::shared_ptr<SceneManager> sm);
    
    void Tick() override;
    
    void Exit() override;
    
};