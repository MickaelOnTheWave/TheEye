#include "GlRenderObject.h"

#include <glad/gl.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


void GlRenderObject::PrepareRendering(const unsigned int shaderProgramId)
{
   glUniform1i(glGetUniformLocation(shaderProgramId, "textureCount"), textureFiles.size());

   for (unsigned int i=0; i<textureFiles.size(); ++i)
   {
      glActiveTexture(GL_TEXTURE0 + i);
      glBindTexture(GL_TEXTURE_2D, textureObjects[i]);
   }

   glBindVertexArray(vertexArrayObject);
}

void GlRenderObject::setupTextureObject(const TextureVec& _textureFiles)
{
   textureFiles = _textureFiles;

   glGenTextures(textureFiles.size(), textureObjects);
   for (int i=0; i<textureFiles.size(); ++i)
   {
      const auto& textureFile = textureFiles[i];
      generateTextureObject(textureFile.first.c_str(), i, textureFile.second);
   }
}

void GlRenderObject::setupVertexArrayAttributes()
{
   const GLsizei stride = 8 * sizeof(float);

   // Position
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
   glEnableVertexAttribArray(0);

   // Color
   glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
   glEnableVertexAttribArray(1);

   // Texture coordinates
   glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));
   glEnableVertexAttribArray(2);
}

void GlRenderObject::generateTextureObject(const char *imagePath, const unsigned int textureIndex,
                                           const unsigned int dataFormat)
{
   int width, height, nrChannels;
   unsigned char *textureData = stbi_load(imagePath, &width, &height, &nrChannels, 0);
   if (textureData)
   {
      glActiveTexture(GL_TEXTURE0 + textureIndex);
      glBindTexture(GL_TEXTURE_2D, textureObjects[textureIndex]);

      // Textures
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, textureData);
      glGenerateMipmap(GL_TEXTURE_2D);

      stbi_image_free(textureData);
   }
}
