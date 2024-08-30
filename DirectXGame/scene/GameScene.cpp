#include "GameScene.h"
#include "CameraController.h"
#include "Enemy.h"
#include "Player.h"
#include "MoveEnemy.h"
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
				currentChipType = MapChipType::kBlock;
			}

			if (mapChipField_->GetMapChipTypeByIndex(j, i) == MapChipType::kMoveBlock) {
				WorldTransform* worldTransform = new WorldTransform();
				worldTransform->Initialize();
				worldTransformBlocks_[i][j] = worldTransform;
				worldTransformBlocks_[i][j]->translation_ =
				    mapChipField_->GetMapChipPositionByIndex(j, i);
				currentMapChipType = MapChipType ::kMoveBlock;
			}
		}
	}
}

void GameScene::CheckAllCollisios() { 
	//AABB型を二つ作る
	//自キャラと敵キャラを作る
	AABB aabb1, aabb2,aabb3,aabb4;
	//自キャラの座標
	aabb1 = player_->GetAABB();
	
	for (Enemy* enemy : enemies_){
		aabb2 = enemy->GetAABB();
		
		if (IsCollision(aabb1, aabb2))
		{
			player_->OnCollision(enemy);
			enemy->OnCollisiton(player_);
		}
		for (Bullet* bullet : bullets_)
		{
			aabb3 = bullet->GetAABB();
			if (IsCollision(aabb1, aabb3))
			{
				player_->OnCollisionBullet(bullet);
				bullet->OnCollisiton(player_);
			}
		}
	}

	for (MoveEnemy* moveEnemy : moveEnemies_)
	{
		aabb4 = moveEnemy->GetAABB();
		if (IsCollision(aabb1, aabb4)) {
			player_->OnCollisionMoveEnemy(moveEnemy);
			moveEnemy->OnCollisiton(player_);
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
	for (Bullet* kBullet : bullets_)
	{
		delete kBullet;
	}
	for (MoveEnemy* kMoveEnemy : moveEnemies_)
	{
		delete kMoveEnemy;
	}
	delete skydome_;
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
	modelTBlock_ = Model::CreateFromOBJ("moveCube",true);

	playerWorldTransform_.Initialize();

	player_ = new Player();
	//// Vector3型でポジションを初期化する
	Vector3 playerPosition = mapChipField_->GetMapChipPositionByPlayerIndex(
	    mapChipField_->GetNumBlockHorizontal(), mapChipField_->GetNumBlockVirtical());
	// モデルプレイヤーの読み込む
	modelPlayer_ = Model::CreateFromOBJ("playerModel", true);
	// プレイヤーの初期化
	player_->Initalize(modelPlayer_, &viewProjection_, playerPosition);

	player_->SetMapChipField(mapChipField_);

	/// カメラコントロールの初期化
	cameraController_ = new CameraController();
	cameraController_->Initialize(&viewProjection_);
	cameraController_->SetTarget(player_);
	cameraController_->Reset();
	cameraController_->SetMovebleArea({0, 500, 0, 70});

	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	// 天球の生成
	skydome_ = new Skydome();
	// 天球の初期化
	skydome_->Initialize(modelSkydome_, &viewProjection_);


	modelEnemy_ = Model::CreateFromOBJ("enemyModel", true);
	modelEnemyBullet_ = Model::CreateFromOBJ("enemyBulletModel", true);
	modelMoveEnemy_ = Model::CreateFromOBJ("moveEnemyModel", true);
	lifetime = 700.f;
	//張り付いてくる敵
	for (uint32_t i = 0; i <6; ++i) {
		if (i % 2 == 0)
		{
			bulletSpeed = 0.005f;
		} else {
			bulletSpeed = 0.010f;
		}
		Enemy* newEnemy = new Enemy();
		Bullet* newBullet = new Bullet();
		Vector3 enemyPosition = {16.f+ 16* i, 28.f, 0};
		Vector3 bulletPosition = {16.f+ 16 * i, 26.f, 0};
		newEnemy->Initalize(modelEnemy_, &viewProjection_, enemyPosition);
		newBullet->Initalize(modelEnemyBullet_, &viewProjection_, bulletPosition,lifetime,bulletSpeed);
		enemies_.push_back(newEnemy);
		bullets_.push_back(newBullet);
	}

	for (uint32_t i = 0; i < 3; i++)
	{
		MoveEnemy* newMoveEnemy = new MoveEnemy();
		Vector3 moveEnemyPosition = {36.f + 30 * i, 2.f, 0};
		newMoveEnemy->Initalize(modelMoveEnemy_, &viewProjection_, moveEnemyPosition);
		moveEnemies_.push_back(newMoveEnemy);
	}
	////パーティクルをnewする
	deathParticle_ = new DeathParticles();
	// モデルプレイヤーの読み込む
	modelDeathParticles_ = Model::CreateFromOBJ("playerModel", true);
	//デスパーティクルを初期化する
	deathParticle_->Initalize(modelDeathParticles_,&viewProjection_,playerPosition);
}

void GameScene::Update() {
	skydome_->Update();
	for (std::vector<WorldTransform*> worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock) {
				continue;
			}
			else
			{
				worldTransformBlock->matWorld_ = worldTransformBlock->MakeAffineMatrix(
				    worldTransformBlock->scale_, worldTransformBlock->rotation_,
				    worldTransformBlock->translation_);
				worldTransformBlock->TransferMatrix();
			}
			
		}
	}
	//上に張り付いている敵
	for (Enemy* enemy : enemies_) {
		if (!enemy) {
			continue;
		}
		else
		{
			for (Bullet* bullet : bullets_) {
				if (!bullet) {
					continue;
				} else {
					bullet->Update();
				}
			}
			enemy->Update();
			CheckAllCollisios();
		}
		
	}
	for (MoveEnemy* moveEnemy : moveEnemies_) {
		if (!moveEnemy) {
			continue;
		} else {	
			moveEnemy->Update();
			//CheckAllCollisios();
		}
	}

	player_->Update();

	//cameraController_->Update();
	if (player_->isAlive == false)
	{
		// パーティクルの更新処理
		deathParticle_->Update();
	} 
	else {
	
		//Vector3 playerPosition = player_->v;
		//// デスパーティクルを初期化する
		//deathParticle_->Initalize(modelDeathParticles_, &viewProjection_,playerPosition );
	}
	if (Input::GetInstance()->PushKey(DIK_1)) {
		finished_ = true;
	}
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
	skydome_->Draw();
	player_->Draw();
	for (Enemy* enemy : enemies_) {
		if (!enemy) {
			continue;
		} else {
		enemy->Draw();
		}
		for (Bullet* bullet : bullets_) {
			if (!bullet) {
				continue;
			} else {
				bullet->Draw();
			}
		}
		
	}
	for (MoveEnemy* moveEnemy : moveEnemies_) {
		if (!moveEnemy) {
			continue;
		} else {
			moveEnemy->Draw();
		}
	}

	for (std::vector<WorldTransform*> worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock) {
				continue;
			}
			
			
			//MapChipType currentChipType = MapChipType::kBlock; 
			modelTBlock_->Draw(*worldTransformBlock, viewProjection_);
		}
	}

	if (player_->isAlive == false) {
		deathParticle_->Draw();
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

int GameScene::switching(MapChipType mapChipNumber) {
	int num;
	switch (mapChipNumber) {
	case MapChipType::kBlock:
		num = 1;
		break;
	case MapChipType::kMoveBlock:
		num = 2;
		break;
	// 他の MapChipType が追加された場合の処理をここに追加することができます。
	default:
		num = 0;
		break;
	}
	return num;
}


//bool GameScene::GetPlayer() { isA = player_->isAlive; }
