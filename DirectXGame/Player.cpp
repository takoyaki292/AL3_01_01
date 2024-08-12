#define NOMINMAX
#include "Player.h"
#include <numbers>
#include <Input.h>
#include <model.h>
#include "Model.h"
#include "string.h"
#include <algorithm>
#include "MapChipField.h"
#include <cassert>
#include <functional>
#include "ImGuiManager.h"
#include "DebugText.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <Input.h>
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
				turnFirstRotationY_ =-worldTransform_.rotation_.y;
				turnTimer_ = kTimeTurn;
			}
		} else if (Input::GetInstance()->PushKey(DIK_LEFT)) {
			if (velocity_.x > 0.0f) {
				velocity_.x *= (1.0f - kAttenuation);
			}
			acceleration.x -= kAceeleration;
			// 向かう方向に変わる
			if (lrDirection_ != LRDirection::kLeft) {
				lrDirection_ = LRDirection::kLeft;
				turnFirstRotationY_ = worldTransform_.rotation_.y;
				turnTimer_ = kTimeTurn;
			}
		} else {
			velocity_.x += (1.0f - kAttenuation);
		}
		velocity_ += acceleration;
	}
	if (turnTimer_ > 0.0f) {
		turnTimer_ -= 1.0f / 60.0f;
		float destinationRotationYTable[] = {
		    std::numbers::pi_v<float> / 2.0f, 
			std::numbers::pi_v<float> * 3.0f / 2.0f};

		float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];
		worldTransform_.rotation_.y =
			easeInOut(destinationRotationY, turnFirstRotationY_, turnTimer_ / kTimeTurn);

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
	if (!info.landingFlag)
	{
		// 天井にあたっていると処理をする
		ceiling(info);
	}
	// 壁に接触している場合の処理
	wallContact(info);

	if (!info.ceilingCollisionFlag) {
		//接地状態の切り替え
		landing(info);
	}

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
	mapCollisionDetectionLeft(&info);
	mapCollisionDetectionRight(&info);
	mapCollisionDetectionUp(&info);
	mapCollisionDetectionDown(&info);
}

// マップとの衝突判定の四方向
// 右方向の当たり判定
void Player::mapCollisionDetectionRight(CollisonMapInfo* info) {

	std::array<Vector3, kNumCorner> positionNew{};

	if (info->move.x <= 0) {
		return;
	}
	for (uint32_t i = 0; i < positionNew.size(); i++) {
		positionNew[i] =
		    CornnerPosition(worldTransform_.translation_ + info->move, static_cast<Corner>(i));
	}

	MapChipType mapChipType;

	IndexSet indexSet;

	bool hit = false;
	positionNew[kRightTop] -= Vector3(-kGaq, 0, 0);
	positionNew[kRightBottom] -= Vector3(-kGaq, 0, 0);
	// 右下の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kRightBottom]);
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
	if (hit == true) {
		//float right = worldTransform_.translation_.x - kWidth / 2+kGaq;
		float right = worldTransform_.translation_.x - kWidth / 2;
		Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info->move.x = std::min(0.0f, rect.left - right);
		// 壁のフラグを立てている
		info->wallContactFlag = true;
	}
}
 //左方向の当たり判定
void Player::mapCollisionDetectionLeft(CollisonMapInfo* info) {
	
	std::array<Vector3, kNumCorner> positionNew{};

	if (info->move.x  >= 0) {
		return;
	}
	for (uint32_t i = 0; i < positionNew.size(); i++) {
		positionNew[i] =
		    CornnerPosition(worldTransform_.translation_ + info->move, static_cast<Corner>(i));
	}

	MapChipType mapChipType;

	IndexSet indexSet;

	bool hit = false;
	positionNew[kLeftTop] += Vector3(-kGaq,0, 0);
	positionNew[kLeftBottom] += Vector3(-kGaq,0, 0);
	// 左上の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kLeftTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	// 左下の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kLeftBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType ::kBlock) {
		hit = true;
	}
	if (hit == true && info->wallContactFlag== false) {
		float left = worldTransform_.translation_.x - kWidth / 2;
		Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info->move.x = std::max(0.0f, rect.right-left);
		// 壁のフラグを立てている
		info->wallContactFlag = true;
	} 
 }
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

	positionNew[kLeftTop] += Vector3(0, kGaq, 0);
	positionNew[kRightTop] += Vector3(0, kGaq, 0);
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
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(
		    positionNew[kLeftTop]);
		
		float top = kGaq+worldTransform_.translation_.y + kHeight / 2;
		Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info->move.y = std::max(0.0f, rect.bottom - top);
		
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
	positionNew[kLeftTop] += Vector3(0, -kGaq, 0);
	positionNew[kRightTop] += Vector3(0, -kGaq, 0);
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
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kLeftBottom]);
		//めり込む先のブロックの範囲矩形
		float bottom = worldTransform_.translation_.y  -kHeight / 2-0.2f;
		Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info->move.y = std::max(0.0f, rect.top-bottom);

		//着地フラグをtrueにする
		info->landingFlag = true;
	} 
}


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
	} 
}

void Player::landing(const CollisonMapInfo& info) { 
	//接地状態の処理
	if (onGround_==true) {
		if (velocity_.y > 0.0f) {
			onGround_ = false;
		} 
		else {
			std::array<Vector3, kNumCorner> positionNew{};
			for (uint32_t i = 0; i < positionNew.size(); i++) {
				positionNew[i] = CornnerPosition(
				    worldTransform_.translation_ + info.move, static_cast<Corner>(i));
			}
			MapChipType mapChipType;

			IndexSet indexSet;

			bool hit = false;

			// 左下の判定
			indexSet = mapChipField_->GetMapChipIndexSetByPosition(
			    positionNew[kLeftBottom] + Vector3(0, -kGaq, 0));
			mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);

			if (mapChipType == MapChipType ::kBlock) {
				hit = true;
			}
			// 右下の判定
			indexSet = mapChipField_->GetMapChipIndexSetByPosition(
			    positionNew[kRightBottom] + Vector3(0, -kGaq, 0));
			mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
			if (mapChipType == MapChipType ::kBlock) {
				hit = true;
			}
			if (!hit){
				onGround_ = false;
			}

		}
	} 
	//空中状態の処理
	else if (onGround_ == false) {
		
		if (info.landingFlag==true) {
			DebugText::GetInstance()->ConsolePrintf("down ceiling\n\n");
			//worldTransform_.translation_.y = 2.0f;
			 //// 落下を止める
			 onGround_ = true;
			// 着地時にx速度を減衰
			velocity_.x *= (1.0f - kAttenuationLanding);
			// y速度をゼロにする
			 velocity_.y = 0.0f;
			 
		} 
		else{
			velocity_ += Vector3(0, -kGravityAcceleration, 0);
			velocity_.y = std::max(velocity_.y, -kLimitFallSpeed);
		}

	}
}

void Player::wallContact(const CollisonMapInfo& info) { 
	if (info.wallContactFlag == true){
		DebugText::GetInstance()->ConsolePrintf("wall ceiling\n\n");
		velocity_.x = 0.0f;
		//velocity_.x *= (1.0f - kAtteuationWall);
	}

}

//float Player::easeInOutSine(float num) { 
//	return -(cos((float)M_PI*num)-1)/2; }

float Player::easeInOut(float x1, float x2, float t) {
	float a = -(std::cosf(std::numbers::pi_v<float> * t) - 1.0f) / 2.0f;
	return Lerp(x1, x2, a);
}

float Player::Lerp(float x1, float x2, float t) { return (1.0f - t) * x1 + t * x2; }

//自キャラのワールド座標
Vector3 Player::GetWorldPosition() { 
	Vector3 worldPos;
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;
	return worldPos;
}

AABB Player::GetAABB() { 
	Vector3 worldPos = GetWorldPosition();

	AABB aabb;
	aabb.min = {
	    worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb.max = {
	    worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};
	return aabb;
}

void Player::OnCollision(const Enemy* enemy) { 
	(void)enemy;
	DebugText::GetInstance()->ConsolePrintf("enemy ceiling\n\n");
	velocity_ += Vector3(0,0.1f,0);
}
