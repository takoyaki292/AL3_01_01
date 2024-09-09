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
#include "moveEnemy.h"
#include <cmath>
#include "moveEnemy.h"
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
	
	// 内積の関数
	float Dot(const Vector3& v1, const Vector3& v2) {
		float a = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
		return a;
	}

	// 2点間の距離を計算
	float Distance(const Vector3& a, const Vector3& b) {
		return sqrt((float)pow(b.x - a.x, 2) + (float)pow(b.y - a.y, 2) + (float)pow(b.z - a.z, 2));
	}

	// ベクトルの正規化（方向ベクトルを単位ベクトルにする）
	Vector3 Normalize(const Vector3& vec) {
		float length = sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
		return {vec.x / length, vec.y / length, vec.z / length};
	}
	Vector3 trackingSpeed(Vector3& enemy, Vector3& player, float enemySpeed);
	Vector3 A(Vector3& enemy, Vector3& player, Vector3 enemyDirection);

	//Player GetPlayer() const;

	//void GetPlayer(Player* player);

	Player* GetPlayer();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	// モデル
	Model* modelBlock_ = nullptr;
	Model* modelPlayer_ = nullptr;
	Model* modelEnemy_ = nullptr;
	Model* modelMoveEnemy_ = nullptr;
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
	//std::list<Enemy*> enemies_;
	std::list<MoveEnemy*> moveEnemies_;
	
	//DeathParticles* deathParticle_ = nullptr;
	//bool isDeachPaticled = true;

	//Vector3 enemyDirection = {};
	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
