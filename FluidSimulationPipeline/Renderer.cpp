#include "Renderer.h"
#include "Shader.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>


Renderer::Renderer() {

    if (!Initialise()) {
        return;
    }
    InitialiseVisualisationShader();
    
    LoadInitialVertexData(); 
}

Renderer::~Renderer() {
    if (!shaderClosed) {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glfwTerminate();
    }
}

bool Renderer::Initialise() {
    // glfw initialise and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    //create window
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Fluid Renderer", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }
    //focus window
    glfwMakeContextCurrent(window);

    glfwSwapInterval(0);

    renderCamera = RenderCamera(glm::vec3(0.0f, 0.0f, 3.0f));


    //load opengl functions
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return false;
    }

    glEnable(GL_DEPTH_TEST);


    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &EBO);

    return true;
}



void Renderer::InitialiseVisualisationShader() {
    textureVisualiser = std::unique_ptr<Shader>(new Shader("TextureVisualiser.vs", "TextureVisualiser.fgs"));
}

void Renderer::LoadInitialVertexData() {
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2D), vertices2D, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices2D), indices2D, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}


void Renderer::RenderVisualFrame(unsigned int renderedTexture) {
    if (renderTexture != -1) {
        textureVisualiser->Use();

        glBindTexture(GL_TEXTURE_2D, renderedTexture);

        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(renderCamera.GetFOV()), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        textureVisualiser->SetMat4("projection", projection);

        // camera/view transformation
        textureVisualiser->SetMat4("view", renderCamera.GetViewMatrix());
        textureVisualiser->SetInt("freeCam", 0);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
    }
}

bool Renderer::RenderFrame() {
    if (!glfwWindowShouldClose(window)) {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        std::cout << "FPS: " << 1 / deltaTime << std::endl;

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glfwGetWindowSize(window, &SCR_WIDTH, &SCR_HEIGHT);
        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

        RenderVisualFrame(renderTexture);

        glfwPollEvents();

        return true;
    }
    else {
        //Close Program
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }


    glfwTerminate();

    shaderClosed = true;

    return false;
}

GLFWwindow* Renderer::GetWindow() {
    return window;
}

void Renderer::GetWindowSize(glm::ivec2& windowSize) {
    windowSize = glm::ivec2(SCR_WIDTH, SCR_HEIGHT);
}


void Renderer::SetVisualTexture(unsigned int& texture) {
    renderTexture = texture;
}

void Renderer::ClearVisualTexture() {
    renderTexture = -1;
}

