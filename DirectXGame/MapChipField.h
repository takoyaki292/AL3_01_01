#pragma once
#include <cstdint>
#include <vector>
#include <string>
#include "Vector3.h"

enum  class MapChipType 
{
	kBlank,
	kBlock
};

struct  MapChipData 
{
	std::vector<std::vector<MapChipType>> data;
};

struct IndexSet {
	uint32_t xIndex;
	uint32_t yIndex;
};

struct Rect {
	float left;
	float right;
	float bottom;
	float top;
};
class MapChipField 
{
public:
	void ResetMapChipData();

	void LoadMapChipCsv(const std::string& filePath);

	uint32_t GetNumBlockVirtical();

	uint32_t GetNumBlockHorizontal();

	uint32_t GetNumPlayerHorizontal();

	MapChipType GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex);

	Vector3 GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex);
	Vector3 GetMapChipPositionByPlayerIndex(uint32_t xIndex, uint32_t yIndex);

	/// <summary>
	/// 座標からマップチップ番号を取得
	/// </summary>
	IndexSet GetMapChipIndexSetByPosition(const Vector3& position);
	
	/// <summary>
	/// ブロックの範囲取得
	/// </summary>
	Rect GetRectByIndex(uint32_t xIndex, uint32_t yIndex);

	static inline const float kBlockWidth = 2.0f;
	static inline const float kBlockHeight = 2.0f;
private:

	static inline const uint32_t kNumBlockVirtical = 30;
	static inline const uint32_t kNumBlockHorizontal = 100;
	static inline const uint32_t kNumPlayerHeight = 100;

	MapChipData mapChipData_;

	
};
