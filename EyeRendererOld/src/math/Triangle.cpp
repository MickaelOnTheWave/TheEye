#include "Triangle.h"

using namespace std;

Triangle::Triangle(const unsigned int _indexP1, const unsigned int _indexP2, const unsigned int _indexP3)
  : indexP1(_indexP1), indexP2(_indexP2), indexP3(_indexP3)
{

}

unsigned int Triangle::GetP1() const
{
   return indexP1;
}

unsigned int Triangle::GetP2() const
{
   return indexP2;
}

unsigned int Triangle::GetP3() const
{
   return indexP3;
}

void Triangle::Subdivide(std::vector<Vector3> &pointRepository, std::vector<Triangle> &triangleRepository,
                         const size_t currentTriangleIndex) const
{
   Vector3& p1 = pointRepository[indexP1];
   Vector3& p2 = pointRepository[indexP2];
   Vector3& p3 = pointRepository[indexP3];

   Vector3* p4 = p1.CreateMedian(p2);
   Vector3* p5 = p2.CreateMedian(p3);
   Vector3* p6 = p3.CreateMedian(p1);

   MovePointToSphere(p4);
   MovePointToSphere(p5);
   MovePointToSphere(p6);

   pointRepository.push_back(*p4);
   const unsigned int indexP4 = pointRepository.size()-1;
   pointRepository.push_back(*p5);
   const unsigned int indexP5 = pointRepository.size()-1;
   pointRepository.push_back(*p6);
   const unsigned int indexP6 = pointRepository.size()-1;
   delete p4;
   delete p5;
   delete p6;

   triangleRepository.erase(triangleRepository.begin() + currentTriangleIndex);
   triangleRepository.emplace_back(indexP4, indexP5, indexP6);
   triangleRepository.emplace_back(indexP1, indexP4, indexP6);
   triangleRepository.emplace_back(indexP4, indexP2, indexP5);
   triangleRepository.emplace_back(indexP6, indexP5, indexP3);
}

void Triangle::MovePointToSphere(Vector3 *point) const
{
   // TODo fix that, this is hardcoded to a 0.5 radius sphere

   const float desiredRadius = 1.f;
   const float currentRadius = point->ComputeLength();
   *point *= (desiredRadius / currentRadius);
}

