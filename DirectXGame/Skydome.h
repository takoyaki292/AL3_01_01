#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Model.h"

class Skydome
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model, ViewProjection* viewProjection);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private:
	WorldTransform worldTransform_;
	ViewProjection* viewProjection_=nullptr;
	//uint32_t textureHandle_ = 0u;

	Model* model_ = nullptr;
};