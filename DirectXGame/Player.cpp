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
#include "MapChipField.h"
#include <cassert>
#include <functional>
#include "ImGuiManager.h"
#include "DebugText.h"
void Player::Initalize(Model* model, ViewProjection* viewProjection, const Vector3& position)
{
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.translation_.y = 2.0f;
	worldTransform_.translation_.x = 2.0f;

	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;

	viewProjection_ = viewProjection;
	playerModel_ = model;

	
}

void Player::Update() {
	//移動入力
	// 左右移動操作
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
		// velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);

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
	if (Input::GetInstance()->TriggerKey(DIK_UP)) {

		// ジャンプの加速度
		velocity_ += Vector3(0, kJumpAcceleration, 0);
	}

	// 移動量を加味して衝突判定
	CollisonMapInfo info;
	info.move = velocity_;
	mapCollision(info);

	// 反映する処理
	Reflection(info);
	// 天井にあたっていると処理をする
	ceiling(info);

	//接地状態の切り替え
	landing(info);

	// 旋回制御
	worldTransform_.translation_ += velocity_;

	// 行列計算
	worldTransform_.UpdateMatrix();
}

void Player::Draw() { playerModel_->Draw(worldTransform_, *viewProjection_); }

WorldTransform& Player::GetWorldTransform() { return worldTransform_; }

void Player::SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }

// マップとの衝突判定
void Player::mapCollision(CollisonMapInfo& info) {
	mapCollisionDetectionUp(&info);
	mapCollisionDetectionDown(&info);
	// mapCollisionDetectionLeft(&info);
	// mapCollisionDetectionRight(&info);
}

// マップとの衝突判定の四方向
//上方向の当たり判定
void Player::mapCollisionDetectionUp(CollisonMapInfo* info) {
	std::array<Vector3, kNumCorner> positionNew{};

	if (info->move.y <= 0) {
		return;
	}
	for (uint32_t i = 0; i < positionNew.size(); i++) {
		positionNew[i] =
		    CornnerPosition(worldTransform_.translation_ + info->move, static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	
	IndexSet indexSet;

	bool hit = false;
	// 左上の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kLeftTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	// 右上の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kRightTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType ::kBlock) {
		hit = true;
	}
	if (hit == true&&info->ceilingCollisionFlag==false) {
		//IndexSet index;
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(
		    positionNew[kLeftTop]);
		
		Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info->move.y = std::max(0.0f,info->move.y);
		
		//天井のフラグを立てている
		info->ceilingCollisionFlag = true;
		
	} 
	
}
// 下方向の当たり判定
void Player::mapCollisionDetectionDown(CollisonMapInfo* info) {
	std::array<Vector3, kNumCorner> positionNew{};

	if (info->move.y >= 0) {
		return;
	}
	for (uint32_t i = 0; i < positionNew.size(); i++) {
		positionNew[i] =
		    CornnerPosition(worldTransform_.translation_ + info->move, static_cast<Corner>(i));
	}

	MapChipType mapChipType;

	IndexSet indexSet;

	bool hit = false;
	// 左下の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kLeftBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	// 右下の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kRightBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType ::kBlock) {
		hit = true;
	}
	if (hit == true && info->landingFlag== false) {
		//めり込まないように移動量を設定する
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kRightBottom]);
		//めり込む先のブロックの範囲矩形
		Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info->move.y = std::min(0.0f, info->move.y);

		//着地フラグをtrueにする
		info->landingFlag = true;
	}
}
//
// void Player::mapCollisionDetectionLeft(CollisonMapInfo* collisonMapInfoLeft) {}
//
// void Player::mapCollisionDetectionRight(CollisonMapInfo* collisonMapInfoRight) {}

Vector3 Player::CornnerPosition(const Vector3& center, Corner corner) {

	Vector3 offsetTable[Player::Corner::kNumCorner] = {
	    {+kWidth / 2.0f, -kHeight / 2.0f, 0}, //  kRgithBottom
	    {-kWidth / 2.0f, -kHeight / 2.0f, 0}, //  kLeftBottom
	    {+kWidth / 2.0f, +kHeight / 2.0f, 0}, //  kRightTop
	    {-kWidth / 2.0f, +kHeight / 2.0f, 0}, //  kLeftTop
	};

	return center + offsetTable[static_cast<uint32_t>(corner)];
}

void Player::Reflection(const CollisonMapInfo& info) { worldTransform_.translation_ += info.move; }

void Player::ceiling(const CollisonMapInfo& info) {
	if (info.ceilingCollisionFlag==true) {
		DebugText::GetInstance()->ConsolePrintf("hit ceiling\n\n");
		velocity_.y = 0;
		onGround_ = false;
	}
}

void Player::landing(const CollisonMapInfo& info) { 
	//接地状態の処理
	if (onGround_ == true) {
		if (velocity_.y > 0.0f) {
			onGround_ = false;

		} 
		else{
			//着地フラグをtrueにする
			info.landingFlag= true;
		}
	} 
	//空中状態の処理
	else if(onGround_==false){
		
		if (info.landingFlag== true) {
			worldTransform_.translation_.y = 2.0f;
			// 着地時にx速度を減衰
			velocity_.x *= (1.0f - kAttenuationLanding);
			// y速度をゼロにする
			 velocity_.y = 0.0f;
			//// 落下を止める
			onGround_ = true;
		}
	}


	std::array<Vector3, kNumCorner> positionNew{};
	for (uint32_t i = 0; i < positionNew.size(); i++) {
		positionNew[i] =
		    CornnerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
	}
	MapChipType mapChipType;
	
	IndexSet indexSet;
	
	bool hit = false;
	
	// 左下の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kLeftBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlank) {
		hit = true;
	}
	// 右下の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kRightBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType ::kBlank) {
		hit = true;
	}
	
	//positionNew[kLeftBottom] + Vector3(0, -0.5, 0);
	//positionNew[kRightBottom] + Vector3(0, -0.5, 0);
	
	if (!hit) {
		onGround_ = false;
	}
}
