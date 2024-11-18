#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "GlRenderer.h"
#include "cameras/OrbitCamera.h"

#include "Eye.h"
#include "FaceLiveDetector.h"

// TODO : Add plane rendering (eyelid)
// TODO : Add animation to eyelid/Renderer
// TODO : Add matrix skinning to eyelid
// TODO : Add screenshot from screen
// TODO : Add texture mapping to conform to geometry

OrbitCamera camera(0, 0, 0);

const int windowSizeX = 1920;
const int windowSizeY = 1080;
const double windowXCenter = windowSizeX / 2.0;
const double windowYCenter = windowSizeY / 2.0;

GlRenderer* renderer = nullptr;
Eye eye;

struct EyeCalibrationData
{
   float offsetX = 0.f;
   float offsetY = 0.f;
   float factorX = 1.f;
   float factorY = 1.f;
};

EyeCalibrationData eyeCalibration;

Vector3 MapToGlSpace(double xPos, double yPos)
{
   const float xFactor = 10.f;
   const float x = xFactor * (xPos - windowXCenter) / windowXCenter;

   const float yFactor = 10.f;
   const float y = yFactor * (yPos - windowYCenter) / windowYCenter;

   const float z = 5.f;

   return Vector3(x, y, z);
}

void SetCalibration()
{
   eyeCalibration.offsetX = 0.f;
   eyeCalibration.offsetY = 0.f;
   eyeCalibration.factorX = 1.f;
   eyeCalibration.factorY = 1.f;
}

Vector3 ConvertTo3dPosition(const FaceRect& faceData, const int videoWidth, const int videoHeight)
{
   const float faceCenterX = faceData.x + (faceData.width / 2.f);
   const float faceCenterY = faceData.y + (faceData.height / 2.f);

   const float videoXCenter = videoWidth / 2.f;
   const float x = -eyeCalibration.factorX * (faceCenterX + eyeCalibration.factorX - videoXCenter) / videoXCenter;

   const float videoYCenter = videoHeight / 2.f;
   const float y = eyeCalibration.factorY * (faceCenterY + eyeCalibration.factorY- videoYCenter) / videoYCenter;

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

   FaceLiveDetector faceDetector;
   bool ok = faceDetector.Initialize("data/haarcascade_frontalface_default.xml");
   if (!ok)
      return 1;

   renderer = new GlRenderer(camera);
   renderer->SetClearColor(0.0f, 0.0f, 0.0f);

   eye.Initialize(renderer);

   renderer->PrepareRendering();

   while (!glfwWindowShouldClose(window))
   {
      glfwPollEvents();

      const FaceDataVec faceData = faceDetector.Detect();

      if (!faceData.empty())
      {
         const Vector3 facePosition = ConvertTo3dPosition(faceData.front(),
                                                          faceDetector.GetVideoWidth(),
                                                          faceDetector.GetVideoHeight());
         eye.LookAt(facePosition);
      }
      else
         eye.LookAt(Vector3(0.f, -1.f, 0.f));

      renderer->Render();

      glfwSwapBuffers(window);
   }

   // Clean up
   delete renderer;
   glfwTerminate();
   return 0;
}

