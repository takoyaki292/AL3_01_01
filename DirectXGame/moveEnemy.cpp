#include "moveEnemy.h"
#include "WorldTransform.h"
void MoveEnemy::Initalize(Model* model, ViewProjection* viewProjection, const Vector3& position) {
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	// bulletWorldTransform_.translation_ = worldTransform_.translation_;
	// worldTransform_.translation_.y = 4.0f;
	// worldTransform_.translation_.x = 30.0f;
	// worldTransform_.rotation_.y = -180;
	// worldTransform_.rotation_.z = 90;

	viewProjection_ = viewProjection;
	enemyModel_ = model;
}

void MoveEnemy::Update() {

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