#ifndef GLRENDEROBJECT_H
#define GLRENDEROBJECT_H

#include <string>
#include <vector>

class GlRenderObject
{
public:
   using TextureVec = std::vector<std::pair<std::string, int>>;

   GlRenderObject();
   virtual ~GlRenderObject() = default;

   virtual void PrepareRendering(const unsigned int shaderProgramId) = 0;

   virtual void Render() = 0;

protected:
   void generateTextureObject(const char* imagePath, const unsigned int textureIndex,
                              const unsigned int dataFormat);

   unsigned int vertexArrayObject;
   unsigned int vertexBufferObject;
   unsigned int elementBufferObject;
   unsigned int textureObjects[2];
   TextureVec textureFiles;
};

#endif // GLRENDEROBJECT_H
