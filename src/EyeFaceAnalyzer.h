#ifndef EYEFACEANALYZER_H
#define EYEFACEANALYZER_H

#include <optional>

#include "FaceLiveDetector.h"
#include "math/Vector3.h"

class EyeFaceAnalyzer
{
public:
   EyeFaceAnalyzer();

   int Initialize();

   std::optional<Vector3> Detect();

   static const int OK = 0;
   static const int ERROR_WITH_CASCADE_FILE = 1;

private:
   Vector3 ConvertTo3dPosition(const FaceRect& faceData, const int videoWidth, const int videoHeight) const;

   struct EyeCalibrationData
   {
      float offsetX = 0.f;
      float offsetY = 0.f;
      float factorX = 1.f;
      float factorY = 1.f;
   };

   EyeCalibrationData eyeCalibration;
   FaceLiveDetector detector;
};

#endif // EYEFACEANALYZER_H
