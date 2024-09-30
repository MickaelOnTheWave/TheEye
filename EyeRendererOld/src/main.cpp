#include <iostream>
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "GlRenderer.h"
#include "objects/GlRenderCube.h"

GlCamera camera;
double windowXCenter;
double windowYCenter;

double previousXpos;
double previousYpos;

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

static void mouseMoveCallback(GLFWwindow* window, double xpos, double ypos)
{
   const double deltaX = xpos - previousXpos;
   const double deltaY = ypos - previousYpos;

   camera.rotateAroundX(deltaY);
   camera.rotateAroundY(deltaX);

   previousXpos = xpos;
   previousYpos = ypos;

   std::cout << deltaX << std::endl;
}

int main()
{
        GLFWwindow* window;

        if (!glfwInit()) return -1;

        const int windowSizeX = 1024;
        const int windowSizeY = 768;
        windowXCenter = windowSizeX / 2.0;
        windowYCenter = windowSizeY / 2.0;

        window = glfwCreateWindow(windowSizeX, windowSizeY, "Eye Renderer", nullptr, nullptr);
        if (!window)
        {
                glfwTerminate();
                return -1;
        }

        glfwSetKeyCallback(window, key_callback);
        glfwSetCursorPosCallback(window, mouseMoveCallback);
        glfwMakeContextCurrent(window);

        gladLoadGL(glfwGetProcAddress);

        GlRenderer renderer(camera);
        renderer.SetClearColor(0.0f, 0.0f, 0.0f);

        renderer.AddRenderObject(new GlRenderCube());

        renderer.PrepareRendering();

        while (!glfwWindowShouldClose(window))
        {
                glfwPollEvents();
                renderer.Render();
                glfwSwapBuffers(window);
        }

        // Clean up
        glfwTerminate();
        return 0;
}

