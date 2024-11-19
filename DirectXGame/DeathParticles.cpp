#include "DeathParticles.h"
#include <cassert>
#include <iostream>
#include <algorithm>
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
		//worldTransform.translation_.y = 2.0f;
		//worldTransform.translation_.x = 2.0f;
	}
	deathParticlesPlayer_->Initalize(deathParticlesModel_, viewProjection_, position);

	objectColor_.Initialize();
	color_ = {1,1,1,1};
}

void DeathParticles::Update()
{
	if (isFinished_ == false)
	{
		for (WorldTransform& worldTransform : worldTransforms_) {

			worldTransform.UpdateMatrix();
		}
		for (uint32_t i = 0; i < kNumParticles; ++i) {

			Vector3 velocity = {kSpped, 0, 0};

			float angle = kAngleUnit * i;
			Matrix4x4 matrixRotation = MakeRotateZMatrix(angle);

			velocity = Transform(velocity, matrixRotation);

			worldTransforms_[i].translation_ += velocity;
			counter_ += 1.0f / 60.0f;

			//counter_が0になったときに
			if (counter_ == 0.0f) {
				colorCounter_ = 1.0f;
			}
			//counter_が、消滅時間になったときに
			else if (counter_ == kDuration) {
				colorCounter_ = 0.0f;
			}
			//その他なら
			else {
				colorCounter_ -= 0.05f / 60.0f;
			}

			// 透明にしていく処理
			color_.w = std::clamp(colorCounter_, 0.0f, 1.0f);
			objectColor_.SetColor(color_);
			objectColor_.TransferMatrix();
		}
		
		if (counter_ >= kDuration) {
			counter_ = kDuration;
			isFinished_ = true;
			finished_ = true;
		}
	} 
	else if(isFinished_==true){
		return;
	}
	
}

void DeathParticles::Draw() { 
	if (!isFinished_)
	{
		for (WorldTransform& worldTransform : worldTransforms_) {
			deathParticlesModel_->Draw(worldTransform, *viewProjection_, &objectColor_);
		}
	}
}

Matrix4x4 DeathParticles::MakeRotateZMatrix(float radian) {
	Matrix4x4 m = {};

	m.m[0][0] = std::cos(radian);
	m.m[0][1] = std::sin(radian);
	m.m[0][2] = 0;
	m.m[0][3] = 0;

	m.m[1][0] = std::sin(-radian);
	m.m[1][1] = std::cos(radian);
	m.m[1][2] = 0;
	m.m[1][3] = 0;

	m.m[2][0] = 0;
	m.m[2][1] = 0;
	m.m[2][2] = 1;
	m.m[2][3] = 0;

	m.m[3][0] = 0;
	m.m[3][1] = 0;
	m.m[3][2] = 0;
	m.m[3][3] = 1;
	return m;
}

Vector3 DeathParticles::Transform(const Vector3& vector, const Matrix4x4& matrix) {
	Vector3 r;
	r.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] +
	      1.0f * matrix.m[3][0];
	r.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] +
	      1.0f * matrix.m[3][1];
	r.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] +
	      1.0f * matrix.m[3][2];
	float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] +
	          1.0f * matrix.m[3][3];

	assert(w != 0.0f);

	r.x /= w;
	r.y /= w;
	r.z /= w;
	return r;
}