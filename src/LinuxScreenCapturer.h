#ifndef LINUXSCREENCAPTURER_H
#define LINUXSCREENCAPTURER_H

#include "ImageData.h"

class LinuxScreenCapturer
{
public:
   LinuxScreenCapturer() = default;

   virtual ImageData Capture();
};

#endif // LINUXSCREENCAPTURER_H
