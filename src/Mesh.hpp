#pragma once

#include <vector>
#include <string>

/*
struct ObjSubSet
{
	unsigned int materialIndex;
	unsigned int faceStart;
	unsigned int faceCount;
};
*/

class Mesh
{
public:
	Mesh();
	~Mesh();

	bool load(const std::string& path);
	unsigned int getVerticesNum() const;
	unsigned int getIndicesNum() const;

	std::vector<float> vertices;
	std::vector<unsigned int> vertexIndex;
private:
	bool loadObj(const std::string& path);

	void vertexParce(std::istringstream& input, const std::string& buf);
	void faceParce(std::istringstream& input);

	unsigned int numVertices_;
	unsigned int numIndices_;
	std::string filePath_;
};
