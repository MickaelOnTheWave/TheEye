#ifndef EYE_H
#define EYE_H

#include <optional>

#include "Eye3dModel.h"
#include "GlRenderer.h"
#include "math/Vector3.h"

class Eye
{
public:
   Eye() = default;

   void Initialize(GlRenderer* renderer);

   void UpdateFaceData(std::optional<Vector3> facePosition);

private:
   Eye3dModel model;
};

#endif // EYE_H
