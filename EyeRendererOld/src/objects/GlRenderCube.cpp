#include "GlRenderCube.h"

#include <glad/gl.h>

GlRenderCube::GlRenderCube()
{
   glGenBuffers(1, &vertexBufferObject);
   glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);

   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
   glEnableVertexAttribArray(0);

   const GLfloat value = 0.5f;
   const GLfloat vertices[] = {
       -value, -value,  value,
        value, -value,  value,
        value,  value,  value,
       -value,  value,  value,

       -value, -value, -value,
        value, -value, -value,
        value,  value, -value,
       -value,  value, -value
   };

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

   glGenVertexArrays(1, &vertexArrayObject);
   glBindVertexArray(vertexArrayObject);
   glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
   glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
   glEnableVertexAttribArray(0);
}

void GlRenderCube::Render()
{
   const unsigned int vertexPerFace = 6;
   const unsigned int facesInCube = 6;
   const unsigned int indexCount = vertexPerFace * facesInCube;

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject);
   glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
}
