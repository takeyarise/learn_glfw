#pragma once

#include <string>
#include <vector>
#include <array>
#include <deque>
#include <sstream>
#include <unordered_map>

#include <cmath>
#include <limits>

/* todo
+ loadMtl関数の作成
+ hash関数のアルゴリズムを変える(案: xxhash)
*/

class Mesh;

struct ColorPack
{
	ColorPack() : startIndex(0), count(0) {}
	std::string name;
	unsigned int startIndex;
	unsigned int count;
};

struct VertexPack
{
	VertexPack() : vertex({}), normal({}), texCoord({}) {}
	// bool operator==(const VertexPack& other);
	std::array<float, 3> vertex;
	std::array<float, 3> normal;
	std::array<float, 2> texCoord;
};

struct ObjMtl
{
	ObjMtl() : specularExponent(0.0f), alpha(0.0f), illum(0) {}
	std::vector<float> ambient;
	std::vector<float> diffuse;
	std::vector<float> specular;
	float specularExponent;
	float alpha;
	int illum;
	std::string ambientTextureName;
	std::string diffuseTextureName;
	std::string specularTextureName;
	std::string specularHighLightTextureName;
	std::string alphaTextureName;
	std::string bumpTextureName;
	std::string displacementMapTextureName;
	std::string stencilDecalTextureName;
};

namespace std
{
	template<>
	struct hash<VertexPack>
	{
		std::size_t operator () (VertexPack const & key) const
		{
			size_t hash = 14695981039346656037U;
			for (int i = 0; i < 3; i++) {
				hash = (1099511628211LLU * hash) ^ static_cast<size_t>(key.vertex.at(i) * (1 << 6));
				hash = (1099511628211LLU * hash) ^ static_cast<size_t>(key.normal.at(i) * (1 << 8));
				if (i < 2) {
					hash = (1099511628211LLU * hash) ^ static_cast<size_t>(key.texCoord.at(i) * (1 << 4));
				}
			}
			return hash;
		}
	};

	template<>
	struct equal_to<VertexPack>
	{
  	bool operator()(const VertexPack &a, const VertexPack &b) const
		{
    	// return a == b;
			return
			std::abs(a.vertex.at(0) - b.vertex.at(0)) <= std::numeric_limits<float>::epsilon() &&
			std::abs(a.vertex.at(1) - b.vertex.at(1)) <= std::numeric_limits<float>::epsilon() &&
			std::abs(a.vertex.at(2) - b.vertex.at(2)) <= std::numeric_limits<float>::epsilon() &&
			std::abs(a.normal.at(0) - b.normal.at(0)) <= std::numeric_limits<float>::epsilon() &&
			std::abs(a.normal.at(1) - b.normal.at(1)) <= std::numeric_limits<float>::epsilon() &&
			std::abs(a.normal.at(2) - b.normal.at(2)) <= std::numeric_limits<float>::epsilon() &&
			std::abs(a.texCoord.at(0) - b.texCoord.at(0)) <= std::numeric_limits<float>::epsilon() &&
			std::abs(a.texCoord.at(1) - b.texCoord.at(1)) <= std::numeric_limits<float>::epsilon();
  	}
	};
}

class ObjLoader
{
public:
	ObjLoader();
	~ObjLoader();

	bool load(const std::string& path, const std::string& objName, const std::string& mtlName);
	void createMesh(Mesh& mesh);
private:
	bool findSameVertex(VertexPack& pack, unsigned int& result);
	void convertVBOIndex(std::string& element);
	void faceParse(std::istringstream& input);
	bool loadObj(const std::string& file);
	bool loadMtl(const std::string& file);

	std::deque<std::array<float, 3>> vertices_;
	std::deque<std::array<float, 3>> normals_;
	std::deque<std::array<float, 2>> texCoords_;

	std::unordered_map<std::string, ObjMtl> mtlTable_;
	std::deque<ColorPack> VBOColor_;

	// openGL vbo index
	std::unordered_map<VertexPack, unsigned int> indexTable_;
	std::deque<VertexPack> VBO_;
	std::deque<unsigned int> VBOIndex_;
	unsigned int numVertices_;
};

