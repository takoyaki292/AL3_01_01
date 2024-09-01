#include "ExplanationScene.h"
#include <numbers>
#include <TextureManager.h>

ExplanationScene::ExplanationScene() {}

ExplanationScene::~ExplanationScene() {
	delete explanationModel_;
	// delete sprite_;
}

void ExplanationScene::Initalize() {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// 必要なリソースをロード
	model = Model::CreateFromOBJ("ExplanationScene", true); // 操作画面に必要なモデルの読み込み

	explanationModel_ = new TitleModel();
	Vector3 explanationPosition = {0.f, 0.f, 0.0f};
	viewProjection_.Initialize();
	explanationModel_->Initalize(model, &viewProjection_, explanationPosition);
}

void ExplanationScene::Update() {
	if (Input::GetInstance()->TriggerKey(DIK_SPACE) && Input::GetInstance()->PushKey(DIK_SPACE)) {
		finished_ = true;
	}

	explanationModel_->Update();
}

void ExplanationScene::Draw() {
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
	explanationModel_->Draw();
	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	
	// sprite_->Draw();
	//  3Dオブジェクト描画後処理
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