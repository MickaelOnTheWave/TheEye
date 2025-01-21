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

const int windowSizeX = 1920;
const int windowSizeY = 1080;
const double windowXCenter = windowSizeX / 2.0;
const double windowYCenter = windowSizeY / 2.0;

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

   OrbitCamera camera(Vector3(0, 0, 0));
   auto renderer = new GlRenderer(&camera);
   renderer->SetClearColor(0.0f, 0.0f, 0.0f);

   EyeFaceAnalyzer faceAnalyzer;
   const int returnCode = faceAnalyzer.Initialize();
   if (returnCode == EyeFaceAnalyzer::OK)
   {
      Eye eye;
      eye.Initialize(renderer);

      while (!glfwWindowShouldClose(window))
      {
         glfwPollEvents();

         std::optional<Vector3> facePosition = faceAnalyzer.Detect();
         eye.UpdateFaceData(facePosition);

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

