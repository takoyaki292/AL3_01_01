#include "CameraController.h"
#include "Player.h"
#include "WorldTransform.h"


CameraController::CameraController() {}

CameraController::~CameraController() {}

void CameraController::Initialize() { viewProjection_.Initialize();
}

void CameraController::Update() {
	const WorldTransform& targeWorldTransform =target_->GetWorldTransform();
	SetTarget(&targeWorldTransform);
	
	viewProjection_.translation_.x= targetWorldTransform_.translation_.x + targetOffset_.x;
	viewProjection_.translation_.y= targetWorldTransform_.translation_.y + targetOffset_.y;
	viewProjection_.translation_.z= targetWorldTransform_.translation_.z + targetOffset_.z;

	viewProjection_.UpdateMatrix();

}

void CameraController::Draw() {}


void CameraController::SetTarget(Player* target) { target_ = target; }

void CameraController::Reset() { 
	const WorldTransform& targeWorldTransform = target_->GetWorldTransform();

	
	//viewProjection_.translation_ = targetWorldTransform_.translation_ + targetOffset_;
	viewProjection_.translation_.x = targetWorldTransform_.translation_.x + targetOffset_.x;
	viewProjection_.translation_.y = targetWorldTransform_.translation_.y + targetOffset_.y;
	viewProjection_.translation_.z = targetWorldTransform_.translation_.z + targetOffset_.z;
}

void CameraController::SetMovebleArea(Rest area) { movableArea_ = area; }


