#pragma once
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
class TitleModel {
public:
	TitleModel();
	~TitleModel();

	void Initalize(Model* model, ViewProjection* viewProjection, const Vector3& position);

	void Update();

	void Draw();

private:
	WorldTransform worldTransform_;
	ViewProjection* viewProjection_ = {};
	Model* titleModel_ = nullptr;
};