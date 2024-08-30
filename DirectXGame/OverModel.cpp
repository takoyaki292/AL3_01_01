#include "OverModel.h"

void OverModel::Initalize(Model* model, ViewProjection* viewProjection, const Vector3& position) {
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

	viewProjection_ = viewProjection;
	OverModel_ = model;
}

void OverModel::Update() {
	worldTransform_.UpdateMatrix();
	worldTransform_.TransferMatrix();
}

void OverModel::Draw() { OverModel_->Draw(worldTransform_, *viewProjection_); }
