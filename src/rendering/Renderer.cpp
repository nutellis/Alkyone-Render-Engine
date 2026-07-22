//
// Created by Nutellis on 13-Feb-26.
//

#include "rendering/Renderer.h"
#include <../../include/rendering/object/Mesh.h>
#include <resources/ResourceManager.h>

Renderer::Renderer(ResourceManager& resourceManager) : resourceManager(resourceManager)
{
}

Renderer::~Renderer()
{
}

bool Renderer::Initialize()
{
    return true;
}

void Renderer::Terminate()
{
}
