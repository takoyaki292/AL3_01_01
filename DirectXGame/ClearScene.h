#pragma once
#include "Audio.h"
#include "CameraController.h"
#include "DeathParticles.h"
#include "DebugCamera.h"
#include "DirectXCommon.h"
#include "Enemy.h"
#include "Input.h"
#include "MapChipField.h"
#include "Model.h"
#include "ClearModel.h"
#include "titleModel.h"
#include "Player.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <vector>

// #include "MapChipField.h"
class ClearScene {
public:
	ClearScene();
	~ClearScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initalize();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	bool IsFinished() const { return finished_; };

private:
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	// 終了フラグ
	bool finished_ = false;

	ViewProjection viewProjection_ = {};
	TitleModel* clearModel_ = nullptr;
	Model* model = nullptr;

	// uint32_t tH = 0;
	// Sprite* sprite_ = nullptr;
};
