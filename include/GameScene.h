#pragma once

#include "raylib.h"

#include "World.h"


class GameScene : public Scene {
    
    private:
        int currentSlot = 0;
        
        bool onGround = false;
        
        bool blockMenu = false;
        
        World* world;
        
        std::vector<KeyboardKey> inventoryKeys = {
            KEY_ONE,
            KEY_TWO,
            KEY_THREE,
            KEY_FOUR,
            KEY_FIVE,
            KEY_SIX,
            KEY_SEVEN,
            KEY_EIGHT,
            KEY_NINE
        };
        
        Ray currentRay;
        
    public:
    
    GameScene(std::shared_ptr<SceneManager> sm);
    
    ~GameScene();
void Tick() override;

void Exit() override;
    
};