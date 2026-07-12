//
// Created by Nutellis on 25-Jan-26.
//

#ifndef ALKYONERENDERENGINE_SCENE_H
#define ALKYONERENDERENGINE_SCENE_H

#include <core/PODTypes.h>
#include "SceneObject.h"
#include "containers/SlotMap.h"
#include "math/Matrix4.h"
#include "../rendering/object/Mesh.h"


struct SceneHandle {
    Handle id;
};

class Scene
{

private:
    // SlotMap<RenderObject> renderObjects; // i dont have a need for dynamic scenes at the moment

public: //TODO: make them private later please
    std::vector<SceneObject> objects; // a list of all the objects
    std::vector<Matrix4f> transforms;
    std::vector<std::string> names;
    std::vector<bool> dirty;
    std::vector<ObjectType> types;
    std::vector<Handle> meshIndices;

public:


    Scene() = default;
    ~Scene() = default;

    SceneObjectHandle AddSceneObject(SceneObject& object);
    void RemoveRenderObject(SceneObjectHandle renderObjectHandle);

    void Update();
};



#endif //ALKYONERENDERENGINE_SCENE_H