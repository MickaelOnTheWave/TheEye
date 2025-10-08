#ifndef WindowsScreenCapturer_H
#define WindowsScreenCapturer_H

#include "../AbstractScreenCapturer.h"

#include <unordered_map>
#include "../MonitorPosition.h"

class WindowsScreenCapturer : public AbstractScreenCapturer
{
public:
	virtual ~WindowsScreenCapturer() = default;
	std::unordered_map<MonitorPosition, ImageData> Capture() override;	
};
#endif
