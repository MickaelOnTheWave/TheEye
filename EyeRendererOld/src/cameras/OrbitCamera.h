#ifndef ORBITCAMERA_H
#define ORBITCAMERA_H

#include "AbstractGlCamera.h"

class OrbitCamera : public AbstractGlCamera
{
public:
   OrbitCamera(const float _targetX,
               const float _targetY,
               const float _targetZ);

   void RotateInX(const double angle);
   void rotateInY(const double angle);

private:
   float targetX, targetY, targetZ;
};

#endif // ORBITCAMERA_H
