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
//#include "DeathParticles.h"

#ifndef STRUCT_H
#define STRUCT_H
struct AABB {
	Vector3 min;
	Vector3 max;
};
#endif
/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	void GenerateBlocks();

	/// <summary>
	/// 全ての当たり判定を行う
	/// </summary>
	void CheckAllCollisios();

	
	bool IsCollision(AABB a,AABB b);
	
	int switching(MapChipType mapChipNumber);

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	// モデル
	Model* modelBlock_ = nullptr;
	Model* modelTBlock_ = nullptr;
	Model* modelPlayer_ = nullptr;
	Model* modelEnemy_ = nullptr;
	Model* modelDeathParticles_ = nullptr;
	

	bool isDebugCameraActive_ = false;
	DebugCamera* debugCamera_ = nullptr;
	// ブロック用のワールドトランスフォーム
	// std::vector<WorldTransform*> worldTransformBlocks_;
	//std::vector<std::vector<WorldTransform*>> worldTransformBlocks_;
	ViewProjection viewProjection_;
	WorldTransform playerWorldTransform_;
	MapChipField* mapChipField_;
	std::vector<std::vector<WorldTransform*>> worldTransformBlocks_;
	Player* player_ = nullptr;
	CameraController* cameraController_ = nullptr;
	
	//Enemy* enemy_ ;
	//敵の複数化
	std::list<Enemy*> enemies_;
	
	//DeathParticles* deathParticle_ = nullptr;
	//bool isDeachPaticled = true;

	//MapChipType mapChipType;
	MapChipType currentChipType = MapChipType::kBlock; 
	int isB = 0;
	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
