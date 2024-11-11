#ifndef EYE_H
#define EYE_H

#include "GlRenderer.h"
#include "math/Vector3.h"

class Eye
{
public:
   Eye() = default;

   void Initialize(GlRenderer* renderer);

   void LookAt(const Vector3& target);

private:
   GlRenderedInstance* renderInstance = nullptr;

   Matrix4x4 initialTransform;
};

#endif // EYE_H
