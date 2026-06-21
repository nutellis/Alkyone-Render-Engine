//
// Created by Nutellis on 24-Jan-26.
//

#ifndef ALKYONERENDERENGINE_MESHLOADER_H
#define ALKYONERENDERENGINE_MESHLOADER_H

#include <string>
#include <tiny_gltf.h>

#include "math/GenericMath.h"
#include "rendering/MeshGroup.h"
#include "rendering/types/Vertex.h"
#include "scene/RenderObject.h"

class ResourceManager;
class SceneManager;
class Mesh;
class Node;
class Material;
class MeshGroup;

class GLTFImporter
{
public:
    GLTFImporter() = default;
    ~GLTFImporter() = default;

    //we assume this will load a "level" so we create an empty scene
    static bool ImportSceneFile(const std::string& filename /*, SceneManager& sceneManager, ResourceManager& resourceManager*/
                                , SceneManager& sceneManager);
    static bool ProcessScenes(const tinygltf::Model& gltf, SceneManager& sceneManager);
    bool ProcessMesh(tinygltf::Mesh mesh);

    //we assume this will load a single model (with multiple mesh/children)
    //static bool LoadModel(std::string& filename, SceneManager& sceneManager, ResourceManager& resourceManager);

private:
    static bool GetMeshData(const tinygltf::Model& gltf, size_t meshIndex);
    static Matrix4f ProcessTransform(tinygltf::Node node);
    static bool PopulateMeshGroup(tinygltf::Model gltfModel, MeshGroup& meshgroup);
    static void GetVertices(tinygltf::Model model, const tinygltf::Primitive& primitive, std::vector<Vertex>& vertices);
    static void GetIndices(tinygltf::Model model, const tinygltf::Primitive& primitive, uint32 padding, std::vector<uint32>& indices);
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