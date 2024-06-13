#define NOMINMAX
#include "Player.h"
#include <numbers>
#include <Input.h>
#include <model.h>
#include "Model.h"
#include "string.h"
#include<string>
#include <imgui.h>
#include <algorithm>
void Player::Initalize(Model* model, ViewProjection* viewProjection, const Vector3& position) 
{
	worldTransform_.Initialize();
	worldTransform_.translation_=position;

	
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;

	viewProjection_ = viewProjection;
	playerModel_ = model;

}

void Player::Update() 
{ 
	// 着地するときのフラグ
	bool landing = false;

	//地面にいるとき
	if (onGround_==true)
	{
		if (velocity_.y > 0.0f)
		{
			onGround_ = false;
		}

		CollisonMapInfo collisonMapInfo;
		collisonMapInfo.move = velocity_;
		mapCollision(collisonMapInfo);

		//左右移動操作
		if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT)) {
			Vector3 acceleration = {};
			if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
				if (velocity_.x < 0.0f) {
					velocity_.x *= (1.0f - kAttenuation);
				}
				acceleration.x += kAceeleration;
				// 向かう方向に変わる
				if (lrDirection_ != LRDirection::kRight) {
					lrDirection_ = LRDirection::kRight;
					turnFirstRotationY_ = -1.0f;
					turnTimer_ = 3.0f;
				}
			} else if (Input::GetInstance()->PushKey(DIK_LEFT)) {
				if (velocity_.x > 0.0f) {
					velocity_.x *= (1.0f - kAttenuation);
				}
				acceleration.x -= kAceeleration;
				// 向かう方向に変わる
				if (lrDirection_ != LRDirection::kLeft) {
					lrDirection_ = LRDirection::kLeft;
					turnFirstRotationY_ = 1.0f;
					turnTimer_ = 3.0f;
				}
			} else {
				velocity_.x += (1.0f - kAttenuation);
			}

			velocity_ += acceleration;
			//velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);

			if (turnTimer_ > 0.0f) {
				turnTimer_ = 60.0f / 1.0f;
				float destinationRotationYTable[] = {
				    std::numbers::pi_v<float> / 2.0f, std::numbers::pi_v<float> * 3.0f / 2.0f};

				// float destinationRotationY =
				//     destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];

				worldTransform_.rotation_.y = sin((turnFirstRotationY_ * turnTimer_) / 2);
			}
		}
		
		// 上キー押していたら
		if (Input::GetInstance()->TriggerKey(DIK_UP)) 
		{
			
			// ジャンプの加速度
			velocity_ += Vector3(0, kJumpAcceleration, 0);
		}
	}
	//空中にいるとき
	else if (onGround_ == false)
	{
	
		
		velocity_ += Vector3(0, -kGravityAcceleration, 0);
		velocity_.y = std::max(velocity_.y, -kLimitFallSpeed);
		
		// 地面との当たり判定
		if (velocity_.y < 0)
		{
			if (worldTransform_.translation_.y <= 1.0f) 
			{
				landing = true;
			}
		}
		
		//着地のフラグがtrue
		if (landing==1)
		{
			worldTransform_.translation_.y = 1.0f;
			velocity_.x *= (1.0f - kAttenuationLanding);
			velocity_.y = 0.0f;
			onGround_ = true;
		}	
	}
	//旋回制御
	worldTransform_.translation_ += velocity_;

	//行列計算
	worldTransform_.UpdateMatrix();
}

void Player::Draw() 
{ 
	playerModel_->Draw(worldTransform_, *viewProjection_);

}

WorldTransform& Player::GetWorldTransform() { return worldTransform_; }

void Player::SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }


//マップとの衝突判定
void Player::mapCollision(CollisonMapInfo& info) { 
	mapCollisionDetectionUp(&info);
	mapCollisionDetectionDown(&info);
	mapCollisionDetectionLeft(&info);
	mapCollisionDetectionRight(&info);
}

//マップとの衝突判定の四方向
void Player::mapCollisionDetectionUp(CollisonMapInfo* collisonMapInfoUp) {}

void Player::mapCollisionDetectionDown(CollisonMapInfo* collisonMapInfoDown) {}

void Player::mapCollisionDetectionLeft(CollisonMapInfo* collisonMapInfoLeft) {}

void Player::mapCollisionDetectionRight(CollisonMapInfo* collisonMapInfoRight) {}

Vector3 Player::CornnerPosition(const Vector3& center, Corner corner) {

	Vector3 offsetTable[Player::Corner::kNumCorner] = {
	    {+kWidth / 2.0f, -kHeight / 2.0f, 0}, //  kRgithBottom
	    {-kWidth / 2.0f, -kHeight / 2.0f, 0}, //  kLeftBottom
	    {+kWidth / 2.0f, +kHeight / 2.0f, 0}, //  kRightTop
	    {-kWidth / 2.0f, +kHeight / 2.0f, 0}, //  kLeftTop
	};

	return center + offsetTable[static_cast<uint32_t>(corner)];
}
