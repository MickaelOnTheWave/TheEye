#include "Eye.h"

void Eye::Initialize(GlRenderer *renderer)
{
   model.Initialize(renderer);
   renderer->PrepareRendering();
}

void Eye::UpdateFaceData(std::optional<Vector3> facePosition)
{
   if (facePosition)
      model.LookAt(facePosition.value());
   else
      model.LookAt(Vector3(0.f, -1.f, 0.f));
}
