#include "GlRenderCube.h"

#include <glad/gl.h>

GlRenderCube::GlRenderCube(const TextureVec &_textureFiles)
{
   glGenVertexArrays(1, &vertexArrayObject);
   glBindVertexArray(vertexArrayObject);

   setupVertexBufferObject();
   setupElementBufferObject();
   setupTextureObject(_textureFiles);

   setupVertexArrayAttributes();
}

void GlRenderCube::Render()
{
   const unsigned int vertexPerFace = 6;
   const unsigned int facesInCube = 6;
   const unsigned int indexCount = vertexPerFace * facesInCube;
   glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
}

void GlRenderCube::setupVertexBufferObject()
{
   const GLfloat value = 0.5f;
   const GLfloat verticesColorsTexcoords[] = {
       -value, -value,  value,   1.f,0.f,0.f,   0.f,0.f,
       value, -value,  value,   0.f,1.f,0.f,   1.f,0.f,
       value,  value,  value,   0.f,0.f,1.f,   1.f,1.f,
       -value,  value,  value,   1.f,1.f,1.f,   1.f,0.f,

       -value, -value, -value,   0.f,0.f,0.f,   1.f,0.f,
       value, -value, -value,   1.f,1.f,0.f,   0.f,0.f,
       value,  value, -value,   0.f,1.f,1.f,   0.f,1.f,
       -value,  value, -value,   1.f,0.f,1.f,   1.f,1.f
   };

   glGenBuffers(1, &vertexBufferObject);
   glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
   glBufferData(GL_ARRAY_BUFFER, sizeof(verticesColorsTexcoords), verticesColorsTexcoords, GL_STATIC_DRAW);
}

void GlRenderCube::setupElementBufferObject()
{
   const GLuint indices[] = {
       0,1,2, 0,2,3,
       1,5,6, 1,6,2,
       5,4,7, 5,7,6,
       4,0,3, 4,3,7,
       3,2,6, 3,6,7,
       4,5,1, 4,1,0
   };

   glGenBuffers(1, &elementBufferObject);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

