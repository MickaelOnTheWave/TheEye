#ifndef GLRENDERSPHERETETRAHEDRON_H
#define GLRENDERSPHERETETRAHEDRON_H

#include "GlRenderSubdividedSphere.h"

class GlRenderSphereTetrahedron : public GlRenderSubdividedSphere
{
public:
   virtual ~GlRenderSphereTetrahedron() = default;

   static GlRenderSphereTetrahedron* Create(const unsigned int subdivisions);

protected:
   void PopulateInitialGeometry() override;
};

#endif // GLRENDERSPHERETETRAHEDRON_H
