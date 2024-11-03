#include "GlRenderSpherePolar.h"

#include <cmath>

void GlRenderSpherePolar::PopulateGeometry(const unsigned int subdivisions)
{
   const unsigned int horizontalPointCount = pow(2, subdivisions + 2);
   const unsigned int verticalPointCount = pow(2, subdivisions + 1);

   PopulateCoordinates(horizontalPointCount, verticalPointCount);
   PopulateTriangles(horizontalPointCount, verticalPointCount);
}

void GlRenderSpherePolar::PopulateCoordinates(const unsigned int horizontalPointCount, const unsigned int verticalPointCount)
{
   const GLfloat radius = 1.f;
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

      for (int j=0; j<=horizontalPointCount; ++j)
      {
         const GLfloat horizontalAngle = j * horizontalAnglePerSlice;
         const float x = plane * sin(horizontalAngle);
         const float z = plane * -cos(horizontalAngle);
         points.emplace_back(x, y, z);

         colors.emplace_back(1.f, 1.f, 1.f);

         const float u = static_cast<float>(j) / horizontalPointCount;
         textureCoordinates.emplace_back(u, v);
      }
   }

   points.emplace_back(0.f, -radius, 0.f);
   colors.emplace_back(1.f, 1.f, 1.f);
   textureCoordinates.emplace_back(0.5f, 1.f);
}

void GlRenderSpherePolar::PopulateTriangles(const unsigned int horizontalPointCount, const unsigned int verticalPointCount)
{
   for (int j=0; j<horizontalPointCount; ++j)
      triangles.emplace_back(0, j+2, j+1);

   for (int i=0; i<verticalPointCount-2; ++i)
   {
      const unsigned int verticalIndex = i * (horizontalPointCount + 1) + 1;
      for (int j=0; j<horizontalPointCount; ++j)
      {
         const unsigned int indexP1 = j + verticalIndex;
         const unsigned int indexP2 = indexP1 + 1;
         const unsigned int indexP3 = indexP1 + horizontalPointCount;
         const unsigned int indexP4 = indexP3 + 1;
         triangles.emplace_back(indexP1, indexP2, indexP4);
         triangles.emplace_back(indexP1, indexP4, indexP3);
         //triangles.emplace_back(indexP1, indexP4, );
      }
   }

   const unsigned int finalIndex = points.size()-1;
   const unsigned int startIndex = finalIndex - horizontalPointCount-1;
   for (int i = startIndex; i < finalIndex; ++i)
}
