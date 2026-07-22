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
    Scene & scene = sceneList[id];

    for (SceneNode nodeData : parsedData.flatHierarchy)
    {
        SceneObject object;

        object.firstChild = SceneObjectHandle{nodeData.firstChild};
        object.nextSibling = SceneObjectHandle{nodeData.nextSibling};
        object.parent = SceneObjectHandle{nodeData.parent};

        scene.objects.push_back(object);
        scene.names.push_back(nodeData.name);
        scene.localTransforms.push_back(nodeData.transform);
        scene.types.push_back(static_cast<ObjectType>(nodeData.type));
        scene.dirty.push_back(nodeData.dirty);

        if (nodeData.gltfMeshIndex != -1)
        {
            scene.meshIndices.push_back(parsedData.engineMeshHandles[nodeData.gltfMeshIndex]);
        }
        else
        {
            scene.meshIndices.push_back(Handle::Invalid());
        }
    }

    UpdateGlobalTransforms(id);
}

void SceneManager::UpdateGlobalTransforms(Handle sceneId)
{
    Scene& scene = sceneList[sceneId];

    scene.globalTransforms.resize(scene.localTransforms.size());

    scene.globalTransforms[0] = scene.localTransforms[0];

    for (size_t i = 1; i < scene.objects.size(); ++i)
    {
        uint32 parentIndex = scene.objects[i].parent.id.index;
        scene.globalTransforms[i] = scene.globalTransforms[parentIndex] * scene.localTransforms[i];
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
