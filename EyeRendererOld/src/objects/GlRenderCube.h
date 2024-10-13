#ifndef GLRENDERCUBE_H
#define GLRENDERCUBE_H

#include "GlRenderObject.h"

class GlRenderCube : public GlRenderObject
{
public:
   GlRenderCube(const TextureVec& _textureFiles);

   void PrepareRendering(const unsigned int shaderProgramId) override;

   void Render() override;

private:
   void setupVertexBufferObject();
   void setupElementBufferObject();
   void setupTextureObject(const TextureVec& _textureFiles);
   void setupVertexArrayAttributes();
};

#endif // GLRENDERCUBE_H
