#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "GlRenderer.h"
#include "cameras/OrbitCamera.h"

#include "Eye.h"
#include "EyeFaceAnalyzer.h"

// TODO : Add plane rendering (eyelid)
// TODO : Add animation to eyelid/Renderer
// TODO : Add matrix skinning to eyelid
// TODO : Add screenshot from screen
// TODO : Add texture mapping to conform to geometry

OrbitCamera camera(Vector3(0, 0, 0));

const int windowSizeX = 1920;
const int windowSizeY = 1080;
const double windowXCenter = windowSizeX / 2.0;
const double windowYCenter = windowSizeY / 2.0;

GlRenderer* renderer = nullptr;
Eye eye;

Vector3 MapToGlSpace(double xPos, double yPos)
{
   const float xFactor = 10.f;
   const float x = xFactor * (xPos - windowXCenter) / windowXCenter;

   const float yFactor = 10.f;
   const float y = yFactor * (yPos - windowYCenter) / windowYCenter;

   const float z = 5.f;

   return Vector3(x, y, z);
}

static void keyPressCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
   if (action == GLFW_PRESS)
   {
      if (key == GLFW_KEY_ESCAPE)
         glfwSetWindowShouldClose(window, GLFW_TRUE);
   }
}

Vector3 DetectFacePosition()
{
   Vector3 facePosition;
   return facePosition;
}

int main()
{
   GLFWwindow* window;

   if (!glfwInit()) return -1;

   window = glfwCreateWindow(windowSizeX, windowSizeY, "Eye Renderer", glfwGetPrimaryMonitor(), nullptr);
   if (!window)
   {
      glfwTerminate();
      return -1;
   }

   glfwSetKeyCallback(window, keyPressCallback);
   glfwMakeContextCurrent(window);

   gladLoadGL(glfwGetProcAddress);

   glViewport(0, 0, windowSizeX, windowSizeY);

   EyeFaceAnalyzer faceAnalyzer;
   const int returnCode = faceAnalyzer.Initialize();
   if (returnCode == EyeFaceAnalyzer::OK)
   {

      renderer = new GlRenderer(&camera);
      renderer->SetClearColor(0.0f, 0.0f, 0.0f);

      eye.Initialize(renderer);

      renderer->PrepareRendering();

      while (!glfwWindowShouldClose(window))
      {
         glfwPollEvents();

         std::optional<Vector3> facePosition = faceAnalyzer.Detect();

         if (facePosition)
            eye.LookAt(facePosition.value());
         else
            eye.LookAt(Vector3(0.f, -1.f, 0.f));

         renderer->Render();

         glfwSwapBuffers(window);
      }

   }
   else
      std::cout << "Error initializing haarcascade. Check if cascade file exists." << std::endl;

   // Clean up
   delete renderer;
   glfwTerminate();
   return returnCode;
}

