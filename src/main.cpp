#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <chrono>

#include "GlRenderer.h"
#include "cameras/OrbitCamera.h"

#include "Eye.h"
#include "EyeFaceAnalyzer.h"
#include "LinuxScreenCapturer.h"

// TODO : Use a DEFINE for the data dir, and update Cmake accordingly.
// For Linux, it should be /usr/share/the-eye.
// For local builds, probably just ./data.
// TODO : Add procedural eye texture
// TODO : Optimize face detection (reuse previous data)
// TODO : Try to use CUDA / GPU for face detection
// TODO : implement pupil dilation with distance

const bool debugging = false;

static void keyPressCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
   if (action == GLFW_PRESS)
   {
      if (key == GLFW_KEY_ESCAPE)
         glfwSetWindowShouldClose(window, GLFW_TRUE);
   }
}

ImageData FindRightScreenshot(const std::unordered_map<MonitorPosition, ImageData>& screenshots, GLFWmonitor* monitor)
{
   int currentMonitorX, currentMonitorY;
   glfwGetMonitorPos(monitor, &currentMonitorX, &currentMonitorY);

   auto it = screenshots.begin();
   for (; it != screenshots.end(); ++it)
   {
      if (it->first.xOrigin == currentMonitorX && it->first.yOrigin == currentMonitorY)
         return it->second;
   }
   return screenshots.begin()->second;
}

int main()
{
   if (!glfwInit()) return -1;

   LinuxScreenCapturer screenshotCapturer;
   const std::unordered_map<MonitorPosition, ImageData> screenshots = screenshotCapturer.Capture();

   GLFWmonitor* monitor = (debugging) ? nullptr : glfwGetPrimaryMonitor();
   const GLFWvidmode* mode = glfwGetVideoMode(monitor);
   const int monitorWidth = (debugging) ? 800 : mode->width;
   const int monitorHeight = (debugging) ? 600 : mode->height;
   GLFWwindow* window = glfwCreateWindow(monitorWidth, monitorHeight, "Eye Renderer", monitor, nullptr);
   if (!window)
   {
      glfwTerminate();
      return -1;
   }

   glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_FALSE);
   glfwSetKeyCallback(window, keyPressCallback);
   glfwMakeContextCurrent(window);

   gladLoadGL(glfwGetProcAddress);

   glViewport(0, 0, monitorWidth, monitorHeight);

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
      const ImageData rightScreen = FindRightScreenshot(screenshots, monitor);
      eye.Initialize(renderer, rightScreen);

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

