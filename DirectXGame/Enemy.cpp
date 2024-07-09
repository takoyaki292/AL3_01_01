#include "Enemy.h"
#include "Vector3.h"
#include<numbers>

void Enemy::Initalize(Model* model, ViewProjection* viewProjection, const Vector3& position) { 
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	//worldTransform_.translation_.y = 2.0f;
	//worldTransform_.translation_.x = 30.0f;
	worldTransform_.rotation_.y = std::numbers::pi_v<float>*1.5f;

	viewProjection_ = viewProjection;
	enemyModel_ = model;

	velocity_ = {-kWalkSpeed, 0, 0};

	walkTimer_ = 0.0f;
}

void Enemy::Update() {
	//アニメーションのタイマーを加算
	walkTimer_ += 1.0f / 30.0f;
	//float param = std::sin(walkTimer_-kWalkMotionTime);
	float param = std::sin(walkTimer_/kWalkMotionTime);
	float radian = kWalkMotionAngleStart + kWalkMotionAngleEnd * (param + 1.0f) / 2.0f;
	worldTransform_.rotation_.x = radian * ((float)PI/ 180);
	worldTransform_.translation_ += velocity_;
	// 行列計算
	worldTransform_.UpdateMatrix();
	worldTransform_.TransferMatrix();
}

void Enemy::Draw() { 
	enemyModel_->Draw(worldTransform_, *viewProjection_); }

void Enemy::SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }
