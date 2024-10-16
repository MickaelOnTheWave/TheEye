#include "GlRenderSphere.h"

#include <math.h>

using namespace std;

GlRenderSphere::GlRenderSphere(const unsigned int subdivisions)
{
   const std::pair<std::string, int> faceTexture = {"data/awesomeface.png", GL_RGBA};
   const std::pair<std::string, int> wallTexture = {"data/wall.jpg", GL_RGB};
   const TextureVec _textureFiles = {wallTexture, faceTexture};

   CreateSubdividedSphere(subdivisions);

   glGenVertexArrays(1, &vertexArrayObject);
   glBindVertexArray(vertexArrayObject);

   setupVertexBufferObject();
   setupElementBufferObject();
   setupTextureObject(_textureFiles);

   setupVertexArrayAttributes();
}

void GlRenderSphere::Render()
{
   const unsigned int vertexPerFace = 3;
   const unsigned int faceCount = triangles.size();
   const unsigned int indexCount = vertexPerFace * faceCount;
   glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
}

void GlRenderSphere::CreateSubdividedSphere(const unsigned int subdivisions)
{
   const GLfloat radius = 1.f;
   const float angle = M_PI / 6.f;
   const GLfloat cosAngle = cos(angle);
   const GLfloat sinAngle = sin(angle);

   // Initial points
   points.emplace_back(0.f, radius, 0.f);
   points.emplace_back(0.f, -sinAngle, -cosAngle);
   points.emplace_back(cosAngle * sinAngle, -sinAngle, cosAngle * cosAngle);
   points.emplace_back(-cosAngle * sinAngle, -sinAngle, cosAngle * cosAngle);

   textureCoordinates.emplace_back(0.5f, 1.f);
   textureCoordinates.emplace_back(0.f, 0.f);
   textureCoordinates.emplace_back(1.f, 0.f);
   textureCoordinates.emplace_back(0.f, 0.f);

   // Initial triangles
   triangles.emplace_back(0, 2, 3);
   triangles.emplace_back(0, 1, 2);
   triangles.emplace_back(0, 3, 1);
   triangles.emplace_back(3, 2, 1);

   for (unsigned int currentDivision = 0; currentDivision < subdivisions; ++currentDivision)
   {
      // TODO create list of pointers to iterate from.
      // Subdivide removes triangles, and thus the indexes are skewed.
      const unsigned int triangleCount = triangles.size();
      for (unsigned int i=0; i<triangleCount; ++i)
      {
         list<Triangle>::iterator itTriangle = triangles.begin();
         itTriangle->Subdivide(points, textureCoordinates, triangles, itTriangle);
      }
   }
}

void GlRenderSphere::setupVertexBufferObject()
{
   const vector<float> verticesColorsTexcoords = CreateVertexBufferData();

   glGenBuffers(1, &vertexBufferObject);
   glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
   glBufferData(GL_ARRAY_BUFFER, verticesColorsTexcoords.size() * sizeof(float), verticesColorsTexcoords.data(), GL_STATIC_DRAW);
}

void GlRenderSphere::setupElementBufferObject()
{
   const vector<GLuint> indices = CreateIndexData();

   glGenBuffers(1, &elementBufferObject);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
}

vector<float> GlRenderSphere::CreateVertexBufferData() const
{
   vector<float> data;

   for (unsigned int i=0; i < points.size(); ++i)
   {
      const auto& point = points[i];
      const auto& texCoord = textureCoordinates[i];

      // Point data
      data.push_back(point.X());
      data.push_back(point.Y());
      data.push_back(point.Z());

      // Color data
      data.push_back(1.f);
      data.push_back(0.5f);
      data.push_back(0.1f);

      // Texture coord data
      data.push_back(texCoord.u);
      data.push_back(texCoord.v);
   }
   return data;
}

std::vector<GLuint> GlRenderSphere::CreateIndexData() const
{
   vector<GLuint> data;
   for (const auto& triangle : triangles)
   {
      data.push_back(triangle.GetP1());
      data.push_back(triangle.GetP2());
      data.push_back(triangle.GetP3());
   }
   return data;
}
