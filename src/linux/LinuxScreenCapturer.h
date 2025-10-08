#ifndef LINUXSCREENCAPTURER_H
#define LINUXSCREENCAPTURER_H

#include "ImageData.h"

#include <unordered_map>

struct MonitorPosition
{
   MonitorPosition();

   bool operator==(const MonitorPosition other) const;

   int xOrigin;
   int yOrigin;

};

namespace std {
template <>
struct hash<MonitorPosition> {
   size_t operator()(const MonitorPosition& key) const {
      size_t h1 = std::hash<int>()(key.xOrigin);
      size_t h2 = std::hash<int>()(key.yOrigin);
      return h1 ^ (h2 << 1);
   }
};
}

class LinuxScreenCapturer
{
public:
   LinuxScreenCapturer() = default;

   virtual std::unordered_map<MonitorPosition, ImageData> Capture();
};

#endif // LINUXSCREENCAPTURER_H
