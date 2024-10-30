#include "GlRenderSpherePolar.h"

#include <cmath>

void GlRenderSpherePolar::PopulateGeometry(const unsigned int subdivisions)
{
   const GLfloat radius = 1.f;

   int horizontalPointCount = pow(2, subdivisions + 2);
   int verticalPointCount = 1 + pow(2, subdivisions + 1);

   const GLfloat horizontalAnglePerSlice = 2 * M_PI / static_cast<GLfloat>(horizontalPointCount);
   const GLfloat verticalAnglePerSlice = M_PI / static_cast<GLfloat>(verticalPointCount);


   for (int i=0; i<verticalPointCount; ++i)
   {
      const GLfloat verticalAngle = i * verticalAnglePerSlice;

      const float y = radius * cos(verticalAngle);
      const float plane = radius * sin(verticalAngle);

      for (int j=0; j<horizontalPointCount; ++j)
      {
         const GLfloat horizontalAngle = j * horizontalAnglePerSlice;
         const float x = plane * sin(horizontalAngle);
         const float z = plane * -cos(horizontalAngle);
         points.emplace_back(x, y, z);

         colors.emplace_back(1.f, 1.f, 1.f);

         //const GLfloat texU = 0.5f + (arctan2(dz, dx) / 2 * M_PI);
         //const GLfloat texV = 0.5f + (arcsin(dy) / 2 * M_PI);

         const float u = static_cast<float>(j) / horizontalPointCount;
         const float v = static_cast<float>(i) / verticalPointCount;
         textureCoordinates.emplace_back(u, v);
      }
   }

   for (int i=1; i<verticalPointCount-1; ++i)
   {
      for (int j=1; j<horizontalPointCount-1; ++j)
      {
         const unsigned int currentIndex = j + i * horizontalPointCount;
         triangles.emplace_back(currentIndex, currentIndex + 1, currentIndex + 1 + horizontalPointCount);
         triangles.emplace_back(currentIndex, currentIndex + 1 + horizontalPointCount, currentIndex + horizontalPointCount);
      }
   }
}
