//
// Created by Nutellis on 13-Jun-26.
//

#ifndef ALKYONERENDERENGINE_SCENEMANAGER_H
#define ALKYONERENDERENGINE_SCENEMANAGER_H

#include <vector>

#include "Scene.h"
#include "resources/GLTFImporter.h"

struct SceneNode;
class Scene;

//can hold a list of scenes that can be active or waiting to be loaded
class SceneManager
{
public:
    SceneManager();
    ~SceneManager();

    bool Initialize();
    void Terminate();
    void SetupSceneHierarchy(const ParsedData& parsed_data);

    void UpdateGlobalTransforms(Handle sceneId);

    Handle CreateScene();

    void SetActiveScene(Handle sceneIndex);
  //  Scene* GetScene(SceneHandle scene);

private:
    SlotMap<Scene> sceneList;
    Scene activeScene;
};


#endif //ALKYONERENDERENGINE_SCENEMANAGER_H