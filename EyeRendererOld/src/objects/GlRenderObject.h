#ifndef GLRENDEROBJECT_H
#define GLRENDEROBJECT_H

class GlRenderObject
{
public:
   GlRenderObject();
   virtual ~GlRenderObject() = default;

   virtual void PrepareRendering(const unsigned int shaderProgramId) = 0;

   virtual void Render() = 0;

protected:
   unsigned int vertexArrayObject;
   unsigned int vertexBufferObject;
   unsigned int elementBufferObject;
};

#endif // GLRENDEROBJECT_H
