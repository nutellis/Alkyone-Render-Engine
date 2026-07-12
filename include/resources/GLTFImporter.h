//
// Created by Nutellis on 24-Jan-26.
//

#ifndef ALKYONERENDERENGINE_MESHLOADER_H
#define ALKYONERENDERENGINE_MESHLOADER_H

#include <string>
#include <tiny_gltf.h>

#include "math/GenericMath.h"
#include "math/Matrix4.h"
#include "../rendering/object/MeshGroup.h"
#include "rendering/types/Vertex.h"

class Mesh;
class Material;


struct SceneNode
{
    Matrix4f transform;
    std::string name;
    uint32 type; // 0 to 7. Silly but i prefer to avoid importing the whole render object logic here. I will make it better, i promise
    bool dirty;
    int32 gltfMeshIndex;


    Handle parent;
    Handle firstChild;
    Handle nextSibling;
};

struct ParsedData
{
    std::vector<SceneNode> flatHierarchy;
    std::vector<MeshGroup> sceneMeshes;
};

class GLTFImporter
{
public:
    GLTFImporter() = default;
    ~GLTFImporter() = default;

    //we assume this will load a "level" so we create an empty scene
    static ParsedData ImportSceneFile(const std::string& filename);
    //we assume this will load a single model (with multiple mesh/children)
    //static bool LoadModel(std::string& filename, SceneManager& sceneManager, ResourceManager& resourceManager);

private:
    static ParsedData ProcessScenes(const tinygltf::Model& gltf);
    bool ProcessMesh(tinygltf::Mesh mesh);
    static MeshGroup ProcessMeshData(const tinygltf::Model& gltf, size_t meshIndex);
    static Matrix4f ProcessTransform(tinygltf::Node node);
    static bool PopulateMeshGroup(tinygltf::Model gltfModel, MeshGroup& meshgroup);
    static void GetVertices(tinygltf::Model model, const tinygltf::Primitive& primitive, std::vector<Vertex>& vertices);
    static void GetIndices(tinygltf::Model model, const tinygltf::Primitive& primitive, std::vector<uint32>& indices);
    //static void dbgModel();



private:
    // tinygltf::TinyGLTF loader;
    //
    // std::string err;
    // std::string warn;
    // std::string filename = "../assets/box/vertexcolours/BoxVertexColors.gltf";

   // tinygltf::Model model {};

};


#endif //ALKYONERENDERENGINE_MESHLOADER_H