#include "MapChipField.h"
#include<map>
#include<string>
#include<vector>
#include <cassert>
#include <fstream>
#include <sstream>


namespace{
	std::map<std::string, MapChipType> mapChipTable = {
		{"0", MapChipType::kBlank},
		{"1", MapChipType::kBlock},
	};
}



void MapChipField::ResetMapChipData() {
	mapChipData_.data.clear();
	mapChipData_.data.resize(kNumBlockVirtical);
	for (std::vector<MapChipType>& mapChipDataline : mapChipData_.data) {
		mapChipDataline.resize(kNumBlockHorizontal);
	}

	
}

void MapChipField::LoadMapChipCsv(const std::string& filePath) {
	ResetMapChipData();

	std::ifstream file;
	file.open(filePath);
	assert(file.is_open());

	std::stringstream mapChioCsv;
	mapChioCsv << file.rdbuf();

	file.close();

	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
		std::string line;
		getline(mapChioCsv, line);


		std::istringstream line_stream(line);
		for (uint32_t j = 0; j < kNumBlockHorizontal; ++j) {
			std::string word;
			getline(line_stream, word, ',');

			if (mapChipTable.contains(word)) {
				mapChipData_.data[i][j] = mapChipTable[word];
			}
		}
	}
}

MapChipType MapChipField::GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex) {
	if (xIndex < 0 || kNumBlockHorizontal - 1 < xIndex) {
		return MapChipType::kBlank;
	}
	if (yIndex < 0 || kNumBlockVirtical - 1 < yIndex) {
		return MapChipType::kBlank;
	}
	return mapChipData_.data[yIndex][xIndex];
}

Vector3 MapChipField::GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex) {

	return Vector3(kBlockWidth * xIndex, kBlockHeight * (kNumBlockVirtical - 1 - yIndex), 0);
}



IndexSet MapChipField::GetMapChipIndexSetByPosition(const Vector3& position) {
	IndexSet indexSet = {};
	IndexSet tempIndex = {};
	indexSet.xIndex = ((uint32_t)position.x + (uint32_t)kBlockWidth / 2) / (uint32_t)kBlockWidth;
	tempIndex.yIndex = ((uint32_t)position.y + (uint32_t)kBlockHeight / 2) / (uint32_t)kBlockHeight;
	indexSet.yIndex =
	    (kNumBlockVirtical - tempIndex.yIndex) - (uint32_t)kBlockHeight;
	
	return indexSet;
}

Rect MapChipField::GetRectByIndex(uint32_t xIndex, uint32_t yIndex) {
	Vector3 center = GetMapChipPositionByIndex(xIndex, yIndex);

	Rect rect;
	rect.left = center.x - kBlockWidth/2.0f;
	rect.right = center.x + kBlockWidth / 2.0f;
	rect.bottom = center.y - kBlockHeight / 2.0f;
	rect.top = center.y + kBlockHeight / 2.0f;

	return rect; 
}

Vector3 MapChipField::GetMapChipPositionByPlayerIndex(uint32_t xIndex, uint32_t yIndex) {

	Vector3 a = {(kBlockWidth * xIndex, kBlockHeight * (kNumBlockVirtical - 1 - yIndex), 0)};
	return a;
}
uint32_t MapChipField::GetNumBlockVirtical() { return kNumBlockVirtical; }
uint32_t MapChipField::GetNumBlockHorizontal() { return kNumBlockHorizontal; }
uint32_t MapChipField::GetNumPlayerHorizontal() { return kNumPlayerHeight; }


