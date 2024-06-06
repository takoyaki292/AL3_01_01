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
	void Initalize(Model* model,ViewProjection* viewProjection,const Vector3& position);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	enum class LRDirection
	{
		kRight,
		kLeft,
	};

	WorldTransform& GetWorldTransform();

private:
	WorldTransform worldTransform_;
	ViewProjection* viewProjection_;
	LRDirection lrDirection_ = LRDirection::kRight;

	Vector3 velocity_ = {};
	static inline const float kAceeleration = 0.01f;
	Model* playerModel_ ;
	static inline const float kAttenuation = 0.1f;
	static inline const float kLimitRunSpeed = 0.2f;

	//旋回開始時の角度
	float turnFirstRotationY_ = 0.0f;
	//旋回タイマー
	float turnTimer_=0.0f;
	//旋回時間
	static inline const float kTimeTurn = 0.3f;

	bool onGround_ = true;

	static inline const float kGravityAcceleration = 1.0f;
	static inline const float kLimitFallSpeed = 2.0f;
	static inline const float kJumpAcceleration = 4.0f;

	static inline const float kAttenuationLanding = 1.0f;
	//bool upKey_ = false;
};
