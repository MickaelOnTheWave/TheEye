#ifndef GLRENDERCUBE_H
#define GLRENDERCUBE_H

#include "GlRenderObject.h"

class GlRenderCube : public GlRenderObject
{
public:
   GlRenderCube();

   void Render() override;

private:
   void setupVertexBufferObject();
   void setupElementBufferObject();
   void setupTextureObject();
   void setupVertexArrayAttributes();

   unsigned int vertexArrayObject;
   unsigned int vertexBufferObject;
   unsigned int elementBufferObject;
   unsigned int textureObject;
};

#endif // GLRENDERCUBE_H
