//
// Created by nutellis on 16/07/2026.
//

#ifndef ALKYONERENDERENGINE_VULKANSHADEROBJECT_H
#define ALKYONERENDERENGINE_VULKANSHADEROBJECT_H

#include "rhi/core/shader/IShaderObject.h"


namespace slang {
    struct TypeLayoutReflection;
}

class VulkanShaderObject : public IShaderObject {
public:

    ~VulkanShaderObject() override;



    void write(ShaderOffset offset, const void *data, size_t size) override;

private:
    slang::TypeLayoutReflection* typeLayout;
};


#endif //ALKYONERENDERENGINE_VULKANSHADEROBJECT_H