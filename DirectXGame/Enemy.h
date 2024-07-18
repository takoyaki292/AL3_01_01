#pragma once
#include"Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

class MapChipField;
class Player;
#ifndef STRUCT_H
#define STRUCT_H
struct AABB {
	Vector3 min;
	Vector3 max;
};
#endif
class Enemy {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initalize(Model* model, ViewProjection* viewProjection, const Vector3& position);

	/// <summary>
	/// 処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 外部からポインタを呼ぶ
	/// </summary>
	void SetMapChipField(MapChipField* mapChipField);

	/// <summary>
	/// 自キャラのワールド座標
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldPosition();

	/// <summary>
	/// AABB取得関数
	/// </summary>
	/// <returns></returns>
	AABB GetAABB();
	/// <summary>
	/// 衝突判定
	/// </summary>
	/// <param name="player"></param>
	void OnCollisiton(const Player* player);

	

private:
	WorldTransform worldTransform_;
	ViewProjection* viewProjection_ = {};
	Model* enemyModel_=nullptr;

	MapChipField* mapChipField_ = nullptr;
	//敵の歩き速度
	static inline const float kWalkSpeed = 0.05f;
	//移動量
	Vector3 velocity_ = {};

	//アニメーションの最初の角度
	static inline const float kWalkMotionAngleStart = 0.0f;
	// アニメーションの最後の角度
	static inline const float kWalkMotionAngleEnd = -10.0f;
	//アニメーションする時間
	static inline const float kWalkMotionTime = 0.5f;
	//経過時間
	float walkTimer_ = 0.0f;
	//π
	const double PI = 3.141593;

	// キャラクターの当たり判定のサイズ
	static inline const float kWidth = 1.6f;
	static inline const float kHeight = 1.6f;
};