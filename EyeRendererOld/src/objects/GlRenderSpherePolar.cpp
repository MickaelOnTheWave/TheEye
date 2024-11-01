#include "GlRenderSpherePolar.h"

#include <cmath>

void GlRenderSpherePolar::PopulateGeometry(const unsigned int subdivisions)
{
   const GLfloat radius = 1.f;

   int horizontalPointCount = pow(2, subdivisions + 2);
   int verticalPointCount = pow(2, subdivisions + 1);

   const GLfloat horizontalAnglePerSlice = 2 * M_PI / static_cast<GLfloat>(horizontalPointCount);
   const GLfloat verticalAnglePerSlice = M_PI / static_cast<GLfloat>(verticalPointCount);

   points.emplace_back(0.f, radius, 0.f);
   colors.emplace_back(1.f, 1.f, 1.f);
   textureCoordinates.emplace_back(0.5f, 0.f);

   for (int i=1; i<verticalPointCount; ++i)
   {
      const GLfloat verticalAngle = i * verticalAnglePerSlice;

      const float y = radius * cos(verticalAngle);
      const float plane = radius * sin(verticalAngle);

      const float v = static_cast<float>(i) / verticalPointCount;

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
         textureCoordinates.emplace_back(u, v);
      }
   }

   points.emplace_back(0.f, -radius, 0.f);
   colors.emplace_back(1.f, 1.f, 1.f);
   textureCoordinates.emplace_back(0.5f, 1.f);

   ///////////////////////

   for (int j=0; j<horizontalPointCount-1; ++j)
      triangles.emplace_back(0, j+2, j+1);
   triangles.emplace_back(0, 1, horizontalPointCount);

   for (int i=0; i<verticalPointCount-2; ++i)
   {
      const unsigned int verticalIndex = i * horizontalPointCount + 1;
      for (int j=0; j<horizontalPointCount-1; ++j)
      {
         const unsigned int currentIndex = j + verticalIndex;
         triangles.emplace_back(currentIndex, currentIndex + 1, currentIndex + 1 + horizontalPointCount);
         triangles.emplace_back(currentIndex, currentIndex + 1 + horizontalPointCount, currentIndex + horizontalPointCount);
      }

      const unsigned int finalJ = horizontalPointCount - 1;
      const unsigned int currentIndex = finalJ + verticalIndex;
      triangles.emplace_back(currentIndex, verticalIndex, verticalIndex + horizontalPointCount);
      triangles.emplace_back(currentIndex, verticalIndex + horizontalPointCount, currentIndex + horizontalPointCount);
   }

   const unsigned int finalIndex = points.size();
   const unsigned int startIndex = finalIndex - horizontalPointCount - 1;
   for (int i = startIndex; i < finalIndex; ++i)
      triangles.emplace_back(i, finalIndex-1, i+1);
   triangles.emplace_back(finalIndex-2, finalIndex-1, startIndex);

}
