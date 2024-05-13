#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "WorldTransform.h"

GameScene::GameScene() {}

GameScene::~GameScene() { 
	delete modelBlock_;

	//ブロック用のワールドトランスフォームを開放する
	//for (WorldTransform* worldTransformBlock: worldTransformBlocks_)
	//{
	//		delete worldTransformBlock;
	//}
	for (std::vector<WorldTransform*>worldTransformBlockLine : worldTransformBlocks_)
	{
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine)
		{
			delete worldTransformBlock;
		}
	}
	//worldTransformBlocks_.clear();

	delete debugCamera_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	textureHandle_ = TextureManager::Load("uvChecker.png");
	modelBlock_ = Model::Create();


	//要素数
	const uint32_t kNumBlockVirtical = 10;
	const uint32_t kNumBlockHorizontal = 20;
	//ブロック一個分の横幅
	const float kBlockWidth = 2.0f;
	const float kBlockHeight = 2.0f;
	//ここで要素数を変更
	//worldTransformBlocks_.resize(kNumBlockHorizontal);
	worldTransformBlocks_.resize(kNumBlockVirtical);
	for (uint32_t i = 0; i < kNumBlockVirtical; ++i)
	{
		worldTransformBlocks_[i].resize(kNumBlockHorizontal);
	}
	//拡張for文
	//
	//ここでキューブを作成する
	//for (uint32_t i = 0; i < kNumBlockHorizontal; ++i)
	//{
	//	worldTransformBlocks_[i]=new WorldTransform;
	//	worldTransformBlocks_[i]->Initialize();
	//	worldTransformBlocks_[i]->translation_.x = kBlockWidth * i;
	//	worldTransformBlocks_[i]->translation_.y = 0.0f;
	//}
	for (uint32_t i = 0; i < kNumBlockVirtical; ++i)
	{
		for (uint32_t j = 0; j < kNumBlockHorizontal; ++j)
		{
			
			{
				worldTransformBlocks_[i][j] = new WorldTransform();
				worldTransformBlocks_[i][j]->Initialize();
				worldTransformBlocks_[i][j]->translation_.x = kBlockWidth *j;
				worldTransformBlocks_[i][j]->translation_.y = kBlockHeight * i;

			}
		}
	}
	viewProjection_.Initialize();

	debugCamera_ = new DebugCamera(1280, 720);
}

void GameScene::Update()
{
	//ブロックの更新
	for (std::vector<WorldTransform*> worldTransformBlockLine : worldTransformBlocks_) 
	{
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) 
		{
			if (!worldTransformBlock)
			{
				continue;	
			}
			worldTransformBlock->matWorld_ = worldTransformBlock->MakeAffineMatrix(
			    worldTransformBlock->scale_, worldTransformBlock->rotation_,
			    worldTransformBlock->translation_);
			worldTransformBlock->TransferMatrix();
		}
	}

	debugCamera_->Update();
//#ifdef DEBUG
//	if (Input_->TriggrKey())
//#endif // DEBUG
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
	
	//for (std::vector<WorldTransform*> WorldTransformLine : worldTransformBlocks_)
	//{
	//	for (WorldTransform* worldTransformBlock : WorldTransformLine	)
	//	{
	//		modelBlock_->Draw(*worldTransformBlock, viewProjection_);
	//	}
	//
	//}
	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	/// ブロックの描画

	for (std::vector<WorldTransform*> worldTransformBlockLine : worldTransformBlocks_) 
	{
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) 
		{
			if (!worldTransformBlock)
			{
				continue;
			}
			modelBlock_->Draw(*worldTransformBlock, viewProjection_);
		}
	}
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
