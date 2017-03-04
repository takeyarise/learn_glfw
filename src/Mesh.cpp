#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>

#include "Debug.hpp"
#include "Mesh.hpp"
#include "loader.hpp"


Mesh::Mesh()
	:numVertices_(0), numSubSets_(0), numIndices_(0)
{}

Mesh::~Mesh()
{}

bool Mesh::load(const std::string& path)
{
	if (!loadObj(path)) {
		return false;
	}

	filePath_ = path;

	return true;
}

unsigned int Mesh::getVerticesNum() const
{
	return numVertices_;
}

unsigned int Mesh::getSubSetNum() const
{
	return numSubSets_;
}

unsigned int Mesh::getIndicesNum() const
{
	return numIndices_;
}

bool Mesh::loadMtl(const std::string& path)
{
	return true;
}

bool Mesh::loadObj(const std::string& path)
{
	std::ifstream ifs;
	ifs.open(path.c_str(), std::ios::in);
	if (!ifs.is_open()) {
		std::cerr << "Fail to open the file: " << path << std::endl;
		return false;
	}

	std::string line;
	std::string buf;
	while (!safeGetLine(ifs, line).eof()) {
		std::istringstream input(line);

		input >> buf;
		if (buf.empty()) {
			continue;
		}
		switch (buf.at(0)) {
			case '#':
				break;
			case 'v':
				vertexParce(input, buf);
				numVertices_ += 1;
				break;
			case 'f':
				faceParce(input, buf);
				numIndices_ += 1;
				break;
			// usemtl 'name'
			//case 'u':
				//break;
			// mtllib 'name'
			//case 'm':
				//loadMtl(...);
				//break;
			default:
				break;
		}
		buf.clear();
	}
}

void Mesh::vertexParce(std::istringstream& input, const std::string& buf)
{
	if (buf.size() == 1) {
		float x, y, z;
		input >> x >> y >> z;
		vertices.push_back(x);
		vertices.push_back(y);
		vertices.push_back(z);
	} else if (buf.size() == 2) {
		if (buf.at(1) == 'n') {
			float x, y, z;
			input >> x >> y >> z;
			normals.push_back(x);
			normals.push_back(y);
			normals.push_back(z);
		} else if (buf.at(1) == 't') {
			float x, y;
			input >> x >> y;
			texCoord.push_back(x);
			texCoord.push_back(y);
		}
	}
}

// 三角形か四角形にのみ対応
void Mesh::faceParce(std::istringstream& input, const std::string& buf)
{
	//unsigned int indexV[4], indexT[4], indexN[4];
	std::vector<unsigned int> indexV, indexT, indexN;
	unsigned int count = 0;
	unsigned int index = 0;

	std::string element;
	input >> element;
	std::string tmp;
	for (int i = 0; i < element.size(); i++) {
		if (element.at(i) == '/') {
			if (index == 0) {
				indexV.push_back(std::stoi(tmp));
				index++;
			} else if (index == 1) {
				indexT.push_back(std::stoi(tmp));
				index++;
			}
			tmp.clear();
		} else {
			tmp += element.at(i);
		}
	}
	if (index == 0) {
		indexV.push_back(std::stoi(tmp));
	} else if (index == 2) {
		indexN.push_back(std::stoi(tmp));
	}
	count++;
	while (!input.eof()) {
		index = 0;
		tmp.clear();
		input >> element;
		for (int i = 0; i < element.size(); i++) {
			if (element.at(i) == '/') {
				if (index == 0) {
					indexV.push_back(std::stoi(tmp));
					index++;
				} else if (index == 1) {
					indexT.push_back(std::stoi(tmp));
					index++;
				}
				tmp.clear();
			} else {
				tmp += element.at(i);
			}
		}
		if (index == 0) {
			indexV.push_back(std::stoi(tmp));
		} else if (index == 2) {
			indexN.push_back(std::stoi(tmp));
		}
		count++;
		if (count > 3) {
			break;
		}
	}

	// add
	if (count == 3) {
		for (auto v : indexV) {
			vertexIndex.push_back(v - 1);
		}
		for (auto t : indexT) {
			texCoordIndex.push_back(t - 1);
		}
		for (auto n : indexN) {
			normalIndex.push_back(n - 1);
		}
	} else if (count == 4) {
		if (!indexV.empty()) {
			vertexIndex.push_back(indexV.at(0) - 1);
			vertexIndex.push_back(indexV.at(1) - 1);
			vertexIndex.push_back(indexV.at(2) - 1);
			vertexIndex.push_back(indexV.at(2) - 1);
			vertexIndex.push_back(indexV.at(3) - 1);
			vertexIndex.push_back(indexV.at(0) - 1);
		}
		if (!indexT.empty()) {
			texCoordIndex.push_back(indexT.at(0) - 1);
			texCoordIndex.push_back(indexT.at(1) - 1);
			texCoordIndex.push_back(indexT.at(2) - 1);
			texCoordIndex.push_back(indexT.at(2) - 1);
			texCoordIndex.push_back(indexT.at(3) - 1);
			texCoordIndex.push_back(indexT.at(0) - 1);
		}
		if (!indexN.empty()) {
			normalIndex.push_back(indexN.at(0) - 1);
			normalIndex.push_back(indexN.at(1) - 1);
			normalIndex.push_back(indexN.at(2) - 1);
			normalIndex.push_back(indexN.at(2) - 1);
			normalIndex.push_back(indexN.at(3) - 1);
			normalIndex.push_back(indexN.at(0) - 1);
		}
	}
}
