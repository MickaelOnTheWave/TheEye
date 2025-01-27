#include "EyeFaceAnalyzer.h"

EyeFaceAnalyzer::EyeFaceAnalyzer(std::mutex& _faceMutex, std::optional<Vector3>& _facePosition)
  : faceMutex(_faceMutex), facePosition(_facePosition)
{
}

int EyeFaceAnalyzer::Initialize()
{
   const bool ok = detector.Initialize("data/haarcascade_frontalface_default.xml");
   if (!ok)
      return ERROR_WITH_CASCADE_FILE;
   return OK;
}

void EyeFaceAnalyzer::RunThreadedDetection()
{
   threadId = std::thread(&EyeFaceAnalyzer::RunThreadedDetect, this);
}

void EyeFaceAnalyzer::StopThreadedDetection()
{
   keepRunning = false;
   threadId.join();
}

std::optional<Vector3> EyeFaceAnalyzer::Detect()
{
   const FaceDataVec faceData = detector.Detect();

   if (!faceData.empty())
   {
      const Vector3 facePosition = ConvertTo3dPosition(faceData.front(),
                                                       detector.GetVideoWidth(),
                                                       detector.GetVideoHeight());
      return facePosition;
   }
   return std::nullopt;
}

void EyeFaceAnalyzer::RunThreadedDetect()
{
   while (keepRunning)
   {
      faceMutex.lock();
      facePosition = Detect();
      faceMutex.unlock();
   }
}

Vector3 EyeFaceAnalyzer::ConvertTo3dPosition(const FaceRect& faceData, const int videoWidth, const int videoHeight) const
{
   const float faceCenterX = faceData.x + (faceData.width / 2.f);
   const float faceCenterY = faceData.y + (faceData.height / 2.f);

   const float videoXCenter = videoWidth / 2.f;
   const float x = -eyeCalibration.factorX * (faceCenterX + eyeCalibration.factorX - videoXCenter) / videoXCenter;

   const float videoYCenter = videoHeight / 2.f;
   const float y = eyeCalibration.factorY * (faceCenterY + eyeCalibration.factorY- videoYCenter) / videoYCenter;

   const float z = 5.f;
   return Vector3(x, y, z);
}

