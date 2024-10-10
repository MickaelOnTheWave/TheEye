#include "GlRenderSphere.h"

#include <math.h>

using namespace std;

GlRenderSphere::GlRenderSphere(const unsigned int subdivisions)
{
   CreateSubdividedSphere(subdivisions);

   glGenVertexArrays(1, &vertexArrayObject);
   glBindVertexArray(vertexArrayObject);

   setupVertexBufferObject();
   setupElementBufferObject();
   setupVertexArrayAttributes();
}

void GlRenderSphere::PrepareRendering(const unsigned int shaderProgramId)
{
   glBindVertexArray(vertexArrayObject);
}

void GlRenderSphere::Render()
{
   const unsigned int indexCount = 4;
   glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
}

void GlRenderSphere::CreateSubdividedSphere(const unsigned int subdivisions)
{
   const GLfloat radius = 0.5f;
   const GLfloat value = radius * cos(M_PI_4);

   // Initial points
   points.push_back(Vector3(0.f, radius, 0.f));
   points.push_back(Vector3(0.f, -value, -value));
   points.push_back(Vector3(value, -value, value));
   points.push_back(Vector3(-value, -value, value));

   // Initial triangles
   triangles.push_back(Triangle(&points[0], &points[2], &points[3]));
   triangles.push_back(Triangle(&points[0], &points[1], &points[2]));
   triangles.push_back(Triangle(&points[0], &points[3], &points[1]));
   triangles.push_back(Triangle(&points[3], &points[2], &points[1]));
}

void GlRenderSphere::setupVertexBufferObject()
{
   const vector<float> vertices = CreatePointVectorData();

   glGenBuffers(1, &vertexBufferObject);
   glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
   glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(), GL_STATIC_DRAW);
}

void GlRenderSphere::setupElementBufferObject()
{
   const GLuint indices[] = {
       0,2,3,
       0,1,2,
       0,3,1,
       3,2,1,
   };

   glGenBuffers(1, &elementBufferObject);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

}

void GlRenderSphere::setupVertexArrayAttributes()
{
   const GLsizei stride = 3 * sizeof(float);

   // Position
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
   glEnableVertexAttribArray(0);

}

vector<float> GlRenderSphere::CreatePointVectorData() const
{
   vector<float> data;
   for (const auto& point : points)
   {
      data.push_back(point.X());
      data.push_back(point.Y());
      data.push_back(point.Z());
   }
   return data;
}
