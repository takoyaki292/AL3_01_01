#include "Player.h"
#include <numbers>
#include <Input.h>
#include <model.h>
#include "Model.h"

void Player::Initalize(Model* model, ViewProjection* viewProjection, const Vector3& position) 
{
	worldTransform_.Initialize();
	worldTransform_.translation_=position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;

	viewProjection_ = viewProjection;
	playerModel_ = model;

}

void Player::Update() 
{ 
	if (Input::GetInstance()->PushKey(DIK_RIGHT) ||
		Input::GetInstance()->PushKey(DIK_LEFT))
	{
		Vector3 acceleration = {};
		if (Input::GetInstance()->PushKey(DIK_RIGHT))
		{
			acceleration.x += kAceeleration;
		}
		else if (Input::GetInstance()->PushKey(DIK_LEFT))
		{
			acceleration.x -= kAceeleration;
		}
		velocity_+= acceleration;
	}
	worldTransform_.translation_+= velocity_;
	
	worldTransform_.UpdateMatrix();
}

void Player::Draw() 
{ 
	playerModel_->Draw(worldTransform_, *viewProjection_);

}
