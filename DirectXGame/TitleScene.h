#pragma once
#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"		
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "MapChipField.h"
#include <vector>
#include "DebugCamera.h"
#include "Player.h"
#include "CameraController.h"
#include "Enemy.h"
#include "DeathParticles.h"
#include "titleModel.h"
//#include "MapChipField.h"
class TitleScene {
public:
	TitleScene();
	~TitleScene();

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
	//終了フラグ
	bool finished_ = false;

	ViewProjection viewProjection_ = {};
	TitleModel* titleModel_ = nullptr;
	Model* model;
};