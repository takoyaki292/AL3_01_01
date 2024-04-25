#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() { 
	delete modelBlock_;
	//ブロック用のワールドトランスフォームを開放する
	for (WorldTransform* worldTransformBlock: worldTransformBlocks_)
	{
			delete worldTransformBlock;
	}
	worldTransformBlocks_.clear();
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	textureHandle_ = TextureManager::Load("uvChecker.png");
	modelBlock_ = Model::Create();


	//要素数
	const uint32_t kNumBlockHorizontal = 20;
	//ブロック一個分の横幅
	const float kBlockWidth = 2.0f;
	//ここで要素数を変更
	worldTransformBlocks_.reserve(kNumBlockHorizontal);

	//拡張for文
	//
	//ここでキューブを作成する
	for (uint32_t i = 0; i < kNumBlockHorizontal; ++i)
	{
		worldTransformBlocks_[i]=new WorldTransform;
		worldTransformBlocks_[i]->Initialize();
		worldTransformBlocks_[i]->translation_.x = kBlockWidth * i;
		worldTransformBlocks_[i]->translation_.y = 0.0f;
	}

	viewProjection_.Initialize();
}

void GameScene::Update()
{
	//ブロックの更新
	for (WorldTransform* worldTransformBlock : worldTransformBlocks_)
	{
		//スケーリング行列に変換
		worldTransformBlock->scale_;
		//回転行列に変換
		worldTransformBlock->rotation_.x;
		worldTransformBlock->rotation_.y;
		worldTransformBlock->rotation_.z;
		//回転行列の合成
		worldTransformBlock->rotation_=
		(worldTransformBlock->rotation_.z * worldTransformBlock->rotation_.x *
		 worldTransformBlock->rotation_.y);

		//平行移動行列の作成
		worldTransformBlock->translation_;
		//マットワールドに入れ込む(スクーリング行列,回転行列、平行移動行列)
		worldTransformBlock->matWorld_ = worldTransformBlock->scale_*
		                                 worldTransformBlock->rotation_ *
		                                 worldTransformBlock->translation_;
		worldTransformBlock->TransferMatrix();                
	}
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	for (WorldTransform* worldTransformBlock : worldTransformBlocks_)
	{
		modelBlock_->Draw(*worldTransformBlock, viewProjection_);
	}
	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
