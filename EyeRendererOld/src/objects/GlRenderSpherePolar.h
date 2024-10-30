#ifndef GLRENDERSPHEREOCTAHEDRON2_H
#define GLRENDERSPHEREOCTAHEDRON2_H

#include "GlRenderSphere.h"

class GlRenderSphereOctahedron2 : public GlRenderSphere
{
public:
   GlRenderSphereOctahedron2() = default;
   virtual ~GlRenderSphereOctahedron2() = default;

protected:
   void PopulateGeometry(const unsigned int subdivisions) override;
};

#endif // GLRENDERSPHEREOCTAHEDRON_H
