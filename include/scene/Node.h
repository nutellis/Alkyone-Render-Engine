//
// Created by Nutellis on 24-Jan-26.
//

#ifndef ALKYONERENDERENGINE_NODE_H
#define ALKYONERENDERENGINE_NODE_H
#include <string>
#include <vector>

#include "math/Vector3.h"


class Node
{
public:
    Node() = default;
    virtual ~Node() = default;

    virtual bool Initialize() = 0;
    virtual void Terminate() = 0;
    virtual void Update() = 0;

    void AddChild(Node* child);
    void RemoveChild(const Node* child);
    void RemoveAllChildren();


    // position, rotation, scale
    Vector3f GetPosition();

    //TODO : always relative to the parent SceneNode
    void SetPosition(Vector3f inPosition);

    //You can move the object relative to its current position by using the translate method.
    //void Translate(Vector3f Translation);

    Vector3f GetRotation();

    //TODO : always relative to the parent SceneNode
    //You can use the pitch, yaw, and roll functions to rotate objects.
    void SetRotation(Vector3f inRotation);
    void AddRotation(Vector3f inRotation);
    void resetOrientation();
    Vector3f GetScale();
    void SetScale(Vector3f inScale);
    void SetScale(float inScale);

protected:

    std::string objectName {};

    Node * parent = nullptr;
    std::vector<Node *> children;

    uint32 nodeID {};
    // Stores the position/translation of the node
    Vector3f objectPosition {};

    // Stores the rotation of the node.
    Vector3f objectRotation {};

    //Quaternion Orientation;
    //...

    // Stores the uniform scaling factor applied to this node if relevant
    Vector3f objectScale {1.0f, 1.0f, 1.0f};

    bool isMovable {};

};


#endif //ALKYONERENDERENGINE_NODE_H