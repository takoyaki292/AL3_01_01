#pragma once
#include <stdio.h>
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Model.h"

///<summary>
///自キャラ
///</summary>
class Player 
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Player();
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Player();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model"></param>
	/// <param name="textureHandele"></param>
	/// <param name="viewProjection"></param>
	void Initialize(Model* model, uint32_t textureHandele, ViewProjection* viewProjection);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private:
	//ワールドトランスフォーム
	WorldTransform worldTransform_;

	//モデル
	Model* model_=nullptr;

	uint32_t textHandle_ = 0u;

	ViewProjection* viewProjection_ = nullptr;
};
