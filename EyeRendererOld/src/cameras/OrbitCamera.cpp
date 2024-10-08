#include "OrbitCamera.h"

OrbitCamera::OrbitCamera(const float _targetX, const float _targetY, const float _targetZ)
   : AbstractGlCamera(),
     targetX(_targetX), targetY(_targetY), targetZ(_targetZ)
{
   rotationSpeed = 0.002f;
}

void OrbitCamera::RotateInX(const double angle)
{
   currentAngleX += angle * rotationSpeed;
   transformMatrix = Matrix4x4::RotationX(currentAngleX);
}

void OrbitCamera::RotateInY(const double angle)
{
   currentAngleY += angle * rotationSpeed;
   transformMatrix = Matrix4x4::RotationY(currentAngleY);
}
