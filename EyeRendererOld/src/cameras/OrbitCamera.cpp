#include "OrbitCamera.h"

OrbitCamera::OrbitCamera(const float _targetX, const float _targetY, const float _targetZ)
   : AbstractGlCamera(),
     targetX(_targetX), targetY(_targetY), targetZ(_targetZ)
{
}
