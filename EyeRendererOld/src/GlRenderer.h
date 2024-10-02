#ifndef GLRENDERER_H
#define GLRENDERER_H

#include <vector>
#include "GlCamera.h"
#include "ShaderProgram.h"
#include "objects/GlRenderObject.h"

class GlRenderer
{
public:
   GlRenderer(GlCamera& _camera);
   virtual ~GlRenderer();

   void SetClearColor(const float r, const float g, const float b);

   void AddRenderObject(GlRenderObject* renderObject);

   void PrepareRendering();

   void Render();

private:
   GlCamera& camera;
   std::vector<GlRenderObject*> renderObjects;
   ShaderProgram shaderProgram;

   float clearColorR, clearColorG, clearColorB;
};

#endif // GLRENDERER_H
