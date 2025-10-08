#include "WindowsScreenCapturer.h"


std::unordered_map<MonitorPosition, ImageData> WindowsScreenCapturer::Capture()
{
	std::unordered_map<MonitorPosition, ImageData> screenshots;

	// Hardcoded screen for now
	MonitorPosition position;
	position.xOrigin = 0;
	position.yOrigin = 1;

	ImageData screenData;
	screenData.width = 3960;
	screenData.height = 2160;
	screenData.channels = 3;
	const size_t dataSize = screenData.width * screenData.height * screenData.channels;
	screenData.data = new unsigned char[dataSize];
	screenshots[position] = screenData;

	memset(screenData.data, 0, dataSize * sizeof(unsigned char));

	return screenshots;
}