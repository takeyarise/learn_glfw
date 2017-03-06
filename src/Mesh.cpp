#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "Debug.hpp"
#include "Mesh.hpp"
#include "loader.hpp"

Mesh::Mesh() : numVertices_(0), numFaces_(0)
{
}

Mesh::~Mesh()
{
}

void Mesh::setVerticesNum(unsigned int num)
{
  numVertices_ = num;
}

void Mesh::setFacesNum(unsigned int num)
{
  numFaces_ = num;
}

unsigned int Mesh::getVerticesNum() const
{
  return numVertices_;
}

unsigned int Mesh::getFacesNum() const
{
  return numFaces_;
}
