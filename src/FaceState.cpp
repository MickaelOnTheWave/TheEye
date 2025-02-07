#include "FaceState.h"

FaceState::FaceState(const float _minTimeForStillness, const float _maxStillDelta)
  : previousFacePosition(std::nullopt),
  minTimeForStillness(_minTimeForStillness),
  maxStillDelta(_maxStillDelta)
{
}

void FaceState::Reset(std::optional<Vector3> initialPosition)
{
   previousFacePosition = initialPosition;
   currentT = 0.f;
}

bool FaceState::IsStill(const float deltaT, const Vector3 &currentPosition)
{
   if (previousFacePosition.has_value())
   {
      const Vector3& faceDelta = currentPosition - previousFacePosition.value();
      const bool isFaceStill = (faceDelta.ComputeLength() < maxStillDelta);
      if (isFaceStill)
      {
         currentT += deltaT;
         return (currentT > minTimeForStillness);
      }
   }
   return UpdateMovedFaceData(currentPosition);
}

bool FaceState::UpdateMovedFaceData(const Vector3 &currentPosition)
{
   currentT = 0.f;
   previousFacePosition = std::make_optional(currentPosition);
   return false;
}
