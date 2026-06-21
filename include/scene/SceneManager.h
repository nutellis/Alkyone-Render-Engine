//
// Created by Nutellis on 13-Jun-26.
//

#ifndef ALKYONERENDERENGINE_SCENEMANAGER_H
#define ALKYONERENDERENGINE_SCENEMANAGER_H

#include <vector>

#include "Scene.h"

class Scene;

//can hold a list of scenes that can be active or waiting to be loaded
class SceneManager
{
private:
    SlotMap<Scene> sceneList;
    Scene activeScene;

public:
    SceneManager();
    ~SceneManager();

    bool Initialize();
    void Terminate();

    SceneHandle CreateScene();

    void SetActiveScene(SceneHandle sceneIndex);
    Scene* GetScene(SceneHandle scene);
};


#endif //ALKYONERENDERENGINE_SCENEMANAGER_H