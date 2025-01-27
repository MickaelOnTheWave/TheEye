#ifndef EYE_H
#define EYE_H

#include <optional>

#include "Eye3dModel.h"
#include "GlRenderer.h"
#include "ImageData.h"
#include "math/Vector3.h"

class Eye
{
public:
   Eye() = default;

   void Initialize(GlRenderer* renderer, const ImageData& screenshot);

   void Update(std::optional<Vector3> facePosition, const float deltaT);

private:
   void UpdateOpening(const float animationT);
   void UpdateClosing(const float animationT);

   std::string GetStateDescription() const;

   Eye3dModel model;

   enum class EyeState
   {
      CLOSED,
      CLOSING,
      OPEN,
      OPENING
   };

   float tAnimation;

   EyeState eyeState;

};

#endif // EYE_H
