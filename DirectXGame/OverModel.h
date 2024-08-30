#pragma once
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
class OverModel {
public:
	OverModel();
	~OverModel();

	void Initalize(Model* model, ViewProjection* viewProjection, const Vector3& position);

	void Update();

	void Draw();

private:
	WorldTransform worldTransform_;
	ViewProjection* viewProjection_ = {};
	Model* OverModel_ = nullptr;
};