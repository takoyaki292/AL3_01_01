#include "TitleScene.h"
#include <numbers>
#include <TextureManager.h>

TitleScene::TitleScene() {}

TitleScene::~TitleScene() {
	delete titleModel_;
	//delete sprite_;
}

void TitleScene::Initalize() {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	model = Model::CreateFromOBJ("GameTitleScene", true);
	titleGameModel = Model::CreateFromOBJ("titleGame", true);
	
	titleModel_ = new TitleModel();
	setumeiModel_ = new TitleModel();
	Vector3 titlePosition = {0.f, 0.f, 0.0f};
	viewProjection_.Initialize();
	titleModel_->Initalize(titleGameModel, &viewProjection_, titlePosition);
	setumeiModel_->Initalize(model, &viewProjection_, titlePosition);
}

void TitleScene::Update() {
	if (input_->TriggerKey(DIK_UP)) {
		selectedOption_ = TitleOption::kStartGame;
	} else if (input_->TriggerKey(DIK_DOWN)) {
		selectedOption_ = TitleOption::kExplanation;
	}

	if (Input::GetInstance()->TriggerKey(DIK_SPACE) &&
		    Input::GetInstance()->PushKey(DIK_SPACE)) 
	{
		if (selectedOption_ == TitleOption::kStartGame) {
			finished_ = true;
		} else if (selectedOption_ == TitleOption::kExplanation) {
			eFinished_ = true;
		}
	}
	
	titleModel_->Update();
	setumeiModel_->Update();
}

void TitleScene::Draw() {
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
	if (selectedOption_ == TitleOption::kStartGame) {
		titleModel_->Draw();
	}
	else if (selectedOption_ == TitleOption::kExplanation)
	{
		setumeiModel_->Draw();
	}
	
	
	//sprite_->Draw();
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

TitleOption TitleScene::GetSelectedOption() const { return selectedOption_; }