#include "TitleModel.h"

#include <cassert>

TitleModel::TitleModel() {}

TitleModel::~TitleModel() {}

void TitleModel::Initalize(Model* model, ViewProjection* viewProjection, const Vector3& position) {
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

	viewProjection_ = viewProjection;
	titleModel_ = model;
}

void TitleModel::Update() {
	worldTransform_.UpdateMatrix();
	worldTransform_.TransferMatrix();
}

void TitleModel::Draw() { titleModel_->Draw(worldTransform_, *viewProjection_); }