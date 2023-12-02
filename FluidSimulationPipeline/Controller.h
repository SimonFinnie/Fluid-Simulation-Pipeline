#pragma once
#include <memory>
#include <glm/glm.hpp>


class Renderer;

class Controller
{
public:
	Controller();

protected:
	//controlled renderer
	std::shared_ptr<Renderer> focusedRenderer;

	bool rendererUpdated = false;


public:
	//Function called every frame
	virtual void Tick(float deltaTime);

	//Sets the focused renderer
	virtual void SetFocusedRenderer(std::shared_ptr<Renderer> newFocusedRenderer);
	virtual void ResetFocusedRenderer();


protected:
	//processes input from the relevant window
	virtual void ProcessInput(float deltaTime);
	virtual void ProcessMouseInput(const glm::ivec2& currClickPos, glm::ivec2& prevClickedPos, glm::vec2& mouseDir);
};

