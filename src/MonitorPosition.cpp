#include "MonitorPosition.h"

MonitorPosition::MonitorPosition()
    : xOrigin(0), yOrigin(0)
{
}

bool MonitorPosition::operator==(const MonitorPosition other) const
{
    return xOrigin == other.xOrigin && yOrigin == other.yOrigin;
}

