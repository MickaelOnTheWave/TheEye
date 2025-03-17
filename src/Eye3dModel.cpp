#include "Eye3dModel.h"

#include <cmath>

#include "objects/GlRenderPlane.h"
#include "objects/GlRenderSphereArc.h"
#include "objects/GlRenderSpherePolar.h"

namespace
{
   const float upperLidOpenAngle = 0.75f;
   const float upperLidClosedAngle = 0.5f;
   const float lowerLidOpenAngle = -0.75f;
   const float lowerLidClosedAngle = -0.5f;
}

void Eye3dModel::Initialize(GlRenderer *renderer, const ImageData& screenshot)
{
   const unsigned int sphereSubdivisions = 4;

   whiteTextureId = renderer->AddTexture(Vector3(1,1,1));
   Material* eyeMaterial = CreateEyeMaterial(renderer);
   Material* skinMaterial = CreateSkinMaterial(renderer, screenshot);

   auto eyeSphere = new GlRenderSpherePolar(eyeMaterial, sphereSubdivisions,
                                            GlRenderSpherePolar::TextureMap::HalfWrap);
   eyeSphere->Initialize();

   initialEyeballTransform = Matrix4x4::Scale(0.5f, 0.5f, -0.5f);
   eyeBall = new GlRenderedInstance(eyeSphere, initialEyeballTransform);

   const float textureScale = 1.f;
   auto eyePlane = new GlRenderPlane(skinMaterial, textureScale);
   eyePlane->Initialize();

   Matrix4x4 faceTransform = Matrix4x4::Scale(2.f) * Matrix4x4::Translation(Vector3(0, 0, -0.2f));
   auto faceSkin = new GlRenderedInstance(eyePlane, faceTransform);

   const float eyeLidScale = 0.51f;
   auto eyeLidUp = new GlRenderSphereArc(M_PI, skinMaterial, sphereSubdivisions,
                                       GlRenderSpherePolar::TextureMap::HalfPolarProjection,
                                       eyeLidScale, -eyeLidScale);
   eyeLidUp->Initialize();

   auto eyeLidDown = new GlRenderSphereArc(M_PI, skinMaterial, sphereSubdivisions,
                                         GlRenderSpherePolar::TextureMap::HalfPolarProjection,
                                         eyeLidScale, eyeLidScale);
   eyeLidDown->Initialize();


   const Matrix4x4 scale = Matrix4x4::Scale(eyeLidScale);
   const float eyeLidAngle = M_PI * 0.5f;
   initialUpperLidTransform = scale *  Matrix4x4::RotationY(eyeLidAngle);
   upperLid = new GlRenderedInstance(eyeLidUp, initialUpperLidTransform);
   initialLowerLidTransform = scale *  Matrix4x4::RotationY(eyeLidAngle);
   lowerLid = new GlRenderedInstance(eyeLidDown, initialLowerLidTransform);

   renderer->AddRenderObject(eyeBall, GlRenderer::ShaderEnum::PROCEDURAL_EYE);
   renderer->AddRenderObject(faceSkin);
   renderer->AddRenderObject(upperLid);
   renderer->AddRenderObject(lowerLid);

   // Eye starts closed
   Close(1.f);
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

   eyeBall->SetTransform(initialEyeballTransform * yRotation * xRotation);
}

void Eye3dModel::Open(const float t)
{
   const float upperLidDelta = upperLidOpenAngle - upperLidClosedAngle;
   const float upperLidFactor = upperLidClosedAngle + upperLidDelta * t;

   const float lowerLidDelta = lowerLidOpenAngle - lowerLidClosedAngle;
   const float lowerLidFactor = lowerLidClosedAngle + lowerLidDelta * t;

   upperLidAngle = M_PI * upperLidFactor;
   lowerLidAngle = M_PI * lowerLidFactor;
   UpdateLidTransform();
}

void Eye3dModel::Close(const float t)
{
   const float upperLidDelta = upperLidClosedAngle - upperLidOpenAngle;
   const float upperLidFactor = upperLidOpenAngle + upperLidDelta * t;

   const float lowerLidDelta = lowerLidClosedAngle - lowerLidOpenAngle;
   const float lowerLidFactor = lowerLidOpenAngle + lowerLidDelta * t;

   upperLidAngle = M_PI * upperLidFactor;
   lowerLidAngle = M_PI * lowerLidFactor;
   UpdateLidTransform();
}

Material *Eye3dModel::CreateEyeMaterial(GlRenderer *renderer) const
{
   const int channelCount = 3;
   const unsigned int eyeTextureId = renderer->AddTexture("data/eye-blue.jpg", channelCount);
   Material* eyeMaterial = new Material("eye");
   eyeMaterial->diffuseTextureId = eyeTextureId;
   eyeMaterial->specularTextureId = whiteTextureId;
   eyeMaterial->shininess = 20.f;
   renderer->AddMaterial(eyeMaterial);
   return eyeMaterial;
}

Material *Eye3dModel::CreateSkinMaterial(GlRenderer *renderer, const ImageData& screenshot) const
{
   const unsigned int textureId = renderer->AddTexture(screenshot, GL_BGRA);
   auto material = new Material("skin");
   material->diffuseTextureId = textureId;
   material->specularTextureId = whiteTextureId;
   material->shininess = 5.f;
   renderer->AddMaterial(material);
   return material;
}

void Eye3dModel::UpdateLidTransform()
{
   const Matrix4x4 upperLidRotation = Matrix4x4::RotationX(upperLidAngle);
   upperLid->SetTransform(initialUpperLidTransform * upperLidRotation);

   const Matrix4x4 lowerLidRotation = Matrix4x4::RotationX(lowerLidAngle);
   lowerLid->SetTransform(initialLowerLidTransform * lowerLidRotation);
}
