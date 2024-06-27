#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "Player.h"
#include "CameraController.h"


GameScene::GameScene() {}

void GameScene::GenerateBlocks() {
	uint32_t numBlockVirtical = mapChipField_->GetNumBlockVirtical();
	uint32_t numBlockHorizontal = mapChipField_->GetNumBlockHorizontal();

	// numBlockVirtical = 20;
	// numBlockHorizontal = 100;
	worldTransformBlocks_.resize(numBlockVirtical);
	
	for (uint32_t i = 0; i < numBlockVirtical; ++i) {
		worldTransformBlocks_[i].resize(numBlockHorizontal);
		for (uint32_t j = 0; j < numBlockHorizontal; ++j) {
			if (mapChipField_->GetMapChipTypeByIndex(j, i) == MapChipType::kBlock) {
				WorldTransform* worldTransform = new WorldTransform();
				worldTransform->Initialize();
				worldTransformBlocks_[i][j] = worldTransform;
				worldTransformBlocks_[i][j]->translation_ =
				    mapChipField_->GetMapChipPositionByIndex(j, i);
			}
		}
	}
}


GameScene::~GameScene() {
	delete mapChipField_;
	delete debugCamera_;
	delete player_;
	delete cameraController_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	mapChipField_ = new MapChipField;
	mapChipField_->LoadMapChipCsv("Resources/blocks.csv");

	
	debugCamera_ = new DebugCamera(1280, 720);

	viewProjection_.Initialize();
	
	GenerateBlocks();

	modelBlock_ = Model::Create();

	playerWorldTransform_.Initialize();

	
	player_ = new Player();
	//Vector3型でポジションを初期化する
	Vector3 playerPosition = mapChipField_->GetMapChipPositionByPlayerIndex(
	    mapChipField_->GetNumBlockHorizontal(),mapChipField_->GetNumBlockVirtical());	
	//モデルプレイヤーの読み込む
	modelPlayer_ = Model::CreateFromOBJ("playerModel", true);
	//プレイヤーの初期化
	player_->Initalize(modelPlayer_, &viewProjection_, playerPosition);

	player_->SetMapChipField(mapChipField_);

	///カメラコントロールの初期化
	cameraController_ = new CameraController();
	cameraController_->Initialize(&viewProjection_);
	cameraController_->SetTarget(player_);
	cameraController_->Reset();
	cameraController_->SetMovebleArea({ 0,500, 0,70});
	
}




void GameScene::Update() 
{
	
	for (std::vector<WorldTransform*> worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock) {
				continue;
			}
			worldTransformBlock->matWorld_ = worldTransformBlock->MakeAffineMatrix(
			    worldTransformBlock->scale_, worldTransformBlock->rotation_,
			    worldTransformBlock->translation_);
			worldTransformBlock->TransferMatrix();
		}
	}

	player_->Update();
	//cameraController_->Update();
	
	//	#ifdef _DEBUG
//	if (input_->TriggerKey(DIK_BACK)) {
//		isDebugCameraActive_ = true;
//	}
//	//if (input_->TriggerKey(DIK_O))
//	//{
//	//	player_->Update();
//	//}
//#endif // DEBUG
//	debugCamera_->Update();
//	if (isDebugCameraActive_) {
//		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
//		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
//		viewProjection_.TransferMatrix();
//	} else {
//		viewProjection_.UpdateMatrix();
//	}
//	
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

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	player_->Draw();
	for (std::vector<WorldTransform*> worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock) {
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


