#pragma once
#include <memory>
#include <string>
#include <map>
#include <iostream>
#include "Scene.h"

class SceneManager {
    
    private:
        
        std::unique_ptr<Scene> _scene;
        
        std::map<std::string, std::unique_ptr<Scene>> storedScenes;
        
    public:
        void AddScene(std::string sceneName, std::unique_ptr<Scene> scene);
        
        void RemoveScene(std::string sceneName);
        
        void SwitchScene(std::string sceneName);

        void Tick();

        SceneManager();
        ~SceneManager();
    
    
    
};