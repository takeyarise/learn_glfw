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
	unsigned int getSubSetNum() const;
	unsigned int getIndicesNum() const;

	std::vector<float> vertices;
	std::vector<float> normals;
	std::vector<float> texCoord;
	std::vector<unsigned int> vertexIndex;
	std::vector<unsigned int> normalIndex;
	std::vector<unsigned int> texCoordIndex;
private:
	bool loadMtl(const std::string& path);
	bool loadObj(const std::string& path);

	void vertexParce(std::istringstream& input, const std::string& buf);
	void faceParce(std::istringstream& input, const std::string& buf);

	unsigned int numVertices_;
	unsigned int numSubSets_;
	//unsigned int numMaterials_;
	unsigned int numIndices_;
	//OBJBOUNDINGBOX box_;
	//OBJBOUNDINGSPHERE sphere_;
	std::string filePath_;
};
