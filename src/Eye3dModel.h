#ifndef EYE3DMODEL_H
#define EYE3DMODEL_H

#include "GlRenderer.h"
#include "math/Vector3.h"

class Eye3dModel
{
public:
   Eye3dModel() = default;

   void Initialize(GlRenderer* renderer);

   void LookAt(const Vector3& target);
   void Open();
   void Close();

private:
   Material* CreateEyeMaterial(GlRenderer *renderer) const;
   Material* CreateSkinMaterial(GlRenderer *renderer) const;

   void UpdateLidTransform();

   GlRenderedInstance* eyeBall = nullptr;
   GlRenderedInstance* upperLid = nullptr;
   GlRenderedInstance* lowerLid = nullptr;

   Matrix4x4 initialEyeballTransform;
   Matrix4x4 initialUpperLidTransform;
   Matrix4x4 initialLowerLidTransform;
   unsigned int whiteTextureId;
   float upperLidAngle;
   float lowerLidAngle;
};

#endif // EYE3DMODEL_H
