#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "GlRenderer.h"
#include "cameras/OrbitCamera.h"

#include "Eye.h"
#include "FaceLiveDetector.h"

OrbitCamera camera(0, 0, 0);

const int windowSizeX = 1024;
const int windowSizeY = 768;
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

static void mouseMoveCallback(GLFWwindow* window, double xpos, double ypos)
{
   eye.LookAt(MapToGlSpace(xpos, ypos));
   //std::cout << deltaX << std::endl;
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

   glViewport(0, 0, windowSizeX, windowSizeY);

   FaceLiveDetector faceDetector;
   bool ok = faceDetector.Initialize("data/haarcascade_frontalface_default.xml");
   //if (!ok)
   //   return 1;

   renderer = new GlRenderer(camera);
   renderer->SetClearColor(0.0f, 0.0f, 0.0f);

   eye.Initialize(renderer);

   renderer->PrepareRendering();

   while (!glfwWindowShouldClose(window))
   {
      glfwPollEvents();

      //const FaceDataVec faceData = faceDetector.Detect();
      //eye.LookAt(facePosition);
      renderer->Render();

      glfwSwapBuffers(window);
   }

   // Clean up
   delete renderer;
   glfwTerminate();
   return 0;
}

