//
// Created by Nutellis on 13-Jun-26.
//

#include "scene/SceneManager.h"

#include <algorithm>

SceneManager::SceneManager()
{
}

SceneManager::~SceneManager()
{
}

bool SceneManager::Initialize()
{
    return true;
}

void SceneManager::Terminate()
{
    // empty the slotmap
}

void SceneManager::SetupSceneHierarchy(const ParsedData& parsedData)
{
    Handle id = CreateScene();
    Scene scene = sceneList[id];

    for (SceneNode nodeData : parsedData.flatHierarchy)
    {
        SceneObject object;

        object.firstChild = SceneObjectHandle{nodeData.firstChild};
        object.nextSibling = SceneObjectHandle{nodeData.nextSibling};
        object.parent = SceneObjectHandle{nodeData.parent};

        scene.objects.push_back(object);
        scene.names.push_back(nodeData.name);
        scene.transforms.push_back(nodeData.transform);
        scene.types.push_back(static_cast<ObjectType>(nodeData.type));
        scene.dirty.push_back(nodeData.dirty);

        Handle meshIndex = parsedData.sceneMeshes[nodeData.gltfMeshIndex].;
        scene.meshIndices.push_back(meshIndex);
    }
}

Handle SceneManager::CreateScene()
{
    //TODO: add some check here
    return sceneList.createSlot(Scene());
}
//
    void SceneManager::SetActiveScene(Handle sceneIndex)
    {
        // TODO: i need a getter!
       // activeScene = sceneList[sceneIndex];
    }
//
// Scene* SceneManager::GetScene(SceneHandle scene)
// {
//     return sceneList.GetSlot(scene.id);
// }
