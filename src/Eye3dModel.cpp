#include "Eye3dModel.h"

#include <cmath>

#include "objects/GlRenderSpherePolar.h"

void Eye3dModel::Initialize(GlRenderer *renderer)
{
   const unsigned int eyeTextureId = renderer->AddTexture("data/eye-blue.jpg", GL_RGB);
   const unsigned int whiteTextureId = renderer->AddTexture(Vector3(1,1,1));
   Material* eyeMaterial = new Material("eye");
   eyeMaterial->diffuseTextureId = eyeTextureId;
   eyeMaterial->specularTextureId = whiteTextureId;
   eyeMaterial->shininess = 20.f;
   renderer->AddMaterial(eyeMaterial);

   auto eyeSphere = new GlRenderSpherePolar(eyeMaterial);
   eyeSphere->SetTextureProjection(GlRenderSpherePolar::TextureMap::HalfWrap);

   const unsigned int subdivisions = 4;
   eyeSphere->Initialize(subdivisions);

   initialTransform = Matrix4x4::Scale(0.5f, 0.5f, -0.5f);
   renderInstance = new GlRenderedInstance(eyeSphere, initialTransform);

   renderer->AddRenderObject(renderInstance);
}

void Eye3dModel::LookAt(const Vector3 &target)
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
