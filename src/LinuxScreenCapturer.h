#ifndef LINUXSCREENCAPTURER_H
#define LINUXSCREENCAPTURER_H

struct ImageData
{
   unsigned int width;
   unsigned int height;
   unsigned char* data;
};

class LinuxScreenCapturer
{
public:
   LinuxScreenCapturer() = default;

   virtual ImageData Capture();
};

#endif // LINUXSCREENCAPTURER_H
