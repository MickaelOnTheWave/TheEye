#ifndef GLRENDERSUBDIVIDEDSPHERE_H
#define GLRENDERSUBDIVIDEDSPHERE_H

#include "GlRenderObject.h"

#include <glad/gl.h>
#include <list>
#include <vector>

#include "math/Triangle.h"
#include "math/Vector3.h"

class GlRenderSubdividedSphere : public GlRenderObject
{
public:
   GlRenderSubdividedSphere();

   void Initialize(const unsigned int subdivisions);

   void Render() override;

protected:
   virtual void PopulateInitialGeometry() = 0;
   void CreateSubdividedSphere(const unsigned int subdivisions);

   void setupVertexBufferObject();
   void setupElementBufferObject();

   std::vector<float> CreateVertexBufferData() const;
   std::vector<GLuint> CreateIndexData() const;

   std::list<Triangle> triangles;
   std::vector<Vector3> points;
   std::vector<Vector3> colors;

   std::vector<TexCoord> textureCoordinates;
};

#endif // GLRENDERSUBDIVIDEDSPHERE_H
