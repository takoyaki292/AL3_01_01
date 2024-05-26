#include "Player.h"
#include <numbers>

void Player::Initalize(Model* model, ViewProjection* viewProjection, const Vector3& position) 
{
	worldTransform_.Initalize();
	worldTransform_.translation=position;
	worldTransform_.rotation.y = std::numbers::pi_v<float> / 2.0f;
}
