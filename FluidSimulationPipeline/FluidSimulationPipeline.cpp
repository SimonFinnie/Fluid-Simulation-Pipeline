// FluidSimulationPipeline.cpp : Defines the entry point for the application.
//

#include <iostream>
#include "Renderer.h"
#include "FluidController2D.h"
#include "AdvancedFluidSim2D.h"
#include <chrono>
#include <thread>


int main()
{
    //Create renderer, sim, and controller, and sets their relevant variables.
    std::shared_ptr<Renderer> renderer = std::shared_ptr<Renderer>(new Renderer());
    FluidController2D fluidController{};
    std::shared_ptr<AdvancedFluidSim2D> fluidSim = std::shared_ptr<AdvancedFluidSim2D>(new AdvancedFluidSim2D());
    fluidController.SetFluidSim(fluidSim);
    fluidController.SetFocusedRenderer(renderer);
    //remove local variable
    fluidSim.reset();

    //run render loop
    bool programRunning = true;
    //sets a fixed deltatime for the fluid simulation 
    // (currently not forced to run at this framerate, so will be faster or slower based on framerate)
    float fluidDeltaTime = 1.0f / 60.0f;
    while (programRunning) {
        fluidController.Tick(fluidDeltaTime);
        programRunning = renderer->RenderFrame();
    }
}

