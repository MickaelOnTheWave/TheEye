#ifndef GLRENDERSPHERE_H
#define GLRENDERSPHERE_H

#include "GlRenderObject.h"

class GlRenderSphere : public GlRenderObject
{
public:
   GlRenderSphere(const unsigned int subdivisions);

   void PrepareRendering(const unsigned int shaderProgramId) override;

   void Render() override;

private:
};

#endif // GLRENDERSPHERE_H
