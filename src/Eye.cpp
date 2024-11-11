#include "Eye.h"

#include <cmath>

#include "objects/GlRenderSpherePolar.h"

void Eye::Initialize(GlRenderer *renderer)
{
   const unsigned int subdivisions = 4;
   const GlRenderObject::Texture wallTexture = {"data/eye-blue.jpg", GL_RGB};
   auto eyeSphere = new GlRenderSpherePolar();
   eyeSphere->SetTextureProjection(GlRenderSpherePolar::TextureMap::HalfWrap);
   eyeSphere->Initialize(subdivisions, {wallTexture});

   initialTransform = Matrix4x4::Scale(0.5f, 0.5f, -0.5f);
   renderInstance = new GlRenderedInstance(eyeSphere, initialTransform);

   renderer->AddRenderObject(renderInstance);
}

void Eye::LookAt(const Vector3 &target)
{
   const Vector3 eyePosition(0.f, 0.f, 0.f);
   Vector3 directionVec = target - eyePosition;
   directionVec.Normalize();

   const float angleXZ = -asin(directionVec.X());
   const Matrix4x4 yRotation = Matrix4x4::RotationY(angleXZ);

   const float angleXY = -asin(directionVec.Y());
   const Matrix4x4 xRotation = Matrix4x4::RotationX(angleXY);

   renderInstance->SetTransform(initialTransform * yRotation * xRotation);
}
