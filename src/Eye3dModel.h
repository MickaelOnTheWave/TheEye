#ifndef EYE3DMODEL_H
#define EYE3DMODEL_H

#include "GlRenderer.h"
#include "math/Vector3.h"

class Eye3dModel
{
public:
   Eye3dModel() = default;

   void Initialize(GlRenderer* renderer);

   void LookAt(const Vector3& target);

private:
   GlRenderedInstance* renderInstance = nullptr;
   Matrix4x4 initialTransform;

};

#endif // EYE3DMODEL_H
