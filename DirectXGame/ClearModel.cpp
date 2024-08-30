#include "ClearModel.h"

void ClearModel::Initalize(Model* model, ViewProjection* viewProjection, const Vector3& position) {
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

	viewProjection_ = viewProjection;
	ClearModel_ = model;
}

void ClearModel::Update() {
	worldTransform_.UpdateMatrix();
	worldTransform_.TransferMatrix();
}

void ClearModel::Draw() { ClearModel_->Draw(worldTransform_, *viewProjection_); }
