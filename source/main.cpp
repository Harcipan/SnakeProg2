/*******************************************************************
** Ez a program a prog2 targy nhf-ra keszult.
**
** Az OpenGL megismeresehez a learnopengl.com oldalt
** hasznaltam igy elofordulhatnak hasonlosagok, fokent a: 
** shaderekben, hasznalt librarykban es bizonyos osztalyokban
** (camera, shader, model) melyek a grakikai megjeleniteshez szuksegesek.
**
** Keszitette: Kemecsei Kornel (HDB6X9)
******************************************************************/
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb/stb_image.h"

#include "headers/game.h"
#include "headers/resource_manager.h"

#include <iostream>

const unsigned int SCREEN_WIDTH = 900;
const unsigned int SCREEN_HEIGHT = 900;

Game SnakeGame(SCREEN_WIDTH, SCREEN_HEIGHT);

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    // when a user presses the escape key, we set the WindowShouldClose property to true, closing the application
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
            SnakeGame.Keys[key] = true;
        else if (action == GLFW_RELEASE)
            SnakeGame.Keys[key] = false;
    }
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

GLFWwindow* windowInit()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    glfwWindowHint(GLFW_RESIZABLE, true);

    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Snake - Kemecsei Kornel (HDB6X9)", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return NULL;
    }

    //window icon setup
    int width, height, channels;
    unsigned char* image = stbi_load("source/resource/textures/awesome.png", &width, &height, &channels, 4);

    if (image != NULL) {
        GLFWimage icon{};
        icon.width = width;
        icon.height = height;
        icon.pixels = image;
        glfwSetWindowIcon(window, 1, &icon);
    }

    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    return window;
}

int main(int argc, char* argv[])
{

    GLFWwindow* window = windowInit();


    SnakeGame.Init();


    //dt fps fuggetlen mukodeshez
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    //----GAME LOOP----//
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        glfwPollEvents();


        SnakeGame.ProcessInput(deltaTime);
        SnakeGame.Update(deltaTime, window);


        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        SnakeGame.Render();

        glfwSwapBuffers(window);
    }

    //----MEMORY CLEAN---//
    ResourceManager::Clear();

    glfwTerminate();
    return 0;
}