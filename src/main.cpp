#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <chrono>

#include "GlRenderer.h"
#include "cameras/OrbitCamera.h"

#include "Eye.h"
#include "EyeFaceAnalyzer.h"
#include "LinuxScreenCapturer.h"

// TODO : Add procedural eye texture
// TODO : Optimize face detection (reuse previous data)
// TODO : Try to use CUDA / GPU for face detection
// TODO : Implement funnier logic for eye
// TODO : implement pupil dilation with distance

const bool debugging = true;

int GetWindowX()
{
   return (debugging) ? 800 : 1920;
}

int GetWindowY()
{
   return (debugging) ? 600 : 1080;
}

static void keyPressCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
   if (action == GLFW_PRESS)
   {
      if (key == GLFW_KEY_ESCAPE)
         glfwSetWindowShouldClose(window, GLFW_TRUE);
   }
}

int main()
{
   if (!glfwInit()) return -1;

   LinuxScreenCapturer screenshotCapturer;
   const ImageData screenshot = screenshotCapturer.Capture();

   GLFWmonitor* monitor = (debugging) ? nullptr : glfwGetPrimaryMonitor();
   GLFWwindow* window = glfwCreateWindow(GetWindowX(), GetWindowY(), "Eye Renderer", monitor, nullptr);
   if (!window)
   {
      glfwTerminate();
      return -1;
   }

   glfwSetKeyCallback(window, keyPressCallback);
   glfwMakeContextCurrent(window);

   gladLoadGL(glfwGetProcAddress);

   glViewport(0, 0, GetWindowX(), GetWindowY());

   OrbitCamera camera(Vector3(0, 0, 0));
   auto renderer = new GlRenderer(&camera);
   renderer->SetClearColor(0.0f, 0.0f, 0.0f);

   std::mutex faceMutex;
   std::optional<Vector3> facePosition;
   std::optional<Vector3> lastFacePosition;

   EyeFaceAnalyzer faceAnalyzer(faceMutex, facePosition);
   const int returnCode = faceAnalyzer.Initialize();
   if (returnCode == EyeFaceAnalyzer::OK)
   {
      faceAnalyzer.RunThreadedDetection();

      Eye eye;
      eye.Initialize(renderer, screenshot);

      float deltaT = 0.f;

      while (!glfwWindowShouldClose(window))
      {
         const auto startIteration = std::chrono::steady_clock::now();
         glfwPollEvents();

         std::optional<Vector3>* faceDataToUse = nullptr;
         bool tookTheLock = false;
         if (faceMutex.try_lock())
         {
            faceDataToUse = &facePosition;
            tookTheLock = true;
         }
         else
            faceDataToUse = &lastFacePosition;

         eye.Update(*faceDataToUse, deltaT);

         if (tookTheLock)
            lastFacePosition = facePosition;
         faceMutex.unlock();

         renderer->Render();

         glfwSwapBuffers(window);

         const auto endIteration = std::chrono::steady_clock::now();
         deltaT = std::chrono::duration<float>(endIteration - startIteration).count();
      }

      faceAnalyzer.StopThreadedDetection();
   }
   else
      std::cout << "Error initializing haarcascade. Check if cascade file exists." << std::endl;

   // Clean up
   delete renderer;
   glfwTerminate();
   return returnCode;
}

