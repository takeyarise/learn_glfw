#ifndef OBJ_LOADER_HPP
#define OBJ_LOADER_HPP

#include <string>
#include <vector>
#include <array>
#include <deque>
#include <sstream>
#include <unordered_map>

#include <cmath>
#include <limits>

/* todo
+ loadObj関数のテスト(vertexのみ)
+ どこかのstd::arrayでオーバーフローが起こっているので直すこと
+ Meshファイルの見直し
+ loadMtl関数の作成
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
	bool loadMtl(const std::string& file) {return true;}

	std::deque<std::array<float, 3>> vertices_;
	std::deque<std::array<float, 3>> normals_;
	std::deque<std::array<float, 2>> texCoords_;

	// openGL vbo index
	std::unordered_map<VertexPack, unsigned int> indexTable_;
	std::deque<VertexPack> VBO_;
	std::deque<unsigned int> VBOIndex_;
	std::deque<ColorPack> VBOColor_;
	unsigned int numVertices_;
};

#endif /* end of include guard: OBJ_LOADER_HPP */
