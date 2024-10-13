#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Vector3.h"

#include <vector>

class Triangle
{
public:
   Triangle(const unsigned int _indexP1, const unsigned int _indexP2,
            const unsigned int _indexP3);

   unsigned int GetP1() const;
   unsigned int GetP2() const;
   unsigned int GetP3() const;

   void Subdivide(std::vector<Vector3>& pointRepository,
                  std::vector<Triangle>& triangleRepository,
                  const std::size_t currentTriangleIndex) const;

private:
   void MovePointToSphere(Vector3* point) const;

   unsigned int indexP1;
   unsigned int indexP2;
   unsigned int indexP3;
};

#endif // TRIANGLE_H
