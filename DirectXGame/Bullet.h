#pragma once
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

class Player;
#ifndef STRUCT_H
#define STRUCT_H
struct AABB {
	Vector3 min;
	Vector3 max;
};
#endif
class Bullet {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initalize(
	    Model* model, ViewProjection* viewProjection, const Vector3& position, float lifetime,
	    float speed, const Vector3& direction);

	/// <summary>
	/// 処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	bool IsOutOfBounds() const; // 画面外に出たかどうかをチェックするメソッド
	void OnCollisiton(const Player* player);

	AABB GetAABB();

	Vector3 GetWorldPosition();

	void Reset();

	void SetVelocity(const Vector3& velocity) { velocity_ = velocity; }

private:
	WorldTransform worldTransform_;
	// WorldTransform bulletWorldTransform_;
	ViewProjection* viewProjection_;
	Model* bulletModel_ = nullptr;
	Vector3 velocity_ = {};
	float lifetime_; // 弾のライフタイム（消えるまでの時間）
	float speed_;
	// キャラクターの当たり判定のサイズ
	static inline const float kWidth = 2.0f;
	static inline const float kHeight = 2.0f;
	//MapChipField* mapChipField_;
	Vector3 initialPosition_;
	bool isBullet_;
	Vector3 direction_;
};
