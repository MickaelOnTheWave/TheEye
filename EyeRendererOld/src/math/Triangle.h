#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Vector3.h"

#include <vector>

class Triangle
{
public:
   Triangle(Vector3* _p1, Vector3* _p2, Vector3* _p3);

   // The resulting triangles have the ownership of their points.
   // The points need to be stored somewhere else for the triangle to
   // be using them as reference, as it is meant.
   std::vector<Triangle> Subdivide() const;

private:
   Vector3* p1;
   Vector3* p2;
   Vector3* p3;
};

#endif // TRIANGLE_H
