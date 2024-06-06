#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugCamera.h"
#include "Player.h"

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
	void Initialize();
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
	void SetTarget(Player* target);

	/// <summary>
	/// 瞬間合わせの関数
	/// </summary>
	void Reset();

	typedef struct Rest {
		float left = 0.0f;
		float right = 1.0f;
		float bottom = 0.0f;
		float top = 1.0f;
	};

	Rest movableArea_ = {0, 100, 0, 100};

	void SetMovebleArea(Rest area);
	//Rest setter();
	private:
	Player* target_ = nullptr;
	Vector3 targetOffset_ = {0, 0, -15.0f};
	ViewProjection viewProjection_;
	WorldTransform targetWorldTransform_;
	//DebugCamera* debugCamera_;
	//CameraController cameraController_=NULL;

};

 