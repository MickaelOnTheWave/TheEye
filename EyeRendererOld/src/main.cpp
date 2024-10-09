#include <iostream>
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "GlRenderer.h"
#include "cameras/OrbitCamera.h"
#include "objects/GlRenderCube.h"

OrbitCamera camera(2, 0, 2);
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

   camera.RotateInX(deltaY);
   camera.RotateInY(deltaX);

   previousXpos = xpos;
   previousYpos = ypos;

   //std::cout << deltaX << std::endl;
}

void populateScene(GlRenderer& renderer)
{
   const std::pair<std::string, int> faceTexture = {"data/awesomeface.png", GL_RGBA};
   const std::pair<std::string, int> wallTexture = {"data/wall.jpg", GL_RGB};
   const GlRenderCube::TextureVec textures1 = {
      {"data/texture-wood.jpeg", GL_RGB}, faceTexture
   };
   const GlRenderCube::TextureVec textures2 = {wallTexture, faceTexture};
   auto cube1 = new GlRenderCube(textures1);
   auto cube2 = new GlRenderCube(textures2);
   auto cube3 = new GlRenderCube({wallTexture});

   const int dim = 8;
   const float dimScaling = 0.5;

   for (int i=0; i<dim; ++i)
   {
      for (int j=0; j<dim; ++j)
      {
         for (int k=0; k<dim; ++k)
         {
            GlRenderCube* cube = (k < 4) ? (i < 4) ? cube3 : cube2 : cube1;
            auto transform1 = Matrix4x4::Scale(0.3);
            transform1 *= Matrix4x4::Translation(i * dimScaling,
                                                 j * dimScaling,
                                                 k * dimScaling);
            auto cubeInstance = new GlRenderedInstance(cube, transform1);
            renderer.AddRenderObject(cubeInstance);
         }
      }
   }
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

