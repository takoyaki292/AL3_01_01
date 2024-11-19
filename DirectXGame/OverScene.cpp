#include "OverScene.h"

OverScene::OverScene() {}

OverScene::~OverScene() { delete overModel_; }

void OverScene::Initalize() {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	model = Model::CreateFromOBJ("GameOverScene", true);

	overModel_ = new TitleModel();
	Vector3 overPosition = {0.f, 0.f, 0.0f};
	viewProjection_.Initialize();
	overModel_->Initalize(model, &viewProjection_, overPosition);
}

void OverScene::Update() {
	if (Input::GetInstance()->TriggerKey(DIK_SPACE) &&
	    Input::GetInstance()->PushKey(DIK_SPACE)) {
		finished_ = true;
	}
	overModel_->Update();
}

void OverScene::Draw() 
{
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
	overModel_->Draw();
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
