//
// Created by Nutellis on 13-Jun-26.
//

#include "resources/ResourceManager.h"

#include <span>
#include <rhi/core/DynamicRHI.h>
#include <rendering/object/MeshGroup.h>


#include "rendering/types/Vertex.h"
#include "rhi/descriptors/BufferDesc.h"
#include <utilities/TypeUtilities.h>

#include "resources/GLTFImporter.h"
#include "rhi/descriptors/ShaderCompileDesc.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/bundled/os.h"

ResourceManager::ResourceManager(DynamicRHI &rhi) : rhi(rhi) {
}

ResourceManager::~ResourceManager() {
}

bool ResourceManager::Initialize() {
    return true;
}

void ResourceManager::Terminate() {
}

void ResourceManager::LoadSceneGeometry(ParsedData &parsedData) {
    UploadMeshData(parsedData.globalVertices, parsedData.globalIndices);

    std::vector<Handle> meshHandles;
    meshHandles.reserve(parsedData.sceneMeshes.size());
    for (MeshGroup& group : parsedData.sceneMeshes) {
        parsedData.engineMeshHandles.push_back(meshRegistry.createSlot(std::move(group)));
    }
}

void ResourceManager::UploadMeshData(std::span<const Vertex> vertices, std::span<const uint32> indices) {

    size_t verticesSize = vertices.size() * sizeof(Vertex);
    size_t indicesSize = indices.size() * sizeof(uint32);
    size_t totalSize = verticesSize + indicesSize;
    BufferDesc stagingDesc = {
        .size = totalSize,
        .usageFlags = BufferTypeBits::TRANSFER_SRC,
        .sharingMode = SharingMode::EXCLUSIVE,
        .memoryUsageStrategy = MemoryUsageStrategy::CPU_TO_GPU
    };

    BufferHandle stagingBufferHandle = rhi.CreateBuffer(stagingDesc);

    rhi.HostCopyBuffer(stagingBufferHandle, vertices.data(), verticesSize, 0);
    rhi.HostCopyBuffer(stagingBufferHandle,indices.data(), indicesSize, verticesSize);

    BufferDesc gpuDesc = {
        .size = totalSize,
        .usageFlags = BufferTypeBits::VERTEX_BUFFER | BufferTypeBits::INDEX_BUFFER |
                      BufferTypeBits::TRANSFER_DST,
        .sharingMode = SharingMode::EXCLUSIVE,
        .memoryUsageStrategy = MemoryUsageStrategy::GPU_ONLY
    };
    BufferHandle gpuBufferHandle = rhi.CreateBuffer(gpuDesc);

    rhi.SubmitCopyBuffer({rhi.RecordCopyBuffer(
        stagingBufferHandle, gpuBufferHandle, totalSize,
        0, 0)});
    rhi.DestroyBuffer(stagingBufferHandle);

    spdlog::info("ResourceManager: Uploaded mesh data to GPU. Total size: {} bytes", totalSize);

    megaBufferHandle = gpuBufferHandle;
}

ShaderHandle ResourceManager::LoadShader(const std::string &filename, const std::string &vertexEntryName,
                                         const std::string &fragmentEntryName) {
    //TODO: probably do some checks on the filename...
    if (shaderCache.contains(filename)) {
        return shaderCache[filename];
    }

    ShaderCompileDesc desc = {
        filename, vertexEntryName, fragmentEntryName
    };
    ShaderHandle handle = rhi.CreateShader(desc);
    if (handle.resourceId == Handle::Invalid()) {
        spdlog::info("ResourceManager: Failed to load shader: {}", filename);
        return handle;
    }

    shaderCache.insert({filename, handle});
    return handle;
}
