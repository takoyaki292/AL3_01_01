#include "Player.h"
#include "TextureManager.h"
#include <cassert>

Player::Player() {}

Player::~Player() {}

//初期化
void Player::Initialize(Model* model, uint32_t textureHandele, ViewProjection* viewProjection)
{
	assert(model);
	textHandle_ = TextureManager::Load("uvChecker.png");
	model_=model;
	textureHandele = textHandle_;



	//ワールドとビューの初期化
	worldTransform_.Initialize();

	viewProjection_= viewProjection;
}


// 更新
void Player::Update() { 
	//ワールドトランスフォームを定数に変換する
	worldTransform_.TransferMatrix();
}

//描画
void Player::Draw() { 
	model_->Draw(worldTransform_, *viewProjection_, textHandle_);
}

