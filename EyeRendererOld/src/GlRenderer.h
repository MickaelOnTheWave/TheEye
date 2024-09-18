#ifndef GLRENDERER_H
#define GLRENDERER_H

#include <string>

#include "GlCamera.h"
#include "ShaderProgram.h"

class GlRenderer
{
public:
   GlRenderer(GlCamera& _camera);
   virtual ~GlRenderer();

   void render();

private:
   void initializeRendering();

   GlCamera& camera;
   ShaderProgram shaderProgram;

   unsigned int vbo;
   unsigned int vao;
};

#endif // GLRENDERER_H
