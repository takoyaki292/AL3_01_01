#include "moveEnemy.h"
#include "WorldTransform.h"
//class GameScene;
void MoveEnemy::Initalize(Model* model, ViewProjection* viewProjection, const Vector3& position) {
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	viewProjection_ = viewProjection;
	enemyModel_ = model;

}

void MoveEnemy::Update() {
	if (player_) {
		// プレイヤーの位置と向きを取得
		Vector3 playerPosition = player_->GetWorldTransform().translation_;
		Vector3 playerDirection = player_->GetWorldTransform().rotation_; // プレイヤーの向きを取得

		// 敵の位置と向きを取得
		Vector3 enemyPosition = worldTransform_.translation_;

		Vector3 directionToPlayer = {
		    playerPosition.x - enemyPosition.x, playerPosition.y - enemyPosition.y,
		    playerPosition.z - enemyPosition.z};

		// 正規化
		float distanceToPlayer = sqrt(
		    directionToPlayer.x * directionToPlayer.x + directionToPlayer.y * directionToPlayer.y +
		    directionToPlayer.z * directionToPlayer.z);

		if (distanceToPlayer > 0.0f) {
			// 内積を計算
			float dotProduct = Dot(playerDirection, directionToPlayer);

			// 内積が0以下なら
			if (dotProduct <= 0.0f) {
				//移動
				Vector3 directionNormalized = Normalize(directionToPlayer);
				worldTransform_.translation_.x += directionNormalized.x * enemySpeed_;
				worldTransform_.translation_.y += directionNormalized.y * enemySpeed_;
				worldTransform_.translation_.z += directionNormalized.z * enemySpeed_;
			}
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