#include "GlRenderSphere.h"

using namespace std;

void GlRenderSphere::Initialize(const unsigned int subdivisions,
                                          const TextureVec &_textureFiles)
{
   SetTextures(_textureFiles);
   PopulateGeometry(subdivisions);
   InitializeGlData();
}

vector<GLfloat> GlRenderSphere::CreateVertexBufferData() const
{
   vector<float> data;

   for (unsigned int i=0; i < points.size(); ++i)
   {
      const auto& point = points[i];
      const auto& color = colors[i];
      const auto& texCoord = textureCoordinates[i];

      // Point data
      data.push_back(point.X());
      data.push_back(point.Y());
      data.push_back(point.Z());

      // Color data
      data.push_back(1.f);
      data.push_back(1.f);
      data.push_back(1.f);

      /*data.push_back(color.X());
      data.push_back(color.Y());
      data.push_back(color.Z());*/

      // Texture coord data
      data.push_back(texCoord.u);
      data.push_back(texCoord.v);
   }
   return data;
}
