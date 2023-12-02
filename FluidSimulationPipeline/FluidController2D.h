#pragma once
#include "Controller.h"


class BaseFluidSimulation;

class FluidController2D : public Controller
{
public:
	FluidController2D();

protected:
	//fluid sim being controlled
	std::shared_ptr<BaseFluidSimulation> focusedFluidSim;

	// Mouse control variables
	
	//default values
	const float minMouseMovement = 2.0f;
	const glm::vec2 defaultMouseMovement{ 1,0 };
	
	//holds previous mouse values
	glm::ivec2 lastClickedPos;
	glm::vec2 lastMouseMovement{ 1,0 };

	//click control booleans
	bool clicked = false;
	bool firstClick = true;
	

	bool fluidTextureUpdated = false;
	

public:
	//Run every frame
	virtual void Tick(float deltaTime) override;

	
	//Sets the focused fluid simulation
	virtual void SetFluidSim(std::shared_ptr<BaseFluidSimulation> focus);
	void ResetFluidSim();

protected:
	//processes input from the relevant window
	virtual void ProcessInput(float deltaTime) override;
	virtual void ProcessMouseInput(const glm::ivec2& currClickPos, glm::ivec2& prevClickedPos, glm::vec2& mouseDir) override;
};

