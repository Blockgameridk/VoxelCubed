#include "SceneManager.h"
#include <format>

void SceneManager::AddScene(std::string sceneName, std::unique_ptr<Scene> scene)
{
    
    if (storedScenes.contains(sceneName)) {
        
        return;
        
    }
    
    storedScenes[sceneName] = std::move(scene);
    
}

void SceneManager::SwitchScene(std::string sceneName) {
    
    if (storedScenes.contains(sceneName)) {
        
        if (_scene != nullptr) {
            
            _scene = nullptr;
            
        }
        
        _scene = std::move(storedScenes[sceneName]);
        
    }
    
}

void SceneManager::Tick() {
    
    if (_scene != nullptr) {
        
        _scene->Tick();
        
    }
    
}

void SceneManager::RemoveScene(std::string sceneName)
{
    if (storedScenes.contains(sceneName)) {
        
        storedScenes[sceneName] = nullptr;
        
    }
}

SceneManager::SceneManager()
{
    
    std::cout << "SceneManager initialized" << std::endl;
    
}

SceneManager::~SceneManager() {
    
    std::cout << "SceneManager out!" << std::endl;
    
    for (auto& ref : storedScenes) {
        
        RemoveScene(ref.first);
        
        std::cout << std::format("Stored scene: {}", ref.first) << std::endl;
        
    }
    
    storedScenes.clear();
    
}