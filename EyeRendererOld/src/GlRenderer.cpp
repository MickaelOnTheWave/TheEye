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
}

void GlRenderer::Render()
{
   glClearColor(clearColorR, clearColorG, clearColorB, 1.0f);
   glClear(GL_COLOR_BUFFER_BIT);

   glPushMatrix();

      glMultMatrixf(camera.getTransformMatrix().getData());

      shaderProgram.use();

      for (const auto renderObj : renderObjects)
      {
         glPushMatrix();
            renderObj->Render();
         glPopMatrix();
      }

   glPopMatrix();
}
