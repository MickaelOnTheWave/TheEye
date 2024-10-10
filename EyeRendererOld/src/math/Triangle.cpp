#include "Triangle.h"

using namespace std;

Triangle::Triangle(Vector3 *_p1, Vector3 *_p2, Vector3 *_p3)
  : p1(_p1), p2(_p2), p3(_p3)
{
}

vector<Triangle> Triangle::Subdivide() const
{
   Vector3* p4 = p1->CreateMedian(*p2);
   Vector3* p5 = p2->CreateMedian(*p3);
   Vector3* p6 = p3->CreateMedian(*p1);

   vector<Triangle> subTriangles;
   subTriangles.push_back(Triangle(p4, p5, p6));
   subTriangles.push_back(Triangle(p1, p4, p6));
   subTriangles.push_back(Triangle(p4, p2, p5));
   subTriangles.push_back(Triangle(p6, p5, p3));
   return subTriangles;
}
