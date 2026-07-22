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



Float3 Node::GetPosition()
{
    return this->objectPosition;
}

void Node::SetPosition(Float3 inPosition)
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

Float3 Node::GetRotation()
{
    return objectRotation;
}

void Node::SetRotation(Float3 inRotation)
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

void Node::AddRotation(Float3 inRotation)
{
    objectRotation = inRotation;

}

void Node::resetOrientation()
{
    //TODO: resetOrientation
}

Float3 Node::GetScale()
{
    //TODO: do something for uniform scaling? Maybe return a float?
    return objectScale;

}

void Node::SetScale(Float3 inScale)
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