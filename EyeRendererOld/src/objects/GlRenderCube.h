#ifndef GLRENDERCUBE_H
#define GLRENDERCUBE_H

#include "GlRenderObject.h"

class GlRenderCube : public GlRenderObject
{
public:
   GlRenderCube();

   void PrepareRendering() override;

   void Render() override;

private:
   void setupVertexBufferObject();
   void setupElementBufferObject();
   void setupTextureObject();
   void setupVertexArrayAttributes();

   void generateTextureObject(const char* imagePath, const unsigned int textureIndex,
                              const unsigned int dataFormat);

   unsigned int vertexArrayObject;
   unsigned int vertexBufferObject;
   unsigned int elementBufferObject;
   unsigned int textureObjects[2];
};

#endif // GLRENDERCUBE_H
