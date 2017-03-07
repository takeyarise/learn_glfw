#pragma once

#include <array>
#include <map>
#include <string>
#include <vector>

// struct ColorRGB
// {
//   ColorRGB() : r(1.0), g(1.0), b(1.0) {}
//   float r;
//   float g;
//   float b;
// };

// struct MtlData
// {
//   MtlData() : specularExponent(0), alpha(1.0) {}
//   std::string name;
//   ColorRGB ambient;
//   ColorRGB diffuse;
//   ColorRGB specular;
//   int specularExponent;
//   float alpha;
//   std::vector<std::string> textureNames;
// };

// class MeshColor
// {
// public:
//   MeshColor();
//   ~MeshColor();
//
// private:
//   key = index, value = vector.at(value)
//   std::map<unsigned int, int> table_;
//   std::vector<MtlData> mtlData_;
// };

class Mesh
{
public:
  Mesh();
  ~Mesh();

  void setVerticesNum(unsigned int num);
  void setFacesNum(unsigned int num);
  unsigned int getVerticesNum() const;
  unsigned int getFacesNum() const;

  std::vector<float> vertices;
  std::vector<float> normals;
  std::vector<float> texCoords;
  std::vector<unsigned int> indices;

private:
  unsigned int numVertices_;
  unsigned int numFaces_;
};
