#include <iostream>
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "GlRenderer.h"
#include "cameras/GlCamera.h"
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

   //std::cout << deltaX << std::endl;
}

void populateScene(GlRenderer& renderer)
{
   auto startCube = new GlRenderCube();

   auto transform1 = Matrix4x4::Scale(0.4);
   transform1 *= Matrix4x4::Translation(0.5, 0.0, 0.0);
   auto cube1 = new GlRenderedInstance(startCube, transform1);
   renderer.AddRenderObject(cube1);

   auto transform2 = Matrix4x4::Scale(0.4);
   transform2 *= Matrix4x4::Translation(-0.5, 0.0, 0.0);
   auto cube2 = new GlRenderedInstance(startCube, transform2);
   renderer.AddRenderObject(cube2);

   auto transform3 = Matrix4x4::Scale(0.4);
   transform3 *= Matrix4x4::Translation(0.0, 0.5, 0.0);
   auto cube3 = new GlRenderedInstance(startCube, transform3);
   renderer.AddRenderObject(cube3);
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

        populateScene(renderer);

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

