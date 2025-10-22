#ifndef WindowsScreenCapturer_H
#define WindowsScreenCapturer_H

#include "../AbstractScreenCapturer.h"

#include <basetsd.h>
#include <unordered_map>
#include "../MonitorPosition.h"

class WindowsScreenCapturer : public AbstractScreenCapturer
{
public:
	WindowsScreenCapturer();
	~WindowsScreenCapturer();

	std::unordered_map<MonitorPosition, ImageData> Capture() override;

private:
	ULONG_PTR gdiplusToken;
};
#endif
