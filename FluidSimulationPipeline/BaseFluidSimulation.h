#pragma once
#include <glm/glm.hpp>
#include <memory>


class FluidController2D;
class ComputeShader;


class BaseFluidSimulation
{
public:
	BaseFluidSimulation();
	~BaseFluidSimulation();

public:

protected:

	//Fluid sim controller
	FluidController2D* controller = nullptr;

	//fluid texture size
	unsigned int TEXTURE_WIDTH = 1024;
	unsigned int TEXTURE_HEIGHT = 1024;

	// shader varaibles
	unsigned int COMPUTE_LOCAL_SIZE_X = 8;
	unsigned int COMPUTE_LOCAL_SIZE_Y = 8;

	//number of itterations we run our poisson solver
	int projectionNumItterations = 100;

	//inverse of spatial step size
	float rdx = 1.0f;

	//force application variables
	glm::ivec2 previousClickedLocation{ -1,-1 };

	//force control variables
	float forceMultiple = 1000.0f;
	float impulseRadius = 200;

	//compute shaders
	std::unique_ptr<ComputeShader> advectionShader;
	std::unique_ptr<ComputeShader> forceShader;
	std::unique_ptr<ComputeShader> divergenceShader;
	std::unique_ptr<ComputeShader> solverShader;
	std::unique_ptr<ComputeShader> gradientCalculator;
	std::unique_ptr<ComputeShader> gradientSubtraction;
	std::unique_ptr<ComputeShader> boundaryShader;
	std::unique_ptr<ComputeShader> copyShader;


	//textures
	unsigned int velocityTexture;
	unsigned int copyVelocityTexture;
	unsigned int pressureTexture;
	unsigned int divTexture;


public:
	//Creates all needed shaders for fluid sim
	virtual void InitialiseFluidSimShaders();

	//Initialises all needed textures
	virtual void InitialiseFluidSimTextures();

	//Creates a 2D data texture
	void InitialiseDataTexture2D(unsigned int& dataTexture);
	
	//Run a single itteration of the fluid simulation
	virtual void RunFluidSimItteration(float fluidDeltaTime);

	//Determines the force to apply to the fluid and then applies it
	virtual void CalculateAndApplyForce(float deltaTime, const glm::vec2& normClickedLoc, const glm::vec2& normLastClickedLoc, const glm::vec2& mouseDir);

protected:
	//fluid simulation functions
	virtual void ApplyForce(unsigned int& velocityTexture, float deltaTime, float impulseRadius, glm::ivec2& clickedLocation, glm::ivec2& lastClickedLocation, glm::vec2& force, glm::ivec2& imageSize);
	
	// copies one texture into another
	virtual void CopyTexture(unsigned int& source, unsigned int& target);
	
	virtual void PerformAdvection(unsigned int& inputTexture, unsigned int& outputTexture, unsigned int& velocityTexture, float deltaTime, glm::ivec2& imageSize);
	
	//projection sub functions
	virtual void CalculateDivergence(unsigned int& divergenceTexture, unsigned int& inputTexture, float rdx, glm::ivec2& imageSize);
	virtual void SolvePoisson(unsigned int& divergenceTexture, unsigned int& pressureTexture, unsigned int& velocityTexture, float alpha, float rBeta, float omega, int numItterations, glm::ivec2& imageSize);
	virtual void SubtractPressureGrad(unsigned int& pressureTexture, unsigned int& velocityTexture, float rdx, glm::ivec2& imageSize);
	
	virtual void PerformProjection(unsigned int& velocityTexture, unsigned int& divergenceTexture, unsigned int& pressureTexture, glm::ivec2& imageSize);

	// disconnects fluid sim from current controller
	void ClearController();
	
public:
	//Getters and Setters
	void GetVelocityTexture(unsigned int& velocityTexture);
	void GetTextureSize(glm::ivec2& size);
	void SetController(FluidController2D* newController);
};

