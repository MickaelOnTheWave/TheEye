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
   screenshot.channels = img->bits_per_pixel / 8;
   //BitsPerPixel = img->bits_per_pixel;
   //Pixels.resize(screenshot.width * screenshot.height * 4);

   //memcpy(&Pixels[0], img->data, Pixels.size());

   const unsigned int componentCount = screenshot.width * screenshot.height * screenshot.channels;
   screenshot.data = new unsigned char[componentCount * sizeof(unsigned char)];
   memcpy(screenshot.data, img->data, componentCount);

   XDestroyImage(img);
   XCloseDisplay(display);

   return screenshot;
}
