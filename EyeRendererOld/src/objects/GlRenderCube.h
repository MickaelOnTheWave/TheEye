#ifndef GLRENDERCUBE_H
#define GLRENDERCUBE_H

#include "GlRenderObject.h"

#include <string>
#include <vector>

class GlRenderCube : public GlRenderObject
{
public:
   using TextureVec = std::vector<std::pair<std::string, int>>;
   GlRenderCube(const TextureVec& _textureFiles);

   void PrepareRendering() override;

   void Render() override;

private:
   void setupVertexBufferObject();
   void setupElementBufferObject();
   void setupTextureObject(const TextureVec& _textureFiles);
   void setupVertexArrayAttributes();

   void generateTextureObject(const char* imagePath, const unsigned int textureIndex,
                              const unsigned int dataFormat);

   unsigned int vertexArrayObject;
   unsigned int vertexBufferObject;
   unsigned int elementBufferObject;
   unsigned int textureObjects[2];
   TextureVec textureFiles;
};

#endif // GLRENDERCUBE_H
