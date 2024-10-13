#ifndef GLRENDEROBJECT_H
#define GLRENDEROBJECT_H

#include <string>
#include <vector>

class GlRenderObject
{
public:
   using TextureVec = std::vector<std::pair<std::string, int>>;

   GlRenderObject() = default;
   virtual ~GlRenderObject() = default;

   virtual void PrepareRendering(const unsigned int shaderProgramId);

   virtual void Render() = 0;

protected:
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

#endif // GLRENDEROBJECT_H
