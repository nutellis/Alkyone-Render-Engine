//
// Created by Nutellis on 23-Jan-26.
//

#include "rendering/types/Vertex.h"

Vertex::Vertex(Vector3f Pos) : position(Pos), normal(Vector3f(0, 1, 0)), uvs(Vector2f(0.0f))
{
}

Vertex::Vertex(Vector3f Pos, Vector3f Norm, Vector2f UV): position(Pos), normal(Norm), uvs(UV)
{}

Vertex::Vertex(const Vertex& Other):
    position(Other.position),
    normal(Other.normal),
    uvs(Other.uvs)
{

}

Vertex::Vertex(Vertex&& Other) noexcept
{
    *this = std::move(Other);
}

Vertex& Vertex::operator=(const Vertex& Other)
{
    return *this;
}

Vertex& Vertex::operator=(Vertex&& Other) noexcept
{
    if (this != &Other) {
        position = Other.position;
        normal = Other.normal;
        uvs = Other.uvs;
    }
    return *this;
}

bool Vertex::operator==(const Vertex& other) const
{
    return position == other.position && normal == other.normal && uvs == other.uvs /*&& Color == other.Color  && Binormal == other.Binormal && Tangent == other.Tangent*/;
}
