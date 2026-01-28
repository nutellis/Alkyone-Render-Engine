//
// Created by Nutellis on 23-Jan-26.
//

#ifndef ALKYONERENDERENGINE_VERTEX_H
#define ALKYONERENDERENGINE_VERTEX_H


#include <math/Vector2.h>
#include <math/Vector3.h>

struct alignas(4) Vertex {
public:
    // position
    Vector3f position {};
    // normal
    Vector3f normal {};
    // texCoords
    Vector2f uvs {};

    //color
    Vector3f color {};
    // bitangent
    //Vector3f Binormal;
    // tangent
    //Vector3f Tangent;

    Vertex() = default;
    ~Vertex() = default;

    Vertex(Vector3f Pos);
    Vertex(Vector3f Pos, Vector3f Norm, Vector2f UV);

    Vertex(const Vertex& Other);

    Vertex(Vertex&& Other) noexcept;

    Vertex& operator=(const Vertex& Other);

    Vertex& operator=( Vertex&& Other) noexcept ;

    bool operator==(const Vertex& other) const;
};


#endif //ALKYONERENDERENGINE_VERTEX_H