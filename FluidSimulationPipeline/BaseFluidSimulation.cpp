#include "BaseFluidSimulation.h"
#include "FluidController2D.h"
#include "ComputeShader.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>




BaseFluidSimulation::BaseFluidSimulation() {
    //initialise shaders
    InitialiseFluidSimShaders();
    InitialiseFluidSimTextures();
}

BaseFluidSimulation::~BaseFluidSimulation() {
    //removes link to current controller
    ClearController();
    //delete textures on close
    glDeleteTextures(1, &velocityTexture);
    glDeleteTextures(1, &copyVelocityTexture);
    glDeleteTextures(1, &pressureTexture);
    glDeleteTextures(1, &divTexture);
}


void BaseFluidSimulation::InitialiseFluidSimShaders() {

}


void BaseFluidSimulation::InitialiseFluidSimTextures() {
    InitialiseDataTexture2D(velocityTexture);
    InitialiseDataTexture2D(copyVelocityTexture);
    InitialiseDataTexture2D(pressureTexture);
    InitialiseDataTexture2D(divTexture);
}


void BaseFluidSimulation::InitialiseDataTexture2D(unsigned int& dataTexture) {
    glGenTextures(1, &dataTexture);
    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, dataTexture);
    // Give an empty image to OpenGL ( the last "0" )
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);

    // Poor filtering. Needed !
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

void BaseFluidSimulation::RunFluidSimItteration(float fluidDeltaTime) {
    glm::ivec2 imageSize = { TEXTURE_WIDTH , TEXTURE_HEIGHT };

    CopyTexture(velocityTexture, copyVelocityTexture);

    PerformAdvection(copyVelocityTexture, velocityTexture, copyVelocityTexture, fluidDeltaTime, imageSize);

    PerformProjection(velocityTexture, divTexture, pressureTexture, imageSize);
}


void BaseFluidSimulation::CalculateAndApplyForce(float deltaTime, const glm::vec2& normClickedLoc, const glm::vec2& normLastClickedLoc, const glm::vec2& mouseDir) {
    glm::ivec2 clickedLocation{ TEXTURE_WIDTH * normClickedLoc[0], TEXTURE_HEIGHT * normClickedLoc[1] };
    glm::ivec2 lastClickedLocation{ TEXTURE_WIDTH * normLastClickedLoc[0], TEXTURE_HEIGHT * normLastClickedLoc[1] };
    glm::vec2 force = forceMultiple * mouseDir;
    glm::ivec2 imageSize = { TEXTURE_WIDTH , TEXTURE_HEIGHT };
    ApplyForce(velocityTexture, deltaTime, impulseRadius, clickedLocation, lastClickedLocation, force, imageSize);
}

void BaseFluidSimulation::ApplyForce(unsigned int& velocityTexture, float deltaTime, float impulseRadius, glm::ivec2& clickedLocation, glm::ivec2& lastClickedLocation, glm::vec2& force, glm::ivec2& imageSize) {
}

void BaseFluidSimulation::CopyTexture(unsigned int& source, unsigned int& target) {
}

void BaseFluidSimulation::PerformAdvection(unsigned int& inputTexture, unsigned int& outputTexture, unsigned int& velocityTexture, float deltaTime, glm::ivec2& imageSize) {
}

void BaseFluidSimulation::CalculateDivergence(unsigned int& divergenceTexture, unsigned int& inputTexture, float rdx, glm::ivec2& imageSize) {
}

void BaseFluidSimulation::SolvePoisson(unsigned int& divergenceTexture, unsigned int& pressureTexture, unsigned int& velocityTexture, float alpha, float rBeta, float omega, int numItterations, glm::ivec2& imageSize) {
    
}

void BaseFluidSimulation::SubtractPressureGrad(unsigned int& pressureTexture, unsigned int& velocityTexture, float rdx, glm::ivec2& imageSize) {
}

void BaseFluidSimulation::PerformProjection(unsigned int& velocityTexture, unsigned int& divergenceTexture, unsigned int& pressureTexture, glm::ivec2& imageSize) {
}

void BaseFluidSimulation::ClearController() {
    if (controller != nullptr) {
        controller->ResetFluidSim();
    }
    controller = nullptr;
}


void BaseFluidSimulation::GetVelocityTexture(unsigned int& velocityTexture) {
    velocityTexture = this->velocityTexture;
}


void BaseFluidSimulation::GetTextureSize(glm::ivec2& size) {
    size = glm::ivec2{TEXTURE_WIDTH, TEXTURE_HEIGHT};
}

void BaseFluidSimulation::SetController(FluidController2D* newController) {
    if (controller != nullptr) {
        controller->ResetFluidSim();
    }
    controller = newController;
}
