#ifndef FACESTATE_H
#define FACESTATE_H

#include <optional>
#include "math/Vector3.h"

class FaceState
{
public:
   FaceState(const float _minTimeForStillness, const float _maxStillDelta = 0.05f);

   void Reset(std::optional<Vector3> initialPosition = std::nullopt);

   bool IsStill(const float deltaT, const Vector3& currentPosition);

private:
   bool UpdateMovedFaceData(const Vector3 &currentPosition);

   std::optional<Vector3> previousFacePosition;
   const float minTimeForStillness;
   const float maxStillDelta;
   float currentT = 0.f;
};

#endif // FACESTATE_H
