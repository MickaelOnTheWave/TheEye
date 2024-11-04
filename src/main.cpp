#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "GlRenderer.h"
#include "cameras/OrbitCamera.h"
#include "objects/GlRenderSpherePolar.h"

OrbitCamera camera(0, 0, 0);
double windowXCenter;
double windowYCenter;

double previousXpos;
double previousYpos;

unsigned int subdivLevel = 1;
GlRenderer* renderer = nullptr;

void PopulateSphereScene()
{
   const GlRenderObject::Texture wallTexture = {"data/eye-blue.jpg", GL_RGB};
   auto sphere1 = new GlRenderSpherePolar();
   sphere1->SetTextureProjection(GlRenderSpherePolar::TextureMap::HalfWrap);
   sphere1->Initialize(subdivLevel, {wallTexture});

   auto transform1 = Matrix4x4::Scale(0.5);
   auto instance = new GlRenderedInstance(sphere1, transform1);
   renderer->AddRenderObject(instance);
}

static void keyPressCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
   if (action == GLFW_PRESS)
   {
      if (key == GLFW_KEY_ESCAPE)
         glfwSetWindowShouldClose(window, GLFW_TRUE);
      else if (key == GLFW_KEY_KP_ADD)
      {
         ++subdivLevel;
         renderer->ClearScene();
         PopulateSphereScene();
      }
      else if (key == GLFW_KEY_KP_SUBTRACT)
      {
         if (subdivLevel > 0)
            --subdivLevel;
         renderer->ClearScene();
         PopulateSphereScene();
      }
   }
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

   glfwSetKeyCallback(window, keyPressCallback);
   glfwSetCursorPosCallback(window, mouseMoveCallback);
   glfwMakeContextCurrent(window);

   gladLoadGL(glfwGetProcAddress);

   renderer = new GlRenderer(camera);
   renderer->SetClearColor(0.0f, 0.0f, 0.0f);

   PopulateSphereScene();

   renderer->PrepareRendering();

   while (!glfwWindowShouldClose(window))
   {
      glfwPollEvents();
      renderer->Render();
      glfwSwapBuffers(window);
   }

   // Clean up
   delete renderer;
   glfwTerminate();
   return 0;
}

