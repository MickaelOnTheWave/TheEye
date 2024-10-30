#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "GlRenderer.h"
#include "cameras/OrbitCamera.h"
#include "objects/GlRenderCube.h"
#include "objects/GlRenderSphereTetrahedron.h"
#include "objects/GlRenderSpherePolar.h"

OrbitCamera camera(0, 0, 0);
double windowXCenter;
double windowYCenter;

double previousXpos;
double previousYpos;

unsigned int subdivLevel = 0;
GlRenderer* renderer = nullptr;

void PopulateSphereScene()
{
   //const GlRenderObject::Texture wallTexture = {"data/eye-blue.jpg", GL_RGB};
   const GlRenderObject::Texture wallTexture = {"data/testTexture.png", GL_RGB};
   auto sphere1 = new GlRenderSpherePolar();
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
   auto sphere1 = new GlRenderSphereTetrahedron();
   sphere1->Initialize(0, textures1);
   auto sphere2 = new GlRenderSphereTetrahedron();
   sphere2->Initialize(4, textures2);

   const int dim = 6;
   const float dimScaling = 0.5;

   for (int i=0; i<dim; ++i)
   {
      for (int j=0; j<dim; ++j)
      {
         for (int k=0; k<dim; ++k)
         {
            GlRenderObject* renderObj;
            if (i < 4)
            {
               if (j < 4)
               {
                  if (k < 4)
                     renderObj = cube1;
                  else
                     renderObj = cube3;
               }
               else
                  renderObj = cube2;
            }
            else
               renderObj = sphere2;

            auto transform1 = Matrix4x4::Scale(0.3);
            transform1 *= Matrix4x4::Translation(i * dimScaling,
                                                 j * dimScaling,
                                                 k * dimScaling);
            auto instance = new GlRenderedInstance(renderObj, transform1);
            renderer.AddRenderObject(instance);
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

   glfwSetKeyCallback(window, keyPressCallback);
   glfwSetCursorPosCallback(window, mouseMoveCallback);
   glfwMakeContextCurrent(window);

   gladLoadGL(glfwGetProcAddress);

   renderer = new GlRenderer(camera);
   renderer->SetClearColor(0.0f, 0.0f, 0.0f);

   populateScene(*renderer);

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

