#include "Audio.h"
#include "AxisIndicator.h"
#include "DirectXCommon.h"
#include "GameScene.h"
#include "ImGuiManager.h"
#include "PrimitiveDrawer.h"
#include "TextureManager.h"
#include "WinApp.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "ClearScene.h"
#include "OverScene.h"
#include "ExplanationScene.h"

GameScene* gameScene = nullptr;
TitleScene* titleScene = nullptr;
OverScene* overScene = nullptr;
ClearScene* clearScene = nullptr;
ExplanationScene* explanationScene = nullptr;
    //シーン
enum class Scene { 
	kUnkown = 0,
	kTitle, 
	kExplanation,
	kGame,
	kOver,
	kClear
};
//現在のシーン
Scene scene = Scene::kUnkown;

//シーン切り替え処理
void ChageScene();

//シーンの更新
void UpdateScene(); 

//シーンの描画
void DrawScene();

    // Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	WinApp* win = nullptr;
	DirectXCommon* dxCommon = nullptr;
	// 汎用機能
	Input* input = nullptr;
	Audio* audio = nullptr;
	AxisIndicator* axisIndicator = nullptr;
	PrimitiveDrawer* primitiveDrawer = nullptr;
	//GameScene* gameScene = nullptr;
	////タイトルシーン
	//TitleScene* titleScene = nullptr;
	// ゲームウィンドウの作成
	win = WinApp::GetInstance();
	win->CreateGameWindow(L"GC2B_08_シミズ_タクミ");

	// DirectX初期化処理
	dxCommon = DirectXCommon::GetInstance();
	dxCommon->Initialize(win);

#pragma region 汎用機能初期化
	// ImGuiの初期化
	ImGuiManager* imguiManager = ImGuiManager::GetInstance();
	imguiManager->Initialize(win, dxCommon);

	// 入力の初期化
	input = Input::GetInstance();
	input->Initialize();

	// オーディオの初期化
	audio = Audio::GetInstance();
	audio->Initialize();

	// テクスチャマネージャの初期化
	TextureManager::GetInstance()->Initialize(dxCommon->GetDevice());
	TextureManager::Load("white1x1.png");

	// スプライト静的初期化
	Sprite::StaticInitialize(dxCommon->GetDevice(), WinApp::kWindowWidth, WinApp::kWindowHeight);

	// 3Dモデル静的初期化
	Model::StaticInitialize();

	// 軸方向表示初期化
	axisIndicator = AxisIndicator::GetInstance();
	axisIndicator->Initialize();

	primitiveDrawer = PrimitiveDrawer::GetInstance();
	primitiveDrawer->Initialize();
#pragma endregion

	// ゲームシーンの初期化
	gameScene = new GameScene();
	gameScene->Initialize();
	//最初のシーンの初期化
	scene = Scene::kTitle;
	//タイトルシーンの初期化
	titleScene = new TitleScene;
	titleScene->Initalize();

	overScene = new OverScene;
	overScene->Initalize();

	clearScene = new ClearScene;
	clearScene->Initalize();

	explanationScene = new ExplanationScene;
	explanationScene->Initalize();

	uint32_t soundDateHandle_ = 0;
	uint32_t voiceHandle_ = 0;

	soundDateHandle_ = audio->LoadWave("GameAudio.mp4");

	audio->PlayWave(soundDateHandle_);
	voiceHandle_ = audio->PlayWave(soundDateHandle_, true);
	// メインループ
	while (true) {
		// メッセージ処理
		if (win->ProcessMessage()) {
			break;
		}

		// ImGui受付開始
		imguiManager->Begin();
		// 入力関連の毎フレーム処理
		input->Update();
		////タイトルシーンの毎フレーム処理
		//titleScene->Update();
		//シーンの切り替え
		ChageScene();
		//現在のシーン更新
		UpdateScene();
		// ゲームシーンの毎フレーム処理
		//gameScene->Update();
		// 軸表示の更新
		axisIndicator->Update();
		// ImGui受付終了
		imguiManager->End();

		// 描画開始
		dxCommon->PreDraw();
		////タイトルシーンの描画
		//titleScene->Draw();
		DrawScene();
		// ゲームシーンの描画
		//gameScene->Draw();
		// 軸表示の描画
		axisIndicator->Draw();
		// プリミティブ描画のリセット
		primitiveDrawer->Reset();
		// ImGui描画
		imguiManager->Draw();
		// 描画終了
		dxCommon->PostDraw();
	}

	// 各種解放
	delete gameScene;
	delete titleScene;
	delete overScene;
	delete clearScene;
	delete explanationScene;
	// 3Dモデル解放
	Model::StaticFinalize();
	audio->Finalize();
	// ImGui解放
	imguiManager->Finalize();

	// ゲームウィンドウの破棄
	win->TerminateGameWindow();

	return 0;
}

void ChageScene() {
	switch (scene) {
	case Scene::kTitle:
		if (titleScene->IsFinished()) {
			if (titleScene->GetSelectedOption() == TitleOption::kStartGame) {
				scene = Scene::kGame;
				delete titleScene;
				titleScene = nullptr;

				gameScene = new GameScene();
				gameScene->Initialize();
			} else if (titleScene->GetSelectedOption() == TitleOption::kExplanation) {
				scene = Scene::kExplanation;
				delete titleScene;
				titleScene = nullptr;

				explanationScene = new ExplanationScene();
				explanationScene->Initalize();
			}
		}
		else if (titleScene->IsEFinished())
		{
			scene = Scene::kExplanation;

			// タイトルシーンの開放
			delete titleScene;
			titleScene = nullptr;

			explanationScene = new ExplanationScene();
			explanationScene->Initalize();
		}

		break;
	case Scene::kExplanation:
		if (explanationScene->IsFinished()) {
			scene = Scene::kTitle;

			// 説明シーンの開放
			delete explanationScene;
			explanationScene = nullptr;

			// タイトルシーンの初期化
			titleScene = new TitleScene();
			titleScene->Initalize();
		}
		break;
	case Scene::kGame:
		
		if (gameScene->IsTimeOver())
		{
			scene = Scene::kClear;

			// ゲームシーンの開放
			delete gameScene;
			gameScene = nullptr;

			clearScene = new ClearScene();
			clearScene->Initalize();
		}
		else if (!gameScene->IsPlayerAlive())
		{
			scene = Scene::kOver;

			// ゲームシーンの開放
			delete gameScene;
			gameScene = nullptr;

			overScene = new OverScene();
			overScene->Initalize();
		}
		
		
		break;
	case Scene::kOver:
		if (overScene->IsFinished()) {
			scene = Scene::kTitle;

			// ゲームシーンの開放
			delete overScene;
			overScene = nullptr;

			titleScene = new TitleScene();
			titleScene->Initalize();
		}
		break;
	case Scene::kClear:
		if (clearScene->IsFinished()) {
			scene = Scene::kTitle;

			// ゲームシーンの開放
			delete clearScene;
			clearScene = nullptr;

			titleScene = new TitleScene();
			titleScene->Initalize();
		}
		break;
	
	}
}

void UpdateScene() {
	switch (scene) {
	case Scene::kTitle:
		titleScene->Update();
		break;
	case Scene::kExplanation:
		explanationScene->Update();
		break;
	case Scene::kGame:
		gameScene->Update();
		break;
	case Scene::kOver:
		overScene->Update();
		break;
	case Scene::kClear:
		clearScene->Update();
		break;
	
	}
}

void DrawScene() {
	switch (scene) {
	case Scene::kTitle:
		titleScene->Draw();
		break;
	case Scene::kExplanation:
		explanationScene->Draw();
		break;
	case Scene::kGame:
		gameScene->Draw();
		break;
	case Scene::kOver:
		overScene->Draw();
		break;
	case Scene::kClear:
		clearScene->Draw();
		break;
	}
}