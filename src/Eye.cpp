#include "Eye.h"

Eye::Eye() : eyeAi(model)
{
}

void Eye::Initialize(GlRenderer *renderer, const ImageData& screenshot)
{
   renderer->SetRenderShader(GlRenderer::ShaderEnum::SIMPLE_TEXTURING);
   model.Initialize(renderer, screenshot);
   renderer->PrepareRendering();
}

void Eye::Update(std::optional<Vector3> facePosition, const float deltaT)
{
   eyeAi.Update(deltaT, facePosition);
}
