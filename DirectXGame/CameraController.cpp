#define NOMINMAX 
#include "CameraController.h"
#include "Player.h"
#include "WorldTransform.h"
#include <string>
#include <cassert>
#include <vector>
#include "string.h"
#include <algorithm>
#include <iostream>
#include "Windows.h"

CameraController::CameraController() {}

CameraController::~CameraController() {}

void CameraController::Initialize(ViewProjection* viewProjection) { 
	viewProjection_ = viewProjection;
	
	//targetWorldTransform_.Initialize();
}

void CameraController::Update() {
	SetTarget(target_);

	// ターゲットが設定されている場合
	if (target_) {
		const WorldTransform& targetWorldTransform = target_->GetWorldTransform();

		// 追従対象とオフセットからカメラの目標座標を計算
		targetCoordinates.x =
		    targetWorldTransform.translation_.x + targetOffset_.x + targetVelocity * kVelocityBias;
		targetCoordinates.y =
		    targetWorldTransform.translation_.y + targetOffset_.y + targetVelocity * kVelocityBias;
		targetCoordinates.z =
		    targetWorldTransform.translation_.z + targetOffset_.z + targetVelocity * kVelocityBias;

		// 座標保管によりゆったり追従
		viewProjection_->translation_ =
		    Lerp(viewProjection_->translation_, targetCoordinates, kInterpolationRate);
	}

	// オートスクロール処理 (X軸方向に一定速度で移動)
	viewProjection_->translation_.x += kAutoScrollSpeed; // 自動スクロール速度を加算

	// 移動範囲宣言
	viewProjection_->translation_.x = std::max(viewProjection_->translation_.x, movableArea_.left);
	viewProjection_->translation_.x = std::min(viewProjection_->translation_.x, movableArea_.right);
	viewProjection_->translation_.y =
	    std::max(viewProjection_->translation_.y, movableArea_.bottom);
	viewProjection_->translation_.y = std::min(viewProjection_->translation_.y, movableArea_.top);

	// マージン範囲による制御
	viewProjection_->translation_.x =
	    std::max(viewProjection_->translation_.x, targetCoordinates.x + margin.left);
	viewProjection_->translation_.x =
	    std::min(viewProjection_->translation_.x, targetCoordinates.x + margin.right);
	viewProjection_->translation_.y =
	    std::max(viewProjection_->translation_.y, targetCoordinates.y + margin.bottom);
	viewProjection_->translation_.y =
	    std::min(viewProjection_->translation_.y, targetCoordinates.y + margin.top);

	// 行列の更新
	viewProjection_->UpdateMatrix();
	viewProjection_->TransferMatrix();
}

void CameraController::Draw() {}

void CameraController::Reset() { 
	SetTarget(target_);
	const WorldTransform& targeWorldTransform = target_->GetWorldTransform();
	//viewProjection_.translation_ = targetWorldTransform_.translation_ + targetOffset_;
	(*viewProjection_).translation_.x = targeWorldTransform.translation_.x + targetOffset_.x;
	(*viewProjection_).translation_.y = targeWorldTransform.translation_.y + targetOffset_.y;
	(*viewProjection_).translation_.z = targeWorldTransform.translation_.z + targetOffset_.z;
}
Vector3 CameraController::Lerp(const Vector3& a, const Vector3& b, float t) {
	Vector3 pos = {
	    (float)(1.0 - t) * a.x + t * b.x, (float)(1.0 - t) * a.y + t * b.y,
	    (float)(1.0 - t) * a.z + t * b.z};
	return pos;
}

	

//void CameraController::SetMovebleArea(Rest area) { movableArea_ = area; }


