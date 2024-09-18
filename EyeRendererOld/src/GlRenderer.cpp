#include "GlRenderer.h"

GlRenderer::GlRenderer(GlCamera& _camera)
   : camera(_camera), shaderProgram("data/basic.vert", "data/basic.frag")
{
   glViewport(0, 0, 1024, 768);
   glGenBuffers(1, &vbo);
   glBindBuffer(GL_ARRAY_BUFFER, vbo);

   initializeRendering();
}

GlRenderer::~GlRenderer()
{
}

void GlRenderer::render()
{
   glClearColor(0.0f, 0.0f, 0.3f, 1.0f);
   glClear(GL_COLOR_BUFFER_BIT);

   glPushMatrix();

      glMultMatrixf(camera.getTransformMatrix().getData());

      shaderProgram.use();
      glBindVertexArray(vao);
      glColor3f(0.7f, 0.2f, 0.0f);
      glDrawArrays(GL_TRIANGLES, 0, 3);

   glPopMatrix();
}

void GlRenderer::initializeRendering()
{
   shaderProgram.use();

   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
   glEnableVertexAttribArray(0);

   float vertices[] = {
       -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f,

      0.0f,  0.5f, 0.0f,
      0.5f, -0.5f, 0.0f,
      0.0f, -0.5f, -0.5f,

      0.0f,  0.5f, 0.0f,
      0.0f, -0.5f, -0.5f,
      -0.5f, -0.5f, 0.0f

   };

   glGenVertexArrays(1, &vao);
   glBindVertexArray(vao);
   glBindBuffer(GL_ARRAY_BUFFER, vbo);
   glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
   glEnableVertexAttribArray(0);
}
