#pragma once

#include "shader.h"
#include <memory>

class IRenderPass {
public:
    std::shared_ptr<Shader> shader;

    IRenderPass() = default;
    virtual ~IRenderPass() = default;

    virtual void init() = 0;
    virtual void update() = 0;
    virtual void render() = 0;
};
