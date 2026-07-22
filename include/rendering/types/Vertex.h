//
// Created by Nutellis on 23-Jan-26.
//

#ifndef ALKYONERENDERENGINE_VERTEX_H
#define ALKYONERENDERENGINE_VERTEX_H


#include <math/Vector2.h>
#include <math/Vector3.h>
#include "math/Vector4.h"

struct alignas(4) Vertex {
public:
    // position
    Float3 position {};
    // normal
    Float3 normal {};
    // texCoords
    Float2 uvs {};

    //color
    Float4 color {};
    // bitangent
    //Vector3f Binormal;
    // tangent
    //Vector3f Tangent;

    Vertex() = default;
    ~Vertex() = default;

    Vertex(Float3 Pos);
    Vertex(Float3 Pos, Float3 Norm, Float2 UV);

    // Vertex(const Vertex& Other);
    //
    // Vertex(Vertex&& Other) noexcept;
    //
    // Vertex& operator=(const Vertex& Other);
    //
    // Vertex& operator=(Vertex&& Other) noexcept ;

    bool operator==(const Vertex& other) const;
};


#endif //ALKYONERENDERENGINE_VERTEX_H