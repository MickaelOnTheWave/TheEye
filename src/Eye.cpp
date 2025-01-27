#include "Eye.h"

//#include <iostream>

void Eye::Initialize(GlRenderer *renderer, const ImageData& screenshot)
{
   eyeState = EyeState::CLOSED;
   renderer->SetRenderShader(GlRenderer::ShaderEnum::SIMPLE_TEXTURING);
   model.Initialize(renderer, screenshot);
   renderer->PrepareRendering();
}

void Eye::Update(std::optional<Vector3> facePosition, const float deltaT)
{
   const float tEndAnimation  = 1.f;
   switch (eyeState)
   {
      case EyeState::CLOSED:
      {
         if (facePosition)
         {
            eyeState = EyeState::OPENING;
            tAnimation = 0.f;
         }
         break;
      }
      case EyeState::CLOSING:
      {
         if (tAnimation > tEndAnimation)
         {
            eyeState = EyeState::CLOSED;
            tAnimation = tEndAnimation;
         }
         else
            tAnimation += deltaT;
         UpdateClosing(tAnimation);

         break;
      }
      case EyeState::OPEN:
      {
         if (facePosition)
            model.LookAt(facePosition.value());
         else
         {
            eyeState = EyeState::CLOSING;
            tAnimation = 0.f;
         }
         break;
      }
      case EyeState::OPENING:
      {
         if (tAnimation > tEndAnimation)
         {
            eyeState = EyeState::OPEN;
            tAnimation = tEndAnimation;
         }
         else
            tAnimation += deltaT;
         UpdateOpening(tAnimation);
         break;
      }
   };

   //std::cout << "EyeState : " << GetStateDescription() << std::endl;
}

void Eye::UpdateOpening(const float animationT)
{
   model.Open(animationT);
}

void Eye::UpdateClosing(const float animationT)
{
   model.Close(animationT);
}

std::string Eye::GetStateDescription() const
{
   if (eyeState == EyeState::OPEN)
      return "Open";
   else if (eyeState == EyeState::OPENING)
      return "Opening";
   else if (eyeState == EyeState::CLOSED)
      return "Closed";
   else if (eyeState == EyeState::CLOSING)
      return "Closing";
   else
      return "Unknown";
}
