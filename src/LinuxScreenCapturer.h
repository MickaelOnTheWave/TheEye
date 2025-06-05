#ifndef LINUXSCREENCAPTURER_H
#define LINUXSCREENCAPTURER_H

#include "ImageData.h"

#include <vector>

class LinuxScreenCapturer
{
public:
   LinuxScreenCapturer() = default;

   virtual std::vector<ImageData> Capture();
};

#endif // LINUXSCREENCAPTURER_H
