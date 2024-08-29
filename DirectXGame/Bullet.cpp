#include "Bullet.h"
#include "Enemy.h"
//Bullet::Bullet() {}

void Bullet::Initalize(
    Model* model, ViewProjection* viewProjection, const Vector3& position, float lifetime,float speed) {
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	initialPosition_ = position; // 初期位置を保存
	viewProjection_ = viewProjection;
	bulletModel_ = model;
	lifetime_ = lifetime;
	speed_ = speed;
	isBullet_ = true;
}

void Bullet::Update() {
	velocity_ += Vector3(0,speed_/60.0f,0.0f);
	worldTransform_.translation_.y-= velocity_.y;
	
	
	if (!isBullet_)
	{
		return;
	}
	if (IsOutOfBounds()) {
		Reset();
	} 
	else
	{
		lifetime_ -= 1.0f / 60.0f; // 1秒間に60フレームと仮定
	}

	
	//float param = std::sin(walkTimer_/kWalkMotionTime);
	//float radian = kWalkMotionAngleStart + kWalkMotionAngleEnd * (param + 1.0f) / 2.0f;
	//worldTransform_.rotation_.x = radian * ((float)PI/ 180);
	//worldTransform_.translation_ += velocity_;
	//  行列計算
	worldTransform_.UpdateMatrix();
	worldTransform_.TransferMatrix();
}

void Bullet::Draw() { 
	if (lifetime_ > 0.0f)
	{
		bulletModel_->Draw(worldTransform_, *viewProjection_); 
	}

}

bool Bullet::IsOutOfBounds() const {
	const float screenWidth = 1280.0f;
	const float screenHeight = 720.0f;

	return worldTransform_.translation_.x < 2 || worldTransform_.translation_.x > screenWidth ||
	       worldTransform_.translation_.y < 2 || worldTransform_.translation_.y > screenHeight ||
	       lifetime_ <= 0.0f;
}


AABB Bullet::GetAABB() {
	Vector3 worldPos = GetWorldPosition();

	AABB aabb;
	aabb.min = {
	    worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb.max = {
	    worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};
	return aabb;
}

// 自キャラのワールド座標
Vector3 Bullet::GetWorldPosition() {
	Vector3 worldPos;
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;
	return worldPos;
}
void Bullet::Reset() {
	worldTransform_.translation_ = initialPosition_;
	lifetime_ = 700.0f;
	isBullet_ = true;
	velocity_ = {0.f, 0.f, 0.f};
}
void Bullet::OnCollisiton(const Player* player) { 
	(void)player;
}