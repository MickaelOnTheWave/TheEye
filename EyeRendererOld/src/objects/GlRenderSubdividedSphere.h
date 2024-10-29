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

   void Initialize(const unsigned int subdivisions, const TextureVec &_textureFiles);

   void Render() override;

protected:
   virtual void PopulateInitialGeometry() = 0;
   void CreateSubdividedSphere(const unsigned int subdivisions);

   std::vector<GLfloat> CreateVertexBufferData() const override;
   std::vector<GLuint> CreateIndexData() const override;

   std::list<Triangle> triangles;
   std::vector<Vector3> points;
   std::vector<Vector3> colors;

   std::vector<TexCoord> textureCoordinates;
};

#endif // GLRENDERSUBDIVIDEDSPHERE_H
