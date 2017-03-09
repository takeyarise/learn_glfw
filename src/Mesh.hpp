#pragma once

#include <array>
#include <map>
#include <string>
#include <vector>

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
