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
#include "Bullet.h"
#include "MoveEnemy.h"
#include "Skydome.h"
#include "DeathParticles.h"
#include <DeathParticles.h>

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

	bool IsFinished() const { return finished_; };

	
	bool IsPlayerAlive() const {
		// プレイヤーオブジェクトが存在し、かつ生存しているかどうかをチェック
		return player_ != nullptr && player_->IsAlive();
	}
	bool IsTimeOver() const {
		return player_->IsTimeOver(); // プレイヤーの時間切れ状態を返す
	}

	void DrawTimeBar();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	// モデル
	Model* modelBlock_ = nullptr;
	Model* modelTBlock_ = nullptr;
	Model* modelPlayer_ = nullptr;
	Model* modelEnemy_ = nullptr;
	Model* modelMoveEnemy_ = nullptr;
	Model* modelEnemyBullet_ = nullptr;
	Model* modelDeathParticles_ = nullptr;
	Model* modelSkydome_ = nullptr;

	// 天球
	Skydome* skydome_ = nullptr;

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
	std::list<MoveEnemy*> moveEnemies_;
	std::list<Bullet*> bullets_;
	
	DeathParticles* deathParticle_ = nullptr;
	//bool isDeachPaticled = true;

	//MapChipType mapChipType;
	MapChipType currentChipType = MapChipType::kBlock; 
	MapChipType currentMapChipType = MapChipType::kMoveBlock; 
	int isB = 0;

	float lifetime = 0.0f;
	float bulletSpeed = 0.0f;
	bool finished_ = false;

	//TextureManager
	bool isA = true;
	//uint32_t soundDateHandle_ = 0;
	//uint32_t voiceHandle_ = 0;
	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
