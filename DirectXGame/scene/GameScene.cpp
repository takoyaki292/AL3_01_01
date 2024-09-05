#include "GameScene.h"
#include "CameraController.h"
#include "Enemy.h"
#include "Player.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

void GameScene::GenerateBlocks() {
	uint32_t numBlockVirtical = mapChipField_->GetNumBlockVirtical();
	uint32_t numBlockHorizontal = mapChipField_->GetNumBlockHorizontal();
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

void GameScene::CheckAllCollisios() { 
	//AABB型を二つ作る
	//自キャラと敵キャラを作る
	AABB aabb1, aabb2;
	//自キャラの座標
	aabb1 = player_->GetAABB();
	
	for (Enemy* enemy : enemies_){
		aabb2 = enemy->GetAABB();
		if (IsCollision(aabb1, aabb2))
		{
			player_->OnCollision(enemy);
			enemy->OnCollisiton(player_);
		}
	}
	//DebugText::GetInstance()->ConsolePrintf("enemy ceiling\n\n");
}

bool GameScene::IsCollision(AABB a, AABB b) { 
	
	bool isF = false;
	if (a.max.x>b.min.x && a.min.x<b.max.x 
		&& a.max.y > b.min.y && a.min.y < b.max.y
		&& a.max.z > b.min.z && a.min.z < b.max.z) {
		isF = true;
	}
	return isF;
}

GameScene::~GameScene() {
	delete mapChipField_;
	delete debugCamera_;
	delete player_;
	delete cameraController_;
	// delete enemy_;
	for (Enemy* kEnemy : enemies_) {
		delete kEnemy;
		// delete newEnemy;
	}

	//delete deathParticle_;
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
	// Vector3型でポジションを初期化する
	Vector3 playerPosition = mapChipField_->GetMapChipPositionByPlayerIndex(
	    mapChipField_->GetNumBlockHorizontal(), mapChipField_->GetNumBlockVirtical());
	// モデルプレイヤーの読み込む
	modelPlayer_ = Model::CreateFromOBJ("playerModel", true);
	// プレイヤーの初期化
	player_->Initalize(modelPlayer_, &viewProjection_, playerPosition);

	player_->SetMapChipField(mapChipField_);

	/// カメラコントロールの初期化
	cameraController_ = new CameraController(); // カメラコントローラを生成
	cameraController_->Initialize(&viewProjection_); // ビュープロジェクションを渡して初期化
	cameraController_->SetTarget(player_); // ターゲットとしてプレイヤーを設定
	cameraController_->Reset();            // カメラの状態をリセット
	cameraController_->SetMovebleArea({0, 500, 0, 70}); // カメラの移動範囲を指定

	modelEnemy_ = Model::CreateFromOBJ("playerModel", true);
	for (uint32_t i = 0; i < 1; ++i) {
		Enemy* newEnemy = new Enemy();
		Vector3 enemyPosition = {0.f+ 4 * i, 2.f, 0};
		newEnemy->Initalize(modelEnemy_, &viewProjection_, enemyPosition);
		enemies_.push_back(newEnemy);
	}
	////パーティクルをnewする
	//deathParticle_ = new DeathParticles();
	//// モデルプレイヤーの読み込む
	//modelDeathParticles_ = Model::CreateFromOBJ("playerModel", true);
	////デスパーティクルを初期化する
	//deathParticle_->Initalize(modelDeathParticles_,&viewProjection_,playerPosition);
}

void GameScene::Update() {

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
	for (Enemy* enemy : enemies_) {
		if (!enemy) {
			continue;
		}
		else
		{
			enemy->Update();
			CheckAllCollisios();
		}
	}
	player_->Update();

	cameraController_->Update();
	//if (isDeachPaticled == true)
	//{
	//	// パーティクルの更新処理
	//	deathParticle_->Update();
	//}
#ifdef _DEBUG
	if (input_->TriggerKey(DIK_BACK)) {
		isDebugCameraActive_ = true;
	}
	// if (input_->TriggerKey(DIK_O))
	//{
	//	player_->Update();
	// }
#endif // DEBUG
	debugCamera_->Update();
	if (isDebugCameraActive_) {
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
	} else {
		viewProjection_.UpdateMatrix();
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

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	player_->Draw();
	for (Enemy* enemy : enemies_) {
		if (!enemy) {
			continue;
		}
		enemy->Draw();
	}
	for (std::vector<WorldTransform*> worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock) {
				continue;
			}
			modelBlock_->Draw(*worldTransformBlock, viewProjection_);
		}
	}

	//if (isDeachPaticled == true)
	//{
	//	deathParticle_->Draw();
	//}
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
