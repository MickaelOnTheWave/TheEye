#ifndef AbstractScreenCapturer_H
#define AbstractScreenCapturer_H

#include <unordered_map>

#include "ImageData.h"
#include "MonitorPosition.h"

class AbstractScreenCapturer
{
public:
	virtual ~AbstractScreenCapturer() = default;
	virtual std::unordered_map<MonitorPosition, ImageData> Capture() = 0;
};

#endif
