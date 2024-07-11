#include "DeathParticles.h"

DeathParticles::DeathParticles() {}

DeathParticles::~DeathParticles() { 
	delete deathParticlesModel_;
	delete deathParticlesPlayer_;
}



void DeathParticles::Initalize(Model* model, ViewProjection* viewProjection, const Vector3& position) {
	deathParticlesPlayer_ = new Player();
	viewProjection_ = viewProjection;
	deathParticlesModel_ = model;
	for (auto& worldTransform : worldTransforms_) {
		worldTransform.Initialize();
		worldTransform.translation_ = position;
		worldTransform.translation_.y = 2.0f;
		worldTransform.translation_.x = 2.0f;
	}
	deathParticlesPlayer_->Initalize(deathParticlesModel_, viewProjection_, position);
}

void DeathParticles::Update() {
	for (WorldTransform& worldTransform : worldTransforms_) {

		worldTransform.UpdateMatrix();
	}
}

void DeathParticles::Draw() { 
	for (WorldTransform& worldTransform : worldTransforms_) {

		deathParticlesModel_->Draw(worldTransform, *viewProjection_); 
	}
}
