#ifndef GLRENDERSPHEREPOLAR_H
#define GLRENDERSPHEREPOLAR_H

#include "GlRenderSphere.h"

class GlRenderSpherePolar : public GlRenderSphere
{
public:
   GlRenderSpherePolar() = default;
   virtual ~GlRenderSpherePolar() = default;

protected:
   void PopulateGeometry(const unsigned int subdivisions) override;
};

#endif // GLRENDERSPHEREPOLAR_H
