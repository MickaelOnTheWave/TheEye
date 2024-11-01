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

private:
   void PopulateCoordinates(const unsigned int horizontalPointCount,
                            const unsigned int verticalPointCount);
   void PopulateTriangles(const unsigned int horizontalPointCount,
                          const unsigned int verticalPointCount);
};

#endif // GLRENDERSPHEREPOLAR_H
