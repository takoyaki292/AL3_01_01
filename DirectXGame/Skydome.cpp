#include "Skydome.h"
#include "cassert"
#include "Sprite.h"
void Skydome::Initialize(Model* model, ViewProjection* viewProjection) { 
	

	assert(model);
	model_ = model;
	//textureHandle_ = textureHandle;
	worldTransform_.Initialize();
	viewProjection_ = viewProjection;
	
}

void Skydome::Update() 
{ worldTransform_.TransferMatrix(); 
}

void Skydome::Draw() 
{ 
	model_->Draw(worldTransform_, *viewProjection_);
}
