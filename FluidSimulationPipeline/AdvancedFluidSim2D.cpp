#include "AdvancedFluidSim2D.h"
#include "FluidController2D.h"
#include "ComputeShader.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>




AdvancedFluidSim2D::AdvancedFluidSim2D() {
    //initialise relevant shader variables
    projectionNumItterations = 100;
    impulseRadius = 30.0f;
    forceMultiple = 1000.0f;

    //size of fluid simulation
    TEXTURE_WIDTH = 1024;
    TEXTURE_HEIGHT = 1024;

    //initialise shaders
    InitialiseFluidSimShaders();
    InitialiseFluidSimTextures();
}

AdvancedFluidSim2D::~AdvancedFluidSim2D() {
}


void AdvancedFluidSim2D::InitialiseFluidSimShaders() {
    advectionShader = std::unique_ptr<ComputeShader>(new ComputeShader("AdvancedAdvection.comp"));
    forceShader = std::unique_ptr<ComputeShader>(new ComputeShader("ForceApplication.comp"));
    divergenceShader = std::unique_ptr<ComputeShader>(new ComputeShader("MACDivergence.comp"));
    solverShader = std::unique_ptr<ComputeShader>(new ComputeShader("SOR.comp"));
    gradientSubtraction = std::unique_ptr<ComputeShader>(new ComputeShader("MACGradientSubtraction.comp"));
    //boundaryShader = std::unique_ptr<ComputeShader>(new ComputeShader("BoundaryConditions.comp")); //Future update
    copyShader = std::unique_ptr<ComputeShader>(new ComputeShader("CopyTexture.comp"));
}


void AdvancedFluidSim2D::ApplyForce(unsigned int& velocityTexture, float deltaTime, float impulseRadius, glm::ivec2& clickedLocation, glm::ivec2& lastClickedLocation, glm::vec2& force, glm::ivec2& imageSize) {
    forceShader->Use();

    glBindImageTexture(0, velocityTexture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);

    forceShader->SetFloat("timeStep", deltaTime);
    forceShader->SetFloat("impulseRadius", impulseRadius);
    forceShader->SetIVec2("clickedLocation", clickedLocation);
    forceShader->SetIVec2("lastClickedLocation", lastClickedLocation);
    forceShader->SetVec2("force", force);
    forceShader->SetIVec2("imageSize", imageSize);

    glDispatchCompute((unsigned int)TEXTURE_WIDTH / COMPUTE_LOCAL_SIZE_X, (unsigned int)TEXTURE_HEIGHT / COMPUTE_LOCAL_SIZE_Y, 1);

    // make sure writing to image has finished before read
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}

void AdvancedFluidSim2D::CopyTexture(unsigned int& source, unsigned int& target) {
    copyShader->Use();
    glBindImageTexture(0, source, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
    glBindImageTexture(1, target, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
    glDispatchCompute((unsigned int)TEXTURE_WIDTH / COMPUTE_LOCAL_SIZE_X, (unsigned int)TEXTURE_HEIGHT / COMPUTE_LOCAL_SIZE_Y, 1);

    // make sure writing to image has finished before read
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}

void AdvancedFluidSim2D::PerformAdvection(unsigned int& inputTexture, unsigned int& outputTexture, unsigned int& velocityTexture, float deltaTime, glm::ivec2& imageSize) {
    advectionShader->Use();
    glBindImageTexture(0, inputTexture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
    glBindImageTexture(1, outputTexture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
    glBindImageTexture(2, velocityTexture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);

    advectionShader->SetFloat("timeStep", deltaTime);
    advectionShader->SetIVec2("imageSize", imageSize);

    glDispatchCompute((unsigned int)TEXTURE_WIDTH / COMPUTE_LOCAL_SIZE_X, (unsigned int)TEXTURE_HEIGHT / COMPUTE_LOCAL_SIZE_Y, 1);

    // make sure writing to image has finished before read
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}

void AdvancedFluidSim2D::CalculateDivergence(unsigned int& divergenceTexture, unsigned int& inputTexture, float rdx, glm::ivec2& imageSize) {
    divergenceShader->Use();
    glBindImageTexture(0, inputTexture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);

    glBindImageTexture(1, divergenceTexture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
    divergenceShader->SetFloat("rdx", rdx);
    divergenceShader->SetIVec2("imageSize", imageSize);

    glDispatchCompute((unsigned int)TEXTURE_WIDTH / COMPUTE_LOCAL_SIZE_X, (unsigned int)TEXTURE_HEIGHT / COMPUTE_LOCAL_SIZE_Y, 1);

    // make sure writing to image has finished before read
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}

void AdvancedFluidSim2D::SolvePoisson(unsigned int& divergenceTexture, unsigned int& pressureTexture, unsigned int& velocityTexture, float alpha, float rBeta, float omega, int numItterations, glm::ivec2& imageSize) {
    solverShader->Use();
    glBindImageTexture(0, pressureTexture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
    glBindImageTexture(1, divergenceTexture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
    glBindImageTexture(2, velocityTexture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);

    solverShader->SetFloat("alpha", alpha);
    solverShader->SetFloat("rBeta", rBeta);
    solverShader->SetIVec2("imageSize", imageSize);
    solverShader->SetFloat("omega", omega);

    int itterationCount = 0;
    bool flipCheckBoard = false;
    //reruns itterative poisson solver 
    for (int i = 0; i < numItterations; i++) {
        itterationCount++;
        //flip focused grid section checkerboard style (property of solver)
        for (int j = 0; j < 2; j++) {
            if (flipCheckBoard) {
                solverShader->SetInt("offset", 0);
            }
            else {
                solverShader->SetInt("offset", 1);
            }
            glDispatchCompute((unsigned int)TEXTURE_WIDTH / (2 * COMPUTE_LOCAL_SIZE_X), (unsigned int)TEXTURE_HEIGHT / COMPUTE_LOCAL_SIZE_Y, 1);
            // make sure writing to image has finished before read
            glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

            flipCheckBoard = !flipCheckBoard;
        }
    }
}

void AdvancedFluidSim2D::SubtractPressureGrad(unsigned int& pressureTexture, unsigned int& velocityTexture, float rdx, glm::ivec2& imageSize) {
    gradientSubtraction->Use();
    glBindImageTexture(0, pressureTexture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
    glBindImageTexture(1, velocityTexture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);

    gradientSubtraction->SetIVec2("imageSize", imageSize);
    gradientSubtraction->SetFloat("rdx", rdx);

    glDispatchCompute((unsigned int)TEXTURE_WIDTH / COMPUTE_LOCAL_SIZE_X, (unsigned int)TEXTURE_HEIGHT / COMPUTE_LOCAL_SIZE_Y, 1);
    // make sure writing to image has finished before read
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}

void AdvancedFluidSim2D::PerformProjection(unsigned int& velocityTexture, unsigned int& divergenceTexture, unsigned int& pressureTexture, glm::ivec2& imageSize) {
    CalculateDivergence(divergenceTexture, velocityTexture, rdx, imageSize);
    SolvePoisson(divergenceTexture, pressureTexture, velocityTexture, projectionAlpha, projectionrBeta, projectionOmega, projectionNumItterations, imageSize);
    SubtractPressureGrad(pressureTexture, velocityTexture, rdx, imageSize);
}


void AdvancedFluidSim2D::RunFluidSimItteration(float fluidDeltaTime) {
    glm::ivec2 imageSize = { TEXTURE_WIDTH , TEXTURE_HEIGHT };

    CopyTexture(velocityTexture, copyVelocityTexture);

    PerformAdvection(copyVelocityTexture, velocityTexture, copyVelocityTexture, fluidDeltaTime, imageSize);

    PerformProjection(velocityTexture, divTexture, pressureTexture, imageSize);
}


void AdvancedFluidSim2D::CalculateAndApplyForce(float deltaTime, const glm::vec2& normClickedLoc, const glm::vec2& normLastClickedLoc, const glm::vec2& mouseDir) {
    BaseFluidSimulation::CalculateAndApplyForce(deltaTime, normClickedLoc, normLastClickedLoc, mouseDir);
}