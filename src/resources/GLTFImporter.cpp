//
// Created by Nutellis on 24-Jan-26.
//

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define TINYGLTF_NOEXCEPTION

#include "resources/GLTFImporter.h"

#include <iostream>

#include "rendering/types/Vertex.h"
#include "spdlog/spdlog.h"
#include <core/PODTypes.h>

#include "math/ParabolaMath.h"
#include "rendering/object/Mesh.h"
#include "rendering/object/MeshGroup.h"


ParsedData GLTFImporter::ImportSceneFile(const std::string& filename)
{
    tinygltf::TinyGLTF loader {};
    tinygltf::Model gltf {};
    std::string err;
    std::string warn;

    ParsedData result = {};

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

     if (ret)
     {
         result = ProcessScenes(gltf);
     }

    return result;
}

ParsedData GLTFImporter::ProcessScenes(const tinygltf::Model& gltf)
{
    ParsedData result = {};
    if (gltf.scenes.empty()) {
        spdlog::error("No scenes found in glTF file");
        return result;
    }

    const std::vector<tinygltf::Node>& nodes = gltf.nodes;

    // do work on all scenes
     for (const auto& scene : gltf.scenes)
     {
        std::vector<size_t> stackIndices;

        std::vector<uint32> indexToHandleMap = std::vector<uint32>(
            nodes.size(), 0xFFFFFFFF);
        std::vector<size_t> gltfSourceIndices;

         //init arrays
        result.flatHierarchy.reserve(nodes.size());
        result.sceneMeshes.reserve(gltf.meshes.size());

         SceneNode root = {
             .transform = Matrix4f::IDENTITY,
             .name = "Root",
             .type = 0, //root
             .dirty = false,
             .gltfMeshIndex = -1,
             .parent = Handle {0, 0xFFFFFFFF},
             .firstChild = Handle {0, 0xFFFFFFFF},
             .nextSibling = Handle {0, 0xFFFFFFFF}
         };

         result.flatHierarchy.push_back(root);

        stackIndices.insert(stackIndices.end(), scene.nodes.rbegin(), scene.nodes.rend());

        while (stackIndices.empty() == false)
        {
            size_t stackIndex = stackIndices.back();
            stackIndices.pop_back();
            const tinygltf::Node& currentNode = nodes[stackIndex]; //current node

            indexToHandleMap[stackIndex] = static_cast<uint32>(result.flatHierarchy.size());
            gltfSourceIndices.push_back(stackIndex);

            SceneNode parsedNode {
                .transform = ProcessTransform(currentNode),
                .name = currentNode.name,
                .type = 7, //unknown
                .dirty = false,
                .gltfMeshIndex = currentNode.mesh,
                .parent = Handle {0, 0xFFFFFFFF},
                .firstChild = Handle {0, 0xFFFFFFFF},
                .nextSibling = Handle {0, 0xFFFFFFFF}
            };
            if (currentNode.mesh > -1)
            {
                parsedNode.type = 6;
                //found a mesh, we should fetch the info
                result.sceneMeshes.push_back(ProcessMeshData(gltf, currentNode.mesh));
            }
            else if (currentNode.camera > -1)
            {
                parsedNode.type = 5;
            }
            else if (currentNode.light > -1)
            {
                auto lightType = gltf.lights[currentNode.light].type == "point" ? 2 : 3;
                 parsedNode.type = lightType;
            }
            else if (currentNode.children.empty() == false)
            {
                parsedNode.type = 1;
            }

            result.flatHierarchy.push_back(parsedNode);

            stackIndices.insert(stackIndices.end(),
                currentNode.children.rbegin(),
                currentNode.children.rend()
                );
        }

        Handle currentHandle = Handle(1,  0);
        Handle previousSiblingHandle = Handle(1, 0xFFFFFFFF);
        for (int32 i =  static_cast<int32>(scene.nodes.size()) - 1; i >= 0; i--)
        {
            size_t childGltfIndex = scene.nodes[i];
            Handle childHandle = Handle(1,  indexToHandleMap[childGltfIndex]);

            result.flatHierarchy[indexToHandleMap[childGltfIndex]].parent = currentHandle;

            if (i == 0)
            {
                result.flatHierarchy[0].firstChild = childHandle;
            } else
            {
                result.flatHierarchy[indexToHandleMap[childGltfIndex]].nextSibling = previousSiblingHandle;

            }
            previousSiblingHandle = childHandle;
        }

        for (size_t i = 1; i < result.flatHierarchy.size() - 1; i++)
        {
            const tinygltf::Node& node = nodes[gltfSourceIndices[i - 1]];

            currentHandle = Handle(1,  indexToHandleMap[i]);
            previousSiblingHandle = Handle(1, 0xFFFFFFFF);


            for (int32 j =  static_cast<int32>(node.children.size()) - 1; j >= 0; j--)
            {
                size_t childGltfIndex = node.children[j];
               Handle childHandle = Handle(1,  indexToHandleMap[childGltfIndex]);

                result.flatHierarchy[indexToHandleMap[childGltfIndex]].parent = currentHandle;

                if (j == 0)
                {
                    result.flatHierarchy[i].firstChild = childHandle;
                } else
                {
                    result.flatHierarchy[indexToHandleMap[childGltfIndex]].nextSibling = previousSiblingHandle;

                }
                previousSiblingHandle = childHandle;
            }
        }
         spdlog::info("Finished processing scene");
     }
    return result;
}

MeshGroup GLTFImporter::ProcessMeshData(const tinygltf::Model& gltf, size_t meshIndex)
{
    const std::vector<tinygltf::Mesh> meshes = gltf.meshes;
    //MESH
    const tinygltf::Mesh mesh = meshes[meshIndex];

    //create a new MeshGroup
    MeshGroup meshGroup = {};
    meshGroup.subMeshes.reserve(mesh.primitives.size());
    for (const tinygltf::Primitive& primitive : mesh.primitives)
    {
        Mesh meshPrimitive = {};

        GetVertices(gltf, primitive, meshGroup.vertices);

        GetIndices(gltf, primitive,meshGroup.indices);

        meshPrimitive.firstIndex = meshGroup.indices.size();
        meshPrimitive.indexCount = gltf.accessors[primitive.indices].count;
        meshPrimitive.vertexOffset = meshGroup.vertices.size();
        meshPrimitive.indexBufferOffset = static_cast<uint32>(meshGroup.vertices.size() * sizeof(Vertex));
        meshPrimitive.materialIndex = primitive.material;

        meshGroup.subMeshes.push_back(meshPrimitive);

        spdlog::info("Finished processing primitive. firstIndex: {}, vertexOffset: {}",
                 meshPrimitive.firstIndex, meshPrimitive.vertexOffset);
    }

    return meshGroup;
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


void GLTFImporter::GetIndices(const tinygltf::Model model, const tinygltf::Primitive& primitive, std::vector<uint32>& indices) {
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
                indices.push_back(buf[i]);
            }
            break;
        }
    case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
        {
            const uint16* buf = reinterpret_cast<const uint16*>(&buffer.data[bufferView.byteOffset + indexAccessor.byteOffset]);
            for (size_t i = 0; i < indexAccessor.count; i++)
            {
                indices.push_back(buf[i]);
            }
            break;
        }
    case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
        {
            const uint8 * buf = reinterpret_cast<const uint8*>(&buffer.data[bufferView.byteOffset + indexAccessor.byteOffset]);
            for (size_t i = 0; i < indexAccessor.count; i++) {
                indices.push_back(buf[i]);

            }
            break;
        }
    default:
        spdlog::error("Index Component type {} not supported", indexAccessor.componentType);
    }
}

// void GLTFImporter::dbgModel() {
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