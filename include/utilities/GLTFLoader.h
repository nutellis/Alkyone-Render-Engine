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

class Mesh;
class Node;
class Material;
class MeshGroup;

class GLTFLoader
{
    // std::unordered_map<std::string, Mesh*> meshes;
    // std::unordered_map<std::string, Node*> nodes;
    // //std::unordered_map<std::string, AllocatedImage> images;
    // std::unordered_map<std::string, Material*> materials;
    //
    // // nodes that are top level
    // std::vector<Node*> topLevelNodes;


    tinygltf::TinyGLTF loader;

    std::string err;
    std::string warn;
    std::string filename = "../assets/box/vertexcolours/BoxVertexColors.glb";

    tinygltf::Model model {};

public:
    bool Load();
    bool PopulateMeshGroup(MeshGroup& meshgroup);
    void GetVertices(const tinygltf::Primitive& primitive, TArray<Vertex>& vertices);
    void GetIndices(const tinygltf::Primitive& primitive, uint32 padding, TArray<uint32>& indices);
    void dbgModel();
};


#endif //ALKYONERENDERENGINE_MESHLOADER_H