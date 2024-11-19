#pragma once
#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "CameraController.h"
#include "DeathParticles.h"
#include "DebugCamera.h"
#include "Enemy.h"
#include "MapChipField.h"
#include "Player.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "titleModel.h"
#include <vector>

enum class TitleOption { kStartGame, kExplanation };
// #include "MapChipField.h"
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
	bool IsEFinished() const { return eFinished_; };
	TitleOption GetSelectedOption() const;

private:
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	// 終了フラグ
	bool finished_ = false;
	bool eFinished_ = false;

	ViewProjection viewProjection_ = {};
	TitleModel* titleModel_ = nullptr;
	TitleModel* setumeiModel_ = nullptr;
	Model* model=nullptr;
	Model* titleGameModel=nullptr;

	TitleOption selectedOption_ = TitleOption::kStartGame; 
	//uint32_t tH = 0;
	//Sprite* sprite_ = nullptr;
};