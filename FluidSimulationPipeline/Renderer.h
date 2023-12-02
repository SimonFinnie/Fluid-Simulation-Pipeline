#pragma once
#include <glm/glm.hpp>
#include "RenderCamera.h"
#include <memory>


struct GLFWwindow;
class Shader;


class Renderer
{

public:
	Renderer();
	~Renderer();

private:
	float vertices2D[20] = {
	 -1.0f, 1.0f, -1.0f, 0.0f, 1.0f,  // top right
	 -1.0f,-1.0f, -1.0f, 0.0f, 0.0f, // bottom right
	  1.0f, 1.0f, -1.0f, 1.0f, 1.0f, // bottom left
	  1.0f,-1.0f, -1.0f, 1.0f, 0.0f // top left 
	};
	unsigned int indices2D[6] = {  // note that we start from 0!
		0, 1, 2,   // first triangle
		1, 2, 3    // second triangle
	};

	//destructor variable
	bool shaderClosed = false;

	// timing 
	float deltaTime = 0.0f; // time between current frame and last frame
	float lastFrame = 0.0f; // time of last frame

	// settings
	int SCR_WIDTH = 1024;
	int SCR_HEIGHT = 1024;

	//Camera
	RenderCamera renderCamera;

	//visual window
	GLFWwindow* window;

	//Buffer objects
	unsigned int VBO;
	unsigned int VAO;
	unsigned int EBO;

	//reference variable incoming texture
	unsigned int renderTexture = -1;

	//visual shader
	std::unique_ptr<Shader> textureVisualiser;

private:
	//initialisation functions
	bool Initialise();
	void InitialiseVisualisationShader();
	void LoadInitialVertexData();

	//Render call, to be called each frame
	void RenderVisualFrame(unsigned int renderedTexture);

public:
	//Public render call, to be called each frame
	bool RenderFrame();


	//Getters and Setters
	GLFWwindow* GetWindow();
	void GetWindowSize(glm::ivec2& windowSize);

	//Update the texture being visualised by the renderer. For 2D fluids
	void SetVisualTexture(unsigned int& texture);
	//Reset texture for visualisation
	void ClearVisualTexture();

};

