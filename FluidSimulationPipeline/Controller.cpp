#include "Controller.h"
#include "Renderer.h"
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>


Controller::Controller() {
}


void Controller::Tick(float deltaTime) {
    ProcessInput(deltaTime);
}


void Controller::SetFocusedRenderer(std::shared_ptr<Renderer> newFocusedRenderer) {
    if (newFocusedRenderer) {
        focusedRenderer = newFocusedRenderer;
        rendererUpdated = true;
    }
}

void Controller::ResetFocusedRenderer() {
    focusedRenderer = nullptr;
}


void Controller::ProcessInput(float deltaTime) {
    std::cout << "Base class has no implementation" << std::endl;
}

void Controller::ProcessMouseInput(const glm::ivec2& currClickPos, glm::ivec2& prevClickedPos, glm::vec2& mouseDir){
    std::cout << "Base class has no implementation" << std::endl;
}