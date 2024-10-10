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
   GlRenderSphere(const unsigned int subdivisions);

   void PrepareRendering(const unsigned int shaderProgramId) override;

   void Render() override;

private:
   void CreateSubdividedSphere(const unsigned int subdivisions);

   void setupVertexBufferObject();
   void setupElementBufferObject();
   void setupVertexArrayAttributes();

   std::vector<float> CreatePointVectorData() const;

   std::vector<Triangle> triangles;
   std::vector<Vector3> points;
};

#endif // GLRENDERSPHERE_H
