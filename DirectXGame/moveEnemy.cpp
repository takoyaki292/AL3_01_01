#include "moveEnemy.h"

void MoveEnemy::Initalize(Model* model, ViewProjection* viewProjection, const Vector3& position) {
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	viewProjection_ = viewProjection;
	enemyModel_ = model;

	velocity_ = {-kMoveSpeed, 0, 0};

	//kMoveSpeed = 0.0f;
	enemySpeed_ = 0.01f;
}

void MoveEnemy::Update() {
	playerPos = player_->GetWorldPosition();
	playerDirection = player_->GetPlayerDirection();
	// 敵の座標とプレイヤーの座標を比べる
	if (worldTransform_.translation_.x >=playerPos.x &&
	    worldTransform_.translation_.y >=playerPos.y) {
		enemyDirection.x = -1;
		enemyDirection.y = -1;
	}
	if (worldTransform_.translation_.x <playerPos.x &&
	    worldTransform_.translation_.y> playerPos.y) {
		enemyDirection.x = 1;
		enemyDirection.y = -1;
	}
	if (worldTransform_.translation_.x > playerPos.x &&
	    worldTransform_.translation_.y < playerPos.y) {
		enemyDirection.x = -1;
		enemyDirection.y = 1;
	}
	if (worldTransform_.translation_.x < playerPos.x &&
	    worldTransform_.translation_.y < playerPos.y) {
		enemyDirection.x = 1;
		enemyDirection.y = 1;
	}

	
}

void MoveEnemy::Draw() { 
	if (!isA)
	{
		enemyModel_->Draw(worldTransform_, *viewProjection_);
	}
}

Vector3 MoveEnemy::GetWorldPosition() { 
	Vector3 worldPos;
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;
	return worldPos;
}

AABB MoveEnemy::GetAABB() { return AABB(); }

//void MoveEnemy::OnCollisiton(const Player* player) {}


Vector3 MoveEnemy::trackingSpeed(Vector3& enemy, Vector3& player, float enemySpeed) {
	// プレイヤーと敵の間の方向ベクトルを計算
	Vector3 direction = {player.x - enemy.x, player.y - enemy.y, 0};
	float em = enemySpeed;
	// 方向ベクトルを正規化（単位ベクトルにする）
	direction = Normalize(direction);

	// 敵の位置を更新（方向ベクトルに速度を掛けて移動）
	enemy.x += direction.x * em;
	enemy.y += direction.y * em;
	//enemy.z += direction.z * enemySpeed;

	return enemy; // 更新後の敵の位置を返す
}
