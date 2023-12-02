#pragma once
#include "BaseFluidSimulation.h"

class AdvancedFluidSim2D : public BaseFluidSimulation
{
public:
	AdvancedFluidSim2D();
	~AdvancedFluidSim2D();

protected:
	//projection algorithm variables
	float projectionAlpha = 1.0f;
	float projectionrBeta = 0.25f;
	float projectionOmega = 1.7f;

protected:
	//overrides of base class functions

	virtual void InitialiseFluidSimShaders() override;

	//fluid simulation functions
	virtual void ApplyForce(unsigned int& velocityTexture, float deltaTime, float impulseRadius, glm::ivec2& clickedLocation, glm::ivec2& lastClickedLocation, glm::vec2& force, glm::ivec2& imageSize) override;
	// copies one texture into another
	virtual void CopyTexture(unsigned int& source, unsigned int& target) override;
	virtual void PerformAdvection(unsigned int& inputTexture, unsigned int& outputTexture, unsigned int& velocityTexture, float deltaTime, glm::ivec2& imageSize) override;
	
	//projection sub functions
	virtual void CalculateDivergence(unsigned int& divergenceTexture, unsigned int& inputTexture, float rdx, glm::ivec2& imageSize) override;
	virtual void SolvePoisson(unsigned int& divergenceTexture, unsigned int& pressureTexture, unsigned int& velocityTexture, float alpha, float rBeta, float omega, int numItterations, glm::ivec2& imageSize) override;
	virtual void SubtractPressureGrad(unsigned int& pressureTexture, unsigned int& velocityTexture, float rdx, glm::ivec2& imageSize) override;
	
	virtual void PerformProjection(unsigned int &velocityTexture, unsigned int &divergenceTexture, unsigned int &pressureTexture, glm::ivec2& imageSize) override;
	

public:
	//Functions for interacting with fluid sim
	virtual void RunFluidSimItteration(float fluidDeltaTime) override;
	virtual void CalculateAndApplyForce(float deltaTime, const glm::vec2& normClickedLoc, const glm::vec2& normLastClickedLoc, const glm::vec2& mouseDir) override;
};

