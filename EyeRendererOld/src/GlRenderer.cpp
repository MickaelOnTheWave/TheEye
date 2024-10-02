#include "GlRenderer.h"

GlRenderer::GlRenderer(GlCamera& _camera)
   : camera(_camera), shaderProgram("data/basic.vert", "data/basic.frag")
{
   glViewport(0, 0, 1024, 768);
}

GlRenderer::~GlRenderer()
{
   for (const auto renderObj : renderObjects)
   {
      delete renderObj;
   }
   renderObjects.clear();
}

void GlRenderer::SetClearColor(const float r, const float g, const float b)
{
   clearColorR = r;
   clearColorG = g;
   clearColorB = b;
}

void GlRenderer::AddRenderObject(GlRenderObject *renderObject)
{
   renderObjects.push_back(renderObject);
}

void GlRenderer::PrepareRendering()
{
   shaderProgram.use();
   glUniform1i(glGetUniformLocation(shaderProgram.GetId(), "texture1"), 0);
   glUniform1i(glGetUniformLocation(shaderProgram.GetId(), "texture2"), 1);
}

void GlRenderer::Render()
{
   glClearColor(clearColorR, clearColorG, clearColorB, 1.0f);
   glClear(GL_COLOR_BUFFER_BIT);

   glPushMatrix();
      for (const auto renderObj : renderObjects)
      {
         unsigned int transformLoc = glGetUniformLocation(shaderProgram.GetId(), "cameraTransform");
         glUniformMatrix4fv(transformLoc, 1, GL_FALSE, camera.getTransformMatrix().getData());

         glPushMatrix();
            renderObj->Render();
         glPopMatrix();
      }

   glPopMatrix();
}
