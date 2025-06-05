#include "LinuxScreenCapturer.h"

#include <cstring>
#include <vector>
#include <X11/extensions/Xinerama.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

namespace
{
   ImageData CaptureSingleScreen(Display* display)
   {
      ImageData screenshot;
      Window root = DefaultRootWindow(display);

      XWindowAttributes attributes = {0};
      XGetWindowAttributes(display, root, &attributes);

      screenshot.width = attributes.width;
      screenshot.height = attributes.height;

      XImage* img = XGetImage(display, root, 0, 0 , screenshot.width, screenshot.height, AllPlanes, ZPixmap);
      screenshot.channels = img->bits_per_pixel / 8;

      const unsigned int componentCount = screenshot.width * screenshot.height * screenshot.channels;
      screenshot.data = new unsigned char[componentCount * sizeof(unsigned char)];
      memcpy(screenshot.data, img->data, componentCount);

      XDestroyImage(img);

      return screenshot;

   }

   std::vector<ImageData> CaptureMultiScreen(Display* display)
   {
      std::vector<ImageData> screenshots;
      int screenCount = 0;

      XineramaScreenInfo* screens = XineramaQueryScreens(display, &screenCount);
      Window root = DefaultRootWindow(display);

      for (int i=0; i<screenCount; ++i)
      {
         const XineramaScreenInfo& info = screens[i];

         XImage* img = XGetImage(display, root, info.x_org, info.y_org , info.width, info.height, AllPlanes, ZPixmap);

         ImageData screenshot;
         screenshot.width = info.width;
         screenshot.height = info.height;
         screenshot.channels = img->bits_per_pixel / 8;

         const unsigned int componentCount = screenshot.width * screenshot.height * screenshot.channels;
         screenshot.data = new unsigned char[componentCount * sizeof(unsigned char)];
         memcpy(screenshot.data, img->data, componentCount);

         XDestroyImage(img);
         screenshots.push_back(screenshot);
      }
      return screenshots;
   }
}


std::vector<ImageData> LinuxScreenCapturer::Capture()
{
   std::vector<ImageData> screenshots;
   Display* display = XOpenDisplay(nullptr);

   if (XineramaIsActive(display))
      screenshots = CaptureMultiScreen(display);
   else
      screenshots.push_back(CaptureSingleScreen(display));

   XCloseDisplay(display);
   return screenshots;
}
