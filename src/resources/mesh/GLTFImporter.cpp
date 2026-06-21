//
// Created by Nutellis on 24-Jan-26.
//

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define TINYGLTF_NOEXCEPTION

#include "../../../include/resources/mesh/GLTFImporter.h"

#include <iostream>

#include "rendering/types/Vertex.h"
#include "spdlog/spdlog.h"
#include <core/PODTypes.h>

#include "math/ParabolaMath.h"
#include "rendering/Mesh.h"
#include "rendering/MeshGroup.h"
#include "rhi/core/IBuffer.h"
#include "scene/RenderObject.h"
#include "scene/SceneManager.h"


bool GLTFImporter::ImportSceneFile(const std::string& filename, SceneManager& sceneManager /*, ResourceManager& resourceManager*/)
{
    tinygltf::TinyGLTF loader {};
    tinygltf::Model gltf {};
    std::string err;
    std::string warn;


    //bool ret = loader.LoadBinaryFromFile(&gltf, &err, &warn, filename.c_str());
    bool ret = loader.LoadASCIIFromFile(&gltf, &err, &warn, filename.c_str());
    spdlog::error(ret);
    //
    if (!warn.empty()) {
        printf("Warn: %s\n", warn.c_str());
    }

    if (!err.empty()) {
        printf("Err: %s\n", err.c_str());
    }

    if (!ret)
    {
        printf("Failed to parse glTF: %s\n", filename.c_str());
    }


    //ret += PopulateMeshGroup(gltfModel, meshGroup);

    ret = ProcessScenes(gltf
        ,sceneManager/*,resourceManager*/);
    return ret;


}

bool GLTFImporter::ProcessScenes(const tinygltf::Model& gltf, SceneManager& sceneManager /*, ResourceManager& resourceManager*/)
{
    if (gltf.scenes.empty()) {
        spdlog::error("No scenes found in glTF file");
        return false;
    }

    const std::vector<tinygltf::Node>& nodes = gltf.nodes;

    // do work on all scenes
     for (const auto& scene : gltf.scenes)
     {
        Scene* activeScene = sceneManager.GetScene(sceneManager.CreateScene()); // questionable approach...
        std::vector<size_t> stackIndices;

        std::vector<uint32> indexToHandleMap = std::vector<uint32>(
            nodes.size(), 0xFFFFFFFF);
        std::vector<size_t> gltfSourceIndices;

         //init arrays
         const size_t estimatedNodes = nodes.size();

         //move this inside scene or scenemanager
         activeScene->renderObjects.reserve(estimatedNodes + 1);
         activeScene->transforms.reserve(estimatedNodes + 1);
         activeScene->names.reserve(estimatedNodes + 1);
         activeScene->dirty.reserve(estimatedNodes + 1);
         activeScene->types.reserve(estimatedNodes + 1);
         activeScene->meshIndices.reserve(estimatedNodes + 1);

        //set root
        RenderObject rootObject {};
        activeScene->renderObjects.push_back(rootObject);
        activeScene->transforms.push_back(Matrix4f::IDENTITY);
        activeScene->names.emplace_back("Root");
        activeScene->dirty.push_back(false);
        activeScene->types.push_back(ObjectType::ROOT);
        activeScene->meshIndices.push_back(0xFFFFFF);

         RenderObjectHandle rootHandle = RenderObjectHandle{
             Handle(1, 0)
         };


        stackIndices.insert(stackIndices.end(), scene.nodes.rbegin(), scene.nodes.rend());

        while (stackIndices.empty() == false)
        {
            size_t stackIndex = stackIndices.back();
            stackIndices.pop_back();
            const tinygltf::Node& currentNode = nodes[stackIndex]; //current node

            RenderObject currentObject {};

            indexToHandleMap[stackIndex] = static_cast<uint32>(activeScene->renderObjects.size());
            gltfSourceIndices.push_back(stackIndex);

            activeScene->renderObjects.push_back(currentObject);
            activeScene->transforms.push_back(ProcessTransform(currentNode));
            activeScene->names.push_back(currentNode.name);
            activeScene->dirty.push_back(false);
            activeScene->meshIndices.push_back(0xFFFFF);

            if (currentNode.mesh > -1)
            {
                activeScene->types.push_back(ObjectType::MESHGROUP);
                //found a mesh, we should fetch the info
               // activeScene->meshIndices.back() = GetMeshData(gltf, currentNode.mesh);
            }
            else if (currentNode.camera > -1)
            {
                activeScene->types.push_back(ObjectType::CAMERA);
            }
            else if (currentNode.light > -1)
            {
                auto lightType = gltf.lights[currentNode.light].type == "point" ? ObjectType::POINT_LIGHT : ObjectType::SPOT_LIGHT;
                activeScene->types.push_back(lightType);
            }
            else if (currentNode.children.empty() == false)
            {
                activeScene->types.push_back(ObjectType::TRANSFORM);
            }
            else
            {
                activeScene->types.push_back(ObjectType::UNKNOWN);
            }

            stackIndices.insert(stackIndices.end(),
                currentNode.children.rbegin(),
                currentNode.children.rend()
                );
        }

        RenderObjectHandle currentHandle = RenderObjectHandle{Handle(1,  0)};
        RenderObjectHandle previousSiblingHandle = RenderObjectHandle{Handle(1, 0xFFFFFFFF)};
        for (int32 i =  static_cast<int32>(scene.nodes.size()) - 1; i >= 0; i--)
        {
            size_t childGltfIndex = scene.nodes[i];
            RenderObjectHandle childHandle = RenderObjectHandle{Handle(1,  indexToHandleMap[childGltfIndex])};

            activeScene->renderObjects[indexToHandleMap[childGltfIndex]].parent = currentHandle;

            if (i == 0)
            {
                activeScene->renderObjects[0].firstChild = childHandle;
            } else
            {
                activeScene->renderObjects[indexToHandleMap[childGltfIndex]].nextSibling = previousSiblingHandle;

            }
            previousSiblingHandle = childHandle;
        }

        for (size_t i = 1; i < activeScene->renderObjects.size(); i++)
        {
            const tinygltf::Node& node = nodes[gltfSourceIndices[i - 1]];

            currentHandle = RenderObjectHandle{Handle(1,  indexToHandleMap[i - 1])};
            previousSiblingHandle = RenderObjectHandle{Handle(1, 0xFFFFFFFF)};


            for (int32 j =  static_cast<int32>(node.children.size()) - 1; j >= 0; j--)
            {
                size_t childGltfIndex = node.children[j];
                RenderObjectHandle childHandle = RenderObjectHandle{Handle(1,  indexToHandleMap[childGltfIndex])};

                activeScene->renderObjects[indexToHandleMap[childGltfIndex]].parent = currentHandle;

                if (j == 0)
                {
                    activeScene->renderObjects[i].firstChild = childHandle;
                } else
                {
                    activeScene->renderObjects[indexToHandleMap[childGltfIndex]].nextSibling = previousSiblingHandle;

                }
                previousSiblingHandle = childHandle;
            }
        }
         spdlog::info("Finished processing scene");
     }
    return true;
}

bool GLTFImporter::GetMeshData(const tinygltf::Model& gltf, size_t meshIndex /*, SceneManager& sceneManager*/)
{
    const std::vector<tinygltf::Mesh> meshes = gltf.meshes;
    //MESH
    tinygltf::Mesh mesh = meshes[meshIndex];
    for (tinygltf::Primitive& primitive : mesh.primitives)
    {
        // here is the place where the AssetRegistry comes in play

        //create a new Mesh
        Mesh * meshPrimitive = new Mesh();
        size_t primitiveStartIndex = 0;
        size_t indexCount = 0;
        size_t vertexStart = 0;

        std::vector<uint32> indices {};
        std::vector<Vertex> vertices {};


        vertexStart = vertices.size();


        GetVertices(gltf, primitive, vertices);
        // here we say: if the last primitive had X indices, the next one will be saved on size + 1.
        primitiveStartIndex = indices.size();


        indexCount += gltf.accessors[primitive.indices].count;
        //  indices.Resize(indexCount);
        GetIndices(gltf, primitive,vertexStart,indices);

        meshPrimitive->firstIndex = primitiveStartIndex;
        meshPrimitive->indexCount = indexCount;
        meshPrimitive->vertices = vertices;
        meshPrimitive->indices = indices;

        // push the mesh to the AssetRegistry and get a handle back
        //meshPrimitive->handle = resourceManager.AddMesh(*meshPrimitive);

        spdlog::info("Finished processing mesh {}", meshPrimitive->firstIndex);
    }

    return true;
}


Matrix4f GLTFImporter::ProcessTransform(tinygltf::Node node)
{
    /*
    The global transform of a node
    is given by the product of all local
    transforms on the path from the
    root to the respective node
    */

    //MATRIX
    if (node.matrix.size() == 16)
    {
        return {node.matrix.data()};
    } else
    {
        // no matrix present so we must initialize empty T, R, S matrices.
        // Get the ones that are present and combine them.
        Matrix4f T = Matrix4f::IDENTITY;
        Matrix4f R = Matrix4f::IDENTITY;
        Matrix4f S = Matrix4f::IDENTITY;

        if (node.translation.size() == 3)
        {
            T = Translate(Float3(node.translation.data()), T);
        }
        if (node.rotation.size() == 4)
        {
            Float3 rotation = QuatToEuler(Float4(node.rotation.data()));
            R = Rotate(rotation, R);
        }
        if (node.scale.size() == 3)
        {
            S = Scale(Float3(node.scale.data()), S);
        }

        return T * R * S;
    }
}

bool GLTFImporter::ProcessMesh(tinygltf::Mesh mesh)
{
    return false;
}



bool GLTFImporter::PopulateMeshGroup(tinygltf::Model gltfModel, MeshGroup& meshgroup)
{
    tinygltf::Mesh mesh = gltfModel.meshes[0];

    size_t primitiveStartIndex = 0;
    size_t indexCount = 0;
    size_t vertexStart = 0;

    std::vector<uint32> indices {};
    std::vector<Vertex> vertices {};


    //equivalents:
    //  tinygltf::Mesh == MeshGroup
    //  tinygltf::Primitive == Mesh

    //for each primitive inside the mesh
    for (tinygltf::Primitive& primitive : mesh.primitives)
    {
        //create a new Mesh
        Mesh * meshPrimitive = new Mesh();

        vertexStart = vertices.size();

        GetVertices(gltfModel, primitive, vertices);
        // here we say: if the last primitive had X indices, the next one will be saved on size + 1.
        primitiveStartIndex = indices.size();


        indexCount += gltfModel.accessors[primitive.indices].count;
      //  indices.Resize(indexCount);
        GetIndices(gltfModel, primitive,vertexStart,indices);

        meshPrimitive->firstIndex = primitiveStartIndex;
        meshPrimitive->indexCount = indexCount;
        meshPrimitive->vertices = vertices;
        meshPrimitive->indices = indices;


        meshgroup.meshLodGroups.PushBack(meshPrimitive);
    }

    return true;
}


void GLTFImporter::GetVertices(const tinygltf::Model model, const tinygltf::Primitive& primitive, std::vector<Vertex>& vertices)
{
    //vertices
    const float* positionBuffer = nullptr;
    const float* normalsBuffer = nullptr;
    const float* texCoordsBuffer = nullptr;
    const float* coloursBuffer = nullptr;
    size_t vertexCount = 0;

    if (primitive.attributes.find("POSITION") != primitive.attributes.end() )
        {
            //attributes are saved as dictionaries so the index of the accessor is on the ->second
            const tinygltf::Accessor & pAccessor = model.accessors[primitive.attributes.find("POSITION")->second];
            const tinygltf::BufferView & pBufferView = model.bufferViews[pAccessor.bufferView];
            positionBuffer = reinterpret_cast<const float*>(&(
                model.buffers[pBufferView.buffer]
                .data[pBufferView.byteOffset + pAccessor.byteOffset]
            ));
            vertexCount = pAccessor.count;
        }

        if (primitive.attributes.find("NORMAL") != primitive.attributes.end() )
        {
            //attributes are saved as dictionaries so the index of the accessor is on the ->second
            const tinygltf::Accessor & nAccessor = model.accessors[primitive.attributes.find("POSITION")->second];
            const tinygltf::BufferView & nBufferView = model.bufferViews[nAccessor.bufferView];
            normalsBuffer = reinterpret_cast<const float*>(&(
                model.buffers[nBufferView.buffer]
                .data[nBufferView.byteOffset + nAccessor.byteOffset]
            ));
            vertexCount = nAccessor.count;
        }

        if (primitive.attributes.find("TEXCOORD_0") != primitive.attributes.end()) {
            const tinygltf::Accessor& tAccessor = model.accessors[primitive.attributes.find("TEXCOORD_0")->second];
            const tinygltf::BufferView& tBufferView = model.bufferViews[tAccessor.bufferView];
            texCoordsBuffer = reinterpret_cast<const float*>(&(
                model.buffers[tBufferView.buffer]
                .data[tBufferView.byteOffset + tAccessor.byteOffset]
            ));
        }

        if (primitive.attributes.find("COLOR_0") != primitive.attributes.end()) {
            const tinygltf::Accessor& cAccessor = model.accessors[primitive.attributes.find("COLOR_0")->second];
            const tinygltf::BufferView& cBufferView = model.bufferViews[cAccessor.bufferView];
            coloursBuffer = reinterpret_cast<const float*>(&(
                model.buffers[cBufferView.buffer]
                .data[cBufferView.byteOffset + cAccessor.byteOffset]
            ));
        }

        for (size_t i = 0; i < vertexCount; i++)
        {
            Vertex vertex {};
            vertex.position = Float3(&positionBuffer[i * 3]);
            if (normalsBuffer)
            {
                vertex.normal = Float3(&normalsBuffer[i * 3]);
            } else
            {
                vertex.normal = Float3::ZERO;
            }
            if (texCoordsBuffer)
            {
                vertex.uvs = Float2(&texCoordsBuffer[i * 2]);
            } else
            {
                vertex.uvs = Float2(0.0f);
            }
            if (coloursBuffer)
            {
                vertex.color = Float3(&coloursBuffer[i * 3]);
            } else
            {
                vertex.color = Float3(1.0f);
            }

            vertices.push_back(vertex);

        }
}


void GLTFImporter::GetIndices(const tinygltf::Model model, const tinygltf::Primitive& primitive, uint32 padding, std::vector<uint32>& indices) {
    //indices
    const tinygltf::Accessor& indexAccessor = model.accessors[primitive.indices];
    const tinygltf::BufferView& bufferView = model.bufferViews[indexAccessor.bufferView];
    const tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];

    //indices.Re(indices.size() + indexAccessor.count);

    // get the raw index data from the buffer based on the type of data (unsigned int, short, byte)
    switch (indexAccessor.componentType)
    {
    case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
        {
            //get a pointer to the first element. Buffer at 0 + bufferview offset + accessor offset
            const uint32* buf = reinterpret_cast<const uint32*>(&buffer.data[bufferView.byteOffset + indexAccessor.byteOffset]);
            for (size_t i = 0; i < indexAccessor.count; i++)
            {
                indices.push_back(buf[i] + padding);
            }
            break;
        }
    case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
        {
            const uint16* buf = reinterpret_cast<const uint16*>(&buffer.data[bufferView.byteOffset + indexAccessor.byteOffset]);
            for (size_t i = 0; i < indexAccessor.count; i++)
            {
                indices.push_back(buf[i] + padding);
            }
            break;
        }
    case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
        {
            const uint8 * buf = reinterpret_cast<const uint8*>(&buffer.data[bufferView.byteOffset + indexAccessor.byteOffset]);
            for (size_t i = 0; i < indexAccessor.count; i++) {
                indices.push_back(buf[i] + padding);

            }
            break;
        }
    default:
        spdlog::error("Index Component type {} not supported", indexAccessor.componentType);
    }
}

// void GLTFLoader::dbgModel() {
//     for (auto &mesh : model.meshes) {
//         std::cout << "mesh : " << mesh.name << std::endl;
//         for (auto &primitive : mesh.primitives) {
//             const tinygltf::Accessor &indexAccessor =
//                 model.accessors[primitive.indices];
//
//             std::cout << "indexaccessor: count " << indexAccessor.count << ", type "
//                       << indexAccessor.componentType << std::endl;
//
//             // tinygltf::Material &mat = model.materials[primitive.material];
//             // for (auto &mats : mat.values) {
//             //     std::cout << "mat : " << mats.first.c_str() << std::endl;
//             // }
//             //
//             // for (auto &image : model.images) {
//             //     std::cout << "image name : " << image.uri << std::endl;
//             //     std::cout << "  size : " << image.image.size() << std::endl;
//             //     std::cout << "  w/h : " << image.width << "/" << image.height
//             //               << std::endl;
//             // }
//
//             std::cout << "indices : " << primitive.indices << std::endl;
//             std::cout << "mode     : "
//                       << "(" << primitive.mode << ")" << std::endl;
//
//             for (auto &attrib : primitive.attributes) {
//                 std::cout << "attribute : " << attrib.first.c_str() << std::endl;
//             }
//         }
//     }
// }