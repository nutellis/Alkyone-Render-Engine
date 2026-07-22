//
// Created by nutellis on 16/07/2026.
//

#include "ShaderCursor.h"

#include "IShaderObject.h"

ShaderCursor ShaderCursor::field(const char * name) {
    return field(typeLayout->findFieldIndexByName(name));
}

ShaderCursor ShaderCursor::field(int32 index) {
    slang::VariableLayoutReflection* field = typeLayout->getFieldByIndex(index);

    ShaderCursor result;

    result.baseObject = baseObject;
    result.typeLayout = field->getTypeLayout();
    result.offset.uniformOffset = offset.uniformOffset + field->getOffset();
    result.offset.bindingRangeIndex = offset.bindingRangeIndex + static_cast<uint32>(typeLayout->getFieldBindingRangeOffset(index));

    result.offset.bindingRangeIndex = offset.bindingRangeIndex;

    return result;
}

ShaderCursor ShaderCursor::element(int32 index) {
    slang::TypeLayoutReflection* elementTypeLayout = typeLayout->getElementTypeLayout();

    ShaderCursor result;
    result.typeLayout = elementTypeLayout;
    result.offset.uniformOffset += index * elementTypeLayout->getStride();

    result.offset.bindingArrayIndex *= typeLayout->getElementCount();
    result.offset.bindingArrayIndex += index;

    return result;
}

void ShaderCursor::write(const void *data, size_t size) {
    baseObject->write(offset, data, size);
}
