#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <array>
#include <unordered_map>
#include <deque>
#include <algorithm>

#include <cmath>
#include <limits>

#include "ObjLoader.hpp"
#include "loader.hpp"
#include "Mesh.hpp"

ObjLoader::ObjLoader()
	: numVertices_(0)
{
}

ObjLoader::~ObjLoader()
{
}

bool ObjLoader::load(const std::string &path, const std::string &objName, const std::string &mtlName)
{
	if (!loadObj(path + objName)) {
		return false;
	}
	if (mtlName.empty()) {
		return true;
	}
	if (!loadMtl(path + mtlName)) {
		return false;
	}
	return true;
}

void ObjLoader::createMesh(Mesh &mesh)
{
	mesh.vertices.clear();
	mesh.texCoords.clear();
	mesh.normals.clear();
	mesh.indices.clear();
	mesh.vertices.reserve(VBO_.size() * 3);
	bool isTex = !(texCoords_.empty());
	bool isNor = !(normals_.empty());
	if (isTex) {
		mesh.texCoords.reserve(VBO_.size() * 2);
	}
	if (isNor) {
		mesh.normals.reserve(VBO_.size() * 3);
	}

	for (auto& vp : VBO_) {
		mesh.vertices.push_back(vp.vertex.at(0));
		mesh.vertices.push_back(vp.vertex.at(1));
		mesh.vertices.push_back(vp.vertex.at(2));
		if (isTex) {
			mesh.texCoords.push_back(vp.texCoord.at(0));
			mesh.texCoords.push_back(vp.texCoord.at(1));
		}
		if (isNor) {
			mesh.normals.push_back(vp.normal.at(0));
			mesh.normals.push_back(vp.normal.at(1));
			mesh.normals.push_back(vp.normal.at(2));
		}
	}

	mesh.indices.reserve(VBOIndex_.size());
	for (auto& i : VBOIndex_) {
		mesh.indices.push_back(i);
	}

	mesh.setVerticesNum(numVertices_);
}

bool ObjLoader::findSameVertex(VertexPack& vp, unsigned int& result)
{
	std::unordered_map<VertexPack, unsigned int>::iterator it = indexTable_.find(vp);
	if (it == indexTable_.end()) {
		return false;
	} else {
		result = it->second;
		return true;
	}
}

void ObjLoader::convertVBOIndex(std::string &element)
{
	VertexPack vp;
	std::replace(element.begin(), element.end(), '/', ' ');
	std::istringstream iss(element);
	int vi;
	iss >> vi;
	if (vi < 0) {
		vi += vertices_.size();
	} else {
		vi--;
	}
	vp.vertex = vertices_.at(vi);
	if (!texCoords_.empty()) {
		int ti;
		iss >> ti;
		if (ti < 0) {
			ti += texCoords_.size();
		} else {
			ti--;
		}
		vp.texCoord = texCoords_.at(ti);
	}
	if (!normals_.empty()) {
		int ni;
		iss >> ni;
		if (ni < 0) {
			ni += normals_.size();
		} else {
			ni--;
		}
		vp.normal = normals_.at(ni);
	}

	unsigned int indexvbo;
	if (findSameVertex(vp, indexvbo)) {
		VBOIndex_.push_back(indexvbo);
	} else {
		VBO_.push_back(vp);
		indexvbo = VBO_.size() - 1;
		indexTable_.emplace(vp, indexvbo);
		VBOIndex_.push_back(indexvbo);
	}
}

// triangle or square only
void ObjLoader::faceParse(std::istringstream &input)
{
	std::array<std::string, 4> indexInfos;
	input >> indexInfos.at(0) >> indexInfos.at(1) >> indexInfos.at(2);
	if (!input.eof()) {
		input >> indexInfos.at(3);
	}
	for (unsigned int i = 0; i < indexInfos.size() - 1; i++) {
		convertVBOIndex(indexInfos.at(i));
	}
	if (indexInfos.at(3).empty()) {
		return;
	} else {
		convertVBOIndex(indexInfos.at(2));
		convertVBOIndex(indexInfos.at(3));
		convertVBOIndex(indexInfos.at(0));
	}
}

bool ObjLoader::loadObj(const std::string& file)
{
	std::ifstream ifs;
	ifs.open(file.c_str(), std::ios::in);
	if (!ifs.is_open()) {
		std::cerr << "Fail to open the file: " << file << std::endl;
		return false;
	}

	std::string line;
	std::string buf;
	bool faceFlag = false;
	ColorPack cp;
	while (!safeGetLine(ifs, line).eof()) {
		std::istringstream input(line);

		input >> buf;
		if (buf.empty() || buf.at(0) == '#') {
			continue;
		} else if (buf == "vn") {
			float x, y, z;
			input >> x >> y >> z;
			normals_.push_back({x, y, z});
		} else if (buf == "vt") {
			float u, v;
			input >> u >> v;
			texCoords_.push_back({u, v});
		} else if (buf == "v") {
			if (faceFlag) {
				if (!cp.name.empty()) {
					cp.count = VBOIndex_.size() - cp.startIndex;
					VBOColor_.push_back(cp);
					cp.name.clear();
					cp.startIndex = VBOIndex_.size();
				}
				faceFlag = false;
			}
			float x, y, z;
			input >> x >> y >> z;
			vertices_.push_back({x, y, z});
			numVertices_++;
		} else if (buf == "f") {
			faceParse(input);
			faceFlag = true;
		} else if (buf == "usemtl") {
			std::string mtl;
			cp.name = mtl;
		}

		buf.clear();
	}

	return true;
}

// sample
bool ObjLoader::loadMtl(const std::string& file)
{
	std::ifstream ifs;
	ifs.open(file.c_str(), std::ios::in);
	if (!ifs.is_open()) {
		std::cerr << "Fail to open the file: " << file << std::endl;
		return false;
	}

	std::string line;
	std::string buf;
	std::string mtlname;
	while (!safeGetLine(ifs, line).eof()) {
		std::istringstream iss(line);
		iss >> buf;

		if (buf == "newmtl") {
			mtlname.clear();
			iss >> mtlname;
		} else if (buf == "Ka") {
			float x, y, z;
			iss >> x >> y >> z;
			mtlTable_[mtlname].ambient.push_back(x);
			mtlTable_[mtlname].ambient.push_back(y);
			mtlTable_[mtlname].ambient.push_back(z);
		} else if (buf == "Kd") {
			float x, y, z;
			iss >> x >> y >> z;
			mtlTable_[mtlname].diffuse.push_back(x);
			mtlTable_[mtlname].diffuse.push_back(y);
			mtlTable_[mtlname].diffuse.push_back(z);
		} else if (buf == "Ks") {
			float x, y, z;
			iss >> x >> y >> z;
			mtlTable_[mtlname].specular.push_back(x);
			mtlTable_[mtlname].specular.push_back(y);
			mtlTable_[mtlname].specular.push_back(z);
		} else if (buf == "Ns") {
			// float e;
			// iss >> e;
			// mtlTable_[mtlname].specularExponent = e;
			iss >> mtlTable_[mtlname].specularExponent;
		} else if (buf == "d") {
			// float a;
			// iss >> a;
			// mtlTable_[mtlname].alpha = a;
			iss >> mtlTable_[mtlname].alpha;
		} else if (buf == "Tr") {
			float a;
			iss >> a;
			mtlTable_[mtlname].alpha = 1.0f - a;
		} else if (buf == "illum") {
			// int i;
			// iss >> i;
			// mtlTable_[mtlname].illum = i;
			iss >> mtlTable_[mtlname].illum;
		} else if (buf == "map_Ka") {
			iss >> mtlTable_[mtlname].ambientTextureName;
		} else if (buf == "map_Kd") {
			iss >> mtlTable_[mtlname].diffuseTextureName;
		} else if (buf == "map_Ks") {
			iss >> mtlTable_[mtlname].specularTextureName;
		} else if (buf == "map_Ns") {
			iss >> mtlTable_[mtlname].specularHighLightTextureName;
		} else if (buf == "map_d") {
			iss >> mtlTable_[mtlname].alphaTextureName;
		} else if (buf == "map_bump" || buf == "bump") {
			iss >> mtlTable_[mtlname].bumpTextureName;
		} else if (buf == "disp") {
			iss >> mtlTable_[mtlname].displacementMapTextureName;
		} else if (buf == "decal") {
			iss >> mtlTable_[mtlname].stencilDecalTextureName;
		}
	}

	return true;
}
