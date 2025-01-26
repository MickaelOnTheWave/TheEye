#include "LinuxScreenCapturer.h"

#include <cstring>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

ImageData LinuxScreenCapturer::Capture()
{
   ImageData screenshot;

   Display* display = XOpenDisplay(nullptr);
   Window root = DefaultRootWindow(display);

   XWindowAttributes attributes = {0};
   XGetWindowAttributes(display, root, &attributes);

   screenshot.width = attributes.width;
   screenshot.height = attributes.height;

   XImage* img = XGetImage(display, root, 0, 0 , screenshot.width, screenshot.height, AllPlanes, ZPixmap);
   //BitsPerPixel = img->bits_per_pixel;
   //Pixels.resize(screenshot.width * screenshot.height * 4);

   //memcpy(&Pixels[0], img->data, Pixels.size());

   const unsigned int channels = 3;
   screenshot.data = new unsigned char[screenshot.width * screenshot.height * sizeof(unsigned char) * channels];
   memcpy(screenshot.data, img->data, screenshot.width * screenshot.height * channels);

   XDestroyImage(img);
   XCloseDisplay(display);

   return screenshot;
}
