#include "Eye.h"

void Eye::Initialize(GlRenderer *renderer)
{
   model.Initialize(renderer);
   renderer->PrepareRendering();
}

void Eye::UpdateFaceData(std::optional<Vector3> facePosition)
{
   if (facePosition)
   {
      if (eyeClosed)
         model.Open();
      model.LookAt(facePosition.value());
   }
   else
      model.Close();

   eyeClosed = !facePosition.has_value();
}
