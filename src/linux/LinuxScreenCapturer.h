#ifndef LINUXSCREENCAPTURER_H
#define LINUXSCREENCAPTURER_H

#include <unordered_map>

#include "../AbstractScreenCapturer.h"
#include "ImageData.h"
#include "MonitorPosition.h"

class LinuxScreenCapturer : public AbstractScreenCapturer
{
public:
   LinuxScreenCapturer() = default;
   virtual ~LinuxScreenCapturer() = default;

   std::unordered_map<MonitorPosition, ImageData> Capture() override;
};

#endif // LINUXSCREENCAPTURER_H
