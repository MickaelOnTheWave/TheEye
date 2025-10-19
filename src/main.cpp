#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <chrono>

#include "GlRenderer.h"
#include "cameras/OrbitCamera.h"

#include "Eye.h"
#include "EyeFaceAnalyzer.h"
#include "MonitorPosition.h"

#ifdef __linux
    #include "linux/LinuxScreenCapturer.h"
#elif defined WIN32
    #include "windows/WindowsScreenCapturer.h"
#endif

// TODO : Use a DEFINE for the data dir, and update Cmake accordingly.
// For Linux, it should be /usr/share/the-eye.
// For local builds, probably just ./data.
// TODO : Add procedural eye texture
// TODO : Optimize face detection (reuse previous data)
// TODO : Try to use CUDA / GPU for face detection
// TODO : implement pupil dilation with distance

#ifdef DEBUGGING
const bool debugging = true;
#else
const bool debugging = false;
#endif

static float globalT = 0.f;

static void keyPressCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
   if (action == GLFW_PRESS)
   {
      if (!debugging || key == GLFW_KEY_ESCAPE)
         glfwSetWindowShouldClose(window, GLFW_TRUE);
   }
}

static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
   if (!debugging)
      glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void mouseMoveCallback(GLFWwindow* window, double xpos, double ypos)
{
   if (globalT > 1.f) // Ignore the first second
   {
      if (!debugging)
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

void errorCallback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error (%d): %s\n", error, description);
}

AbstractScreenCapturer* CreateScreenCapturer()
{
#ifdef __linux
    return new LinuxScreenCapturer();
#elif defined WIN32
    return new WindowsScreenCapturer();
#endif
}

std::optional<std::pair<GLFWmonitor*, GLFWwindow*>> InitializeGlDisplay()
{
    GLFWmonitor* monitor = nullptr;
    GLFWwindow* window = nullptr;

    glfwSetErrorCallback(errorCallback);
    if (!glfwInit())
        return std::nullopt;

    monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    //const int monitorWidth = mode->width;
    const int monitorWidth = 1980;
    //const int monitorHeight = mode->height;
    const int monitorHeight = 1080;

    window = glfwCreateWindow(monitorWidth, monitorHeight, "Eye Renderer", nullptr, nullptr);
    if (!window)
        return std::nullopt;

    glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_FALSE);
    glfwSetKeyCallback(window, keyPressCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetCursorPosCallback(window, mouseMoveCallback);
    glfwMakeContextCurrent(window);

    gladLoadGL(glfwGetProcAddress);
    glViewport(0, 0, monitorWidth, monitorHeight);

    return std::make_pair(monitor, window);
}

GlRenderer* CreateInitializedRenderer()
{
    OrbitCamera camera(Vector3(0, 0, 0));
    auto renderer = new GlRenderer(&camera);
    renderer->SetClearColor(0.0f, 0.0f, 0.0f);
    return renderer;
}

int main()
{
   auto screenshotCapturer = CreateScreenCapturer();
   const std::unordered_map<MonitorPosition, ImageData> screenshots = screenshotCapturer->Capture();  

   std::mutex faceMutex;
   std::optional<Vector3> facePosition;
   std::optional<Vector3> lastFacePosition;

   EyeFaceAnalyzer faceAnalyzer(faceMutex, facePosition);
   const bool ok = faceAnalyzer.Initialize();
   if (!ok)
   {
       std::cout << "Error initializing Face analyzer. Check if you have a working camera and if the data folder is accessible." << std::endl;
       return 1;
   }
   faceAnalyzer.RunThreadedDetection();

    auto glDisplay = InitializeGlDisplay();
    if (!glDisplay.has_value())
    {
        std::cout << "Error creating OpenGl display." << std::endl;
        glfwTerminate();
        return 2;
    }

    GlRenderer* renderer = CreateInitializedRenderer();
/*
    Eye eye;
    const ImageData rightScreen = FindRightScreenshot(screenshots, glDisplay->first);
    eye.Initialize(renderer, rightScreen);
    */
    float deltaT = 0.f;

    while (!glfwWindowShouldClose(glDisplay->second))
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

        //eye.Update(*faceDataToUse, deltaT);

        if (tookTheLock)
        {
            lastFacePosition = facePosition;
            faceMutex.unlock();
        }

        //renderer->Render();

        glfwSwapBuffers(glDisplay->second);

        const auto endIteration = std::chrono::steady_clock::now();
        deltaT = std::chrono::duration<float>(endIteration - startIteration).count();
        globalT += deltaT;
    }

    faceAnalyzer.StopThreadedDetection();

   // Clean up
   glfwTerminate();
   delete renderer;
   //delete screenshotCapturer;   
   return ok ? 0 : 1;
}

