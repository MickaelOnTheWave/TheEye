#ifndef EYE_H
#define EYE_H

#include <optional>

#include "Eye3dModel.h"
#include "EyeStateMachine.h"
#include "GlRenderer.h"
#include "ImageData.h"
#include "math/Vector3.h"

class Eye
{
public:
   Eye();

   void Initialize(GlRenderer* renderer, const ImageData& screenshot);

   void Update(std::optional<Vector3> facePosition, const float deltaT);

private:
   Eye3dModel model;
   EyeStateMachine eyeAi;
};

#endif // EYE_H
