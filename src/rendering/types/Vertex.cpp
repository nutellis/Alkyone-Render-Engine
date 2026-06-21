//
// Created by Nutellis on 23-Jan-26.
//

#include "rendering/types/Vertex.h"

Vertex::Vertex(Float3 Pos) : position(Pos), normal(Float3(0, 1, 0)), uvs(Float2(0.0f))
{
}

Vertex::Vertex(Float3 Pos, Float3 Norm, Float2 UV): position(Pos), normal(Norm), uvs(UV)
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
    return position == other.position && normal == other.normal && uvs == other.uvs && color == other.color /* && Binormal == other.Binormal && Tangent == other.Tangent*/;
}
