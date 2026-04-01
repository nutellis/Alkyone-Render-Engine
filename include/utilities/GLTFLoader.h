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
public:
    GLTFLoader() = default;
    ~GLTFLoader() = default;

    static bool Load(std::string filename, MeshGroup& meshgroup);

private:
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