#pragma once
#include "Shader.h"

class ComputeShader : public Shader
{
    public:
        ComputeShader(const char* computePath);
        ~ComputeShader();    
};

