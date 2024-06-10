#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugCamera.h"
#include "Player.h"
#include <vector>

/// <summary>
/// プレイヤーを呼ぶ
/// </summary>
class Player;

class  CameraController {
public:
	 CameraController();
	~ CameraController();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(ViewProjection* viewProjection);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	
	/// <summary>
	/// 追従対象のポインタの関数
	/// </summary>
	/// <param name="target"></param>
	void SetTarget(Player* target){ target_ = target; };

	
	/// <summary>
	/// 瞬間合わせの関数
	/// </summary>
	void Reset();

	struct Rest {
		float left = 0.0f;
		float right = 1.0f;
		float bottom = 0.0f;
		float top = 1.0f;
	};

	Rest movableArea_ = {0, 100, 0, 100};

	void SetMovebleArea(Rest area) { movableArea_ = area;};

	Vector3 Lerp(const Vector3& a, const Vector3& b, float t);

	
	//Rest setter();
	static inline const float kInterpolationRate = 0.1f;
	//速度の掛け算
	static inline const float kVelocityBias = 2;
	float targetVelocity =1;

	static inline const Rest margin = {0, 0, 0, 0};
	private:
	ViewProjection* viewProjection_;
	Player* target_ = nullptr;
	Vector3 targetOffset_ = {0, 0, -30.0f};
	Vector3 targetCoordinates = {};
	//float target
	//WorldTransform targetWorldTransform_;
	//DebugCamera* debugCamera_;
	//CameraController cameraController_=NULL;

};

 