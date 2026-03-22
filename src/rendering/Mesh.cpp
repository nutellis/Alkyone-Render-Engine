//
// Created by Nutellis on 23-Jan-26.
//

#include "rendering/Mesh.h"
#include <rendering/types/Vertex.h>

#include "rhi/core/IBuffer.h"

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
}

bool Mesh::Initialize()
{

    return true;
}

void Mesh::Terminate()
{
    buffer->Terminate();
}
