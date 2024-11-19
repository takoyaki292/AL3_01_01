#include "Bullet.h"
#include "Enemy.h"
//Bullet::Bullet() {}



void Bullet::Initalize(
    Model* model, ViewProjection* viewProjection, const Vector3& position, float lifetime,
    float speed, const Vector3& direction) {
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	initialPosition_ = position;
	viewProjection_ = viewProjection;
	bulletModel_ = model;
	lifetime_ = lifetime;
	speed_ = speed;
	direction_ = direction;        // 外部から方向を設定
	velocity_ = direction * speed; // 速度を設定
	isBullet_ = true;
}

void Bullet::Update() {
	
	//velocity_ += Vector3(0,speed_/60.0f,0.0f);
	worldTransform_.translation_+= velocity_;
	//worldTransform_.translation_.x+= velocity_.x;
	
	
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
	//const float screenWidth = 1280.0f;
	const float screenHeight = 720.0f;

	// X方向の画面外判定
	bool outOfBoundsX =
	    (worldTransform_.translation_.x < 14.0f) || (worldTransform_.translation_.x > 100);

	// Y方向の画面外判定
	bool outOfBoundsY =
	    (worldTransform_.translation_.y < 2.0f) || (worldTransform_.translation_.y > screenHeight);

	// 寿命切れの判定
	bool lifetimeExpired = (lifetime_ <= 0.0f);

	// いずれかの条件を満たす場合、画面外または寿命切れと判定
	return outOfBoundsX || outOfBoundsY || lifetimeExpired;
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
}
void Bullet::OnCollisiton(const Player* player) { 
	(void)player;
}