#ifndef ABSTRACTGLCAMERA_H
#define ABSTRACTGLCAMERA_H

#include <glad/gl.h>

#include "Matrix4x4.h"

class AbstractGlCamera
{
public:
   AbstractGlCamera();

   Matrix4x4 getTransformMatrix();

protected:
   Matrix4x4 transformMatrix;
};

#endif // ABSTRACTGLCAMERA_H
