#include "GlRenderSphereOctahedron.h"

void GlRenderSphereOctahedron::PopulateInitialGeometry()
{
   const GLfloat radius = 1.f;

   //const GLfloat texU = 0.5f + (arctan2(dz, dx) / 2 * M_PI);
   //const GLfloat texV = 0.5f + (arcsin(dy) / 2 * M_PI);

   // Initial points
   points.emplace_back(0.f, radius, 0.f);
   points.emplace_back(radius, 0.f, 0.f);
   points.emplace_back(0.f, -radius, 0.f);
   points.emplace_back(-radius, 0.f, 0.f);
   points.emplace_back(0.f, 0.f, -radius);
   points.emplace_back(0.f, 0.f, radius);

   colors.emplace_back(0.f, 1.f, 0.f);
   colors.emplace_back(0.f, 0.f, 1.f);
   colors.emplace_back(1.f, 0.f, 0.f);
   colors.emplace_back(1.f, 0.f, 1.f);
   colors.emplace_back(1.f, 1.f, 0.f);
   colors.emplace_back(0.f, 1.f, 1.f);

   textureCoordinates.emplace_back(0.5f, 1.f);
   textureCoordinates.emplace_back(0.75f, 0.5f);
   textureCoordinates.emplace_back(0.5f, 0.f);
   textureCoordinates.emplace_back(0.25f, 0.5f);
   textureCoordinates.emplace_back(0.f, 0.5f);
   textureCoordinates.emplace_back(0.5f, 0.5f);

   // Initial triangles
   triangles.emplace_back(0, 1, 5);
   triangles.emplace_back(0, 5, 3);
   triangles.emplace_back(0, 3, 4);
   triangles.emplace_back(0, 4, 1);
   triangles.emplace_back(2, 5, 1);
   triangles.emplace_back(2, 3, 5);
   triangles.emplace_back(2, 4, 3);
   triangles.emplace_back(2, 1, 4);
}
