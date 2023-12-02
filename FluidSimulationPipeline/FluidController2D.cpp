#include "FluidController2D.h"
#include "BaseFluidSimulation.h"
#include "Renderer.h"
#include <GLFW/glfw3.h>

#include <iostream>


FluidController2D::FluidController2D() {
    lastClickedPos = glm::ivec2{ -1, -1 };
}

void FluidController2D::Tick(float deltaTime) {
    unsigned int velTexture = 0;
    //runs fluid simulation
    if (focusedFluidSim && focusedRenderer) {
        focusedFluidSim->RunFluidSimItteration(deltaTime);
        Controller::Tick(deltaTime);
        //updates fluid simulation frame in renderer
        if (fluidTextureUpdated || rendererUpdated) {
            focusedFluidSim->GetVelocityTexture(velTexture);
            focusedRenderer->SetVisualTexture(velTexture);
            fluidTextureUpdated = false;
            rendererUpdated = false;
        }
    }

}


void FluidController2D::SetFluidSim(std::shared_ptr<BaseFluidSimulation> focus) {
    if (focus) {
        focusedFluidSim = focus;
        focusedFluidSim->SetController(this);
        fluidTextureUpdated = true;
    }
}

void FluidController2D::ResetFluidSim() {
    focusedFluidSim.reset();
}

void FluidController2D::ProcessInput(float deltaTime) {
    if (focusedRenderer) {
        GLFWwindow* focusedWindow = focusedRenderer->GetWindow();
        double xPos = 0, yPos = 0;
        //handles inputs
        if (glfwGetMouseButton(focusedWindow, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
            glfwGetCursorPos(focusedWindow, &xPos, &yPos);
            float xPosF = static_cast<float>(xPos), yPosF = static_cast<float>(yPos);
            if (clicked && focusedFluidSim) {
                glm::ivec2 windowSize;
                focusedRenderer->GetWindowSize(windowSize);

                glm::ivec2 clickedLoc{ xPosF, windowSize.y - yPosF };
                glm::ivec2 prevClickedLoc{ 0, 0 };
                glm::vec2 mouseDir{ 0,0 };
                ProcessMouseInput(clickedLoc, prevClickedLoc, mouseDir);
                //normalises input
                glm::vec2 normClickedLoc{ xPosF/ windowSize.x, 1.0f - yPosF/ windowSize.y };
                float prevXPosF = prevClickedLoc.x;
                float prevYPosF = prevClickedLoc.y;
                glm::vec2 normLastClickedLoc{ prevXPosF / windowSize.x, prevYPosF / windowSize.y };
                //applies force
                focusedFluidSim->CalculateAndApplyForce(deltaTime, normClickedLoc, normLastClickedLoc, mouseDir);
            }
            clicked = true;
        }
        if (glfwGetMouseButton(focusedWindow, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
            clicked = false;
            firstClick = true;
        }
    }
}


void FluidController2D::ProcessMouseInput(const glm::ivec2& currClickPos, glm::ivec2& prevClickedPos, glm::vec2& mouseDir) {
    //handles default values
    if (firstClick) {
        lastClickedPos = currClickPos;
        lastMouseMovement = defaultMouseMovement;
        firstClick = false;
    }
    //checks for mouse movement
    glm::vec2 dirVec = currClickPos - lastClickedPos;
    if (glm::length(dirVec) < minMouseMovement) {
        mouseDir = lastMouseMovement;
    }
    else {
        mouseDir = glm::normalize(dirVec);
    }
    //updates variables
    prevClickedPos = lastClickedPos;


    lastClickedPos = currClickPos;
    lastMouseMovement = mouseDir;
}


