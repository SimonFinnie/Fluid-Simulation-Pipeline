#include "ComputeShader.h"

#include <fstream>
#include <sstream>
#include <iostream>


ComputeShader::ComputeShader(const char* computePath) {
    //file name
    std::string computeCode;
    std::ifstream cShaderFile;
    // add exceptions
    cShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        // open files
        cShaderFile.open(computePath);
        std::stringstream cShaderStream;
        // read file's buffer contents into streams
        cShaderStream << cShaderFile.rdbuf();
        // close file handlers
        cShaderFile.close();
        // convert stream into string
        computeCode = cShaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
    }
    const char* cShaderCode = computeCode.c_str();

	unsigned int compute;
	// compute shader
	compute = glCreateShader(GL_COMPUTE_SHADER);
	glShaderSource(compute, 1, &cShaderCode, NULL);
	glCompileShader(compute);
	CheckCompileErrors(compute, "COMPUTE");

	// shader Program
    ShaderProgramID = glCreateProgram();
	glAttachShader(ShaderProgramID, compute);
	glLinkProgram(ShaderProgramID);
	CheckCompileErrors(ShaderProgramID, "PROGRAM");

    glDeleteShader(compute);
}

ComputeShader::~ComputeShader() {
    glDeleteProgram(ShaderProgramID);
}


