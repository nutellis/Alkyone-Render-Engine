//
// Created by Nutellis on 24-Jan-26.
//

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define TINYGLTF_NOEXCEPTION

#include "../../include/utilities/GLTFLoader.h"

#include <iostream>

#include "rendering/types/Vertex.h"
#include "spdlog/spdlog.h"
#include <core/PODTypes.h>

#include "rendering/Mesh.h"
#include "rendering/MeshGroup.h"

bool GLTFLoader::Load()
{
    tinygltf::Model modelTemp {};
    bool ret = loader.LoadBinaryFromFile(&model, &err, &warn, filename.c_str());
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

    return ret;
}

bool GLTFLoader::PopulateMeshGroup(MeshGroup& meshgroup)
{
    // lets work on 1 mesh for now
    tinygltf::Mesh mesh = model.meshes[0];

    size_t primitiveStartIndex = 0;
    size_t indexCount = 0;
    size_t vertexStart = 0;

    TArray<uint32> indices {};
    TArray<Vertex> vertices {};


    //equivalents:
    //  tinygltf::Mesh == MeshGroup
    //  tinygltf::Primitive == Mesh

    //for each primitive inside the mesh
    for (tinygltf::Primitive& primitive : mesh.primitives)
    {
        //create a new Mesh
        Mesh * meshPrimitive = new Mesh();

        vertexStart = vertices.Size();

        GetVertices(primitive, vertices);
        // here we say: if the last primitive had X indices, the next one will be saved on size + 1.
        primitiveStartIndex = indices.Size();


        indexCount += model.accessors[primitive.indices].count;
      //  indices.Resize(indexCount);
        GetIndices(primitive,vertexStart,indices);

        meshPrimitive->firstIndex = primitiveStartIndex;
        meshPrimitive->indexCount = indexCount;
        meshPrimitive->vertices = vertices;
        meshPrimitive->indices = indices;

        meshgroup.meshLodGroups.PushBack(meshPrimitive);
    }

    return true;
}


void GLTFLoader::GetVertices(const tinygltf::Primitive& primitive, TArray<Vertex>& vertices)
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
            vertex.position = Vector3f(&positionBuffer[i * 3]);
            if (normalsBuffer)
            {
                vertex.normal = Vector3f(&normalsBuffer[i * 3]);
            } else
            {
                vertex.normal = Vector3f::ZERO;
            }
            if (texCoordsBuffer)
            {
                vertex.uvs = Vector2f(&texCoordsBuffer[i * 2]);
            } else
            {
                vertex.uvs = Vector2f(0.0f);
            }
            if (coloursBuffer)
            {
                vertex.color = Vector3f(&coloursBuffer[i * 3]);
            } else
            {
                vertex.color = Vector3f(1.0f);
            }

            vertices.PushBack(vertex);

        }
}


void GLTFLoader::GetIndices(const tinygltf::Primitive& primitive, uint32 padding, TArray<uint32>& indices) {
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
                indices.PushBack(buf[i] + padding);
            }
            break;
        }
    case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
        {
            const uint16* buf = reinterpret_cast<const uint16*>(&buffer.data[bufferView.byteOffset + indexAccessor.byteOffset]);
            for (size_t i = 0; i < indexAccessor.count; i++)
            {
                indices.PushBack(buf[i] + padding);
            }
            break;
        }
    case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
        {
            const uint8 * buf = reinterpret_cast<const uint8*>(&buffer.data[bufferView.byteOffset + indexAccessor.byteOffset]);
            for (size_t i = 0; i < indexAccessor.count; i++) {
                indices.PushBack(buf[i] + padding);

            }
            break;
        }
    default:
        spdlog::error("Index Component type {} not supported", indexAccessor.componentType);
    }
}

void GLTFLoader::dbgModel() {
    for (auto &mesh : model.meshes) {
        std::cout << "mesh : " << mesh.name << std::endl;
        for (auto &primitive : mesh.primitives) {
            const tinygltf::Accessor &indexAccessor =
                model.accessors[primitive.indices];

            std::cout << "indexaccessor: count " << indexAccessor.count << ", type "
                      << indexAccessor.componentType << std::endl;

            // tinygltf::Material &mat = model.materials[primitive.material];
            // for (auto &mats : mat.values) {
            //     std::cout << "mat : " << mats.first.c_str() << std::endl;
            // }
            //
            // for (auto &image : model.images) {
            //     std::cout << "image name : " << image.uri << std::endl;
            //     std::cout << "  size : " << image.image.size() << std::endl;
            //     std::cout << "  w/h : " << image.width << "/" << image.height
            //               << std::endl;
            // }

            std::cout << "indices : " << primitive.indices << std::endl;
            std::cout << "mode     : "
                      << "(" << primitive.mode << ")" << std::endl;

            for (auto &attrib : primitive.attributes) {
                std::cout << "attribute : " << attrib.first.c_str() << std::endl;
            }
        }
    }
}