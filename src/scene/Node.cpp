//
// Created by Nutellis on 24-Jan-26.
//

#include "scene/Node.h"

void Node::AddChild(Node* child)
{
    child->parent = this;
    children.push_back(child);
}

void Node::RemoveChild(const Node* child)
{
    for (auto i = 0; i < children.size(); i++) {
        if (children[i] == child) {
            children.erase(children.begin() + i);
            break;
        }
    }
}

void Node::RemoveAllChildren()
{
    children.clear();
}



Vector3f Node::GetPosition()
{
    return this->objectPosition;
}

void Node::SetPosition(Vector3f inPosition)
{
    if (isMovable) {
        if (parent == nullptr) {
            objectPosition = inPosition;
        }
        else { // why not parent.getposition + inPosition?
            objectPosition = parent->objectPosition + inPosition;
        }

        for (auto & i : children) {
            i->SetPosition(objectPosition);
        }

        // TODO: meshes need to update ttheir world bounding boxes.
        // if (ActorType == MODEL) {
        //     UpdateWorldBoundingBox();
        // }
    }
}

Vector3f Node::GetRotation()
{
    return objectRotation;
}

void Node::SetRotation(Vector3f inRotation)
{
    if (parent == nullptr) {
        objectRotation = inRotation;
    }
    else {
        objectRotation = parent->objectRotation + inRotation;
    }

    for (auto i = 0; i < children.size(); i++) {
        children[i]->SetRotation(objectRotation);
    }
    //TODO: need to care about children
    // if (ActorType == MODEL) {
    //     UpdateWorldBoundingBox();
    // }
}

void Node::AddRotation(Vector3f inRotation)
{
    objectRotation = inRotation;

}

void Node::resetOrientation()
{
    //TODO: resetOrientation
}

Vector3f Node::GetScale()
{
    //TODO: do something for uniform scaling? Maybe return a float?
    return objectScale;

}

void Node::SetScale(Vector3f inScale)
{
    objectScale = inScale;
    // if (ActorType == MODEL) {
    //     UpdateWorldBoundingBox();
    // }
}

void Node::SetScale(float inScale)
{
    objectScale = inScale;
    // if (ActorType == MODEL) {
    //     UpdateWorldBoundingBox();
    // }
}