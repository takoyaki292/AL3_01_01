#include "moveEnemy.h"
#include "WorldTransform.h"
//#include "GameScene.h"
//class GameScene;
void MoveEnemy::Initalize(
    Model* model, ViewProjection* viewProjection, const Vector3& position, Player* player) {
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	viewProjection_ = viewProjection;
	enemyModel_ = model;
	player_ = player;
	//gameScene_->Initialize();
	
}

void MoveEnemy::Update() {
	//player_ = gameScene_->GetPlayer();
	if (player_) {
		
		// フレームごとにプレイヤーの位置や向きを更新
		Vector3 playerPosition = player_->GetWorldTransform().translation_;
		Vector3 playerDirection = player_->GetWorldTransform().rotation_;

		// 敵の位置や方向ベクトルの更新
		Vector3 enemyPosition = worldTransform_.translation_;
		Vector3 directionToPlayer = {
		    playerPosition.x - enemyPosition.x, playerPosition.y - enemyPosition.y,
		    playerPosition.z - enemyPosition.z};

		// プレイヤーが敵を見ているか判定するため、プレイヤーの向きと方向ベクトルの内積を計算
		float dotProduct = Dot(playerDirection, directionToPlayer);
		// y軸方向の距離を確認
		float yDistance = playerPosition.y - enemyPosition.y;

		// yの距離がある程度以下の場合、敵を動かす
		if (dotProduct <= 0.0f || fabs(yDistance) < 0.1f) {
			Vector3 directionNormalized = Normalize(directionToPlayer);
			worldTransform_.translation_.x += directionNormalized.x * enemySpeed_;
			worldTransform_.translation_.y += directionNormalized.y * enemySpeed_;
			worldTransform_.translation_.z += directionNormalized.z * enemySpeed_;
		}

		// 行列の更新
		worldTransform_.UpdateMatrix();
		

		
	}
}

void MoveEnemy::Draw() { 
	enemyModel_->Draw(worldTransform_, *viewProjection_);
}

Vector3 MoveEnemy::GetWorldPosition() { 
	Vector3 worldPos;
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;
	return worldPos;
}

AABB MoveEnemy::GetAABB() { return AABB(); }