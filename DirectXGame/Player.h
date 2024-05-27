#pragma once
#include"Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Model.h"
class Player
{
	public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model"></param>
	/// <param name="viewProjection"></param>
	/// <param name="position"></param>
	void Initalize(Model* model,ViewProjection* viewProjection,const Vector3&position);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	private:
	WorldTransform worldTransform_;
	ViewProjection* viewProjection_;

	Vector3 velocity_ = {};
	static inline const float kAceeleration = 3;
	Model* playerModel_ ;
};
