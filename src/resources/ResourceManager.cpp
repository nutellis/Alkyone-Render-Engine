//
// Created by Nutellis on 13-Jun-26.
//

#include "resources/ResourceManager.h"
#include <rhi/core/DynamicRHI.h>
#include <rendering/object/MeshGroup.h>
#include <rendering/object/GPUMesh.h>

#include "rendering/types/Vertex.h"
#include "rhi/descriptors/BufferDesc.h"
#include <utilities/TypeUtilities.h>

#include "rendering/object/Mesh.h"
#include "rhi/core/IBuffer.h"
#include "spdlog/spdlog.h"

ResourceManager::ResourceManager(DynamicRHI& rhi) : rhi(rhi)
{
}

ResourceManager::~ResourceManager()
{
}

bool ResourceManager::Initialize()
{
    return true;
}

void ResourceManager::Terminate()
{
}

void ResourceManager::UploadMeshes(std::vector<MeshGroup>& meshGroups)
{
    for (MeshGroup& meshGroup : meshGroups)
    {
        size_t verticesByteSize = meshGroup.vertices.size() * sizeof(Vertex);
        size_t indicesByteSize = meshGroup.indices.size() * sizeof(uint32);
        size_t totalSize = verticesByteSize + indicesByteSize;

        BufferDesc stagingDesc = {
            .size = totalSize,
            .usageFlags = BufferTypeBits::TRANSFER_SRC,
            .sharingMode = SharingMode::EXCLUSIVE,
            .memoryUsageStrategy = MemoryUsageStrategy::CPU_TO_GPU
        };

        Handle stagingBufferHandle = rhi.CreateBuffer(stagingDesc);
        IBuffer* stagingBuffer = rhi.GetBuffer(stagingBufferHandle);

        void * hostData = stagingBuffer->Map();

        memcpy(hostData, meshGroup.vertices.data(), verticesByteSize);

        void * indexDestination = static_cast<char*>(hostData) + verticesByteSize;
        memcpy(indexDestination, meshGroup.indices.data(), indicesByteSize);

        stagingBuffer->Unmap();

        BufferDesc gpuDesc = {
            .size = totalSize,
            .usageFlags = BufferTypeBits::VERTEX_BUFFER | BufferTypeBits::INDEX_BUFFER | BufferTypeBits::TRANSFER_DST,
            .sharingMode = SharingMode::EXCLUSIVE,
            .memoryUsageStrategy = MemoryUsageStrategy::GPU_ONLY
        };
        Handle gpuBufferHandle = rhi.CreateBuffer(gpuDesc);

        for (Mesh& subMesh : meshGroup.subMeshes)
        {
            GPUMesh gpuMesh = {
                .firstIndex = subMesh.firstIndex,
                .indexCount = subMesh.indexCount,
                .vertexOffset = subMesh.vertexOffset,
                .indexBufferOffset = static_cast<uint32>(verticesByteSize),
                .bufferHandle = gpuBufferHandle
            };

            subMesh.meshIndexHandle = meshRegistry.createSlot(std::move(gpuMesh));
        }

        rhi.SubmitCopyBuffer(stagingBufferHandle, gpuBufferHandle, totalSize);
        rhi.DestroyBuffer(stagingBufferHandle);
        spdlog::info("ResourceManager: Uploaded mesh group to GPU. Total size: {} bytes",totalSize);
    }
}
