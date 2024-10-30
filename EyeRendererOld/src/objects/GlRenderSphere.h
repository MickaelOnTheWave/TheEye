#ifndef GLRENDERSPHERE_H
#define GLRENDERSPHERE_H

#include "GlRenderObject.h"

#include <glad/gl.h>
#include <vector>

#include "math/Triangle.h"
#include "math/Vector3.h"

class GlRenderSphere : public GlRenderObject
{
public:
   GlRenderSphere() = default;

   void Initialize(const unsigned int subdivisions, const TextureVec &_textureFiles);

protected:
   virtual void PopulateGeometry(const unsigned int subdivisions) = 0;

   std::vector<GLfloat> CreateVertexBufferData() const override;

   std::vector<Vector3> points;
   std::vector<Vector3> colors;
   std::vector<TexCoord> textureCoordinates;
};

#endif // GLRENDERSPHERE_H
