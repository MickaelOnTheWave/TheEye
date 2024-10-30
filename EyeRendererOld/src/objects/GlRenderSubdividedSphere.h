#ifndef GLRENDERSUBDIVIDEDSPHERE_H
#define GLRENDERSUBDIVIDEDSPHERE_H

#include "GlRenderSphere.h"

#include <glad/gl.h>
#include <list>
#include <vector>

#include "math/Triangle.h"

class GlRenderSubdividedSphere : public GlRenderSphere
{
public:
   GlRenderSubdividedSphere() = default;

   void Render() override;

protected:
   void PopulateGeometry(const unsigned int subdivisions) override;

   virtual void PopulateInitialGeometry() = 0;
   void CreateSubdividedSphere(const unsigned int subdivisions);

   std::vector<GLuint> CreateIndexData() const override;

   std::list<Triangle> triangles;
};

#endif // GLRENDERSUBDIVIDEDSPHERE_H
