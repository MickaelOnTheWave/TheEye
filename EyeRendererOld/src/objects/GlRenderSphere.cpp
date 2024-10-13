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

void GlRenderSphere::PrepareRendering(const unsigned int shaderProgramId)
{
   glUniform1i(glGetUniformLocation(shaderProgramId, "textureCount"), textureFiles.size());

   for (unsigned int i=0; i<textureFiles.size(); ++i)
   {
      glActiveTexture(GL_TEXTURE0 + i);
      glBindTexture(GL_TEXTURE_2D, textureObjects[i]);
   }

   glBindVertexArray(vertexArrayObject);
}

void GlRenderSphere::Render()
{
   const unsigned int vertexPerFace = 3;
   const unsigned int faceCount = 4;
   const unsigned int indexCount = vertexPerFace * faceCount;
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
   const vector<float> verticesColorsTexcoords = CreateVertexBufferData();

   glGenBuffers(1, &vertexBufferObject);
   glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
   glBufferData(GL_ARRAY_BUFFER, verticesColorsTexcoords.size() * sizeof(float), verticesColorsTexcoords.data(), GL_STATIC_DRAW);
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

void GlRenderSphere::setupTextureObject(const TextureVec& _textureFiles)
{
   textureFiles = _textureFiles;

   glGenTextures(textureFiles.size(), textureObjects);
   for (int i=0; i<textureFiles.size(); ++i)
   {
      const auto& textureFile = textureFiles[i];
      generateTextureObject(textureFile.first.c_str(), i, textureFile.second);
   }
}

void GlRenderSphere::setupVertexArrayAttributes()
{
   const GLsizei stride = 8 * sizeof(float);

   // Position
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
   glEnableVertexAttribArray(0);

   // Color
   glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
   glEnableVertexAttribArray(1);

   // Texture coordinates
   glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));
   glEnableVertexAttribArray(2);
}

vector<float> GlRenderSphere::CreateVertexBufferData() const
{
   vector<float> data;

   for (const auto& point : points)
   {
      // Point data
      data.push_back(point.X());
      data.push_back(point.Y());
      data.push_back(point.Z());

      // Color data
      data.push_back(1.f);
      data.push_back(0.5f);
      data.push_back(0.1f);

      // Texture coord data
      data.push_back(1.f);
      data.push_back(0.f);
   }
   return data;
}
