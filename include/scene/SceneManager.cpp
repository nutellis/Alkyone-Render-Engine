//
// Created by Nutellis on 13-Jun-26.
//

#include "SceneManager.h"

SceneManager::SceneManager() : sceneList(SlotMap<Scene>()){}

SceneManager::~SceneManager() = default;

bool SceneManager::Initialize()
{
    activeScene = Scene();

    return true;
}

void SceneManager::Terminate()
{
    // empty the slotmap
}

SceneHandle SceneManager::CreateScene()
{
    Handle id = sceneList.createSlot(Scene());
    return SceneHandle{id};
}

void SceneManager::SetActiveScene(SceneHandle sceneIndex)
{
    // TODO: i need a getter!
   // activeScene = sceneList[sceneIndex];
}

Scene* SceneManager::GetScene(SceneHandle scene)
{
    return sceneList.GetSlot(scene.id);
}
