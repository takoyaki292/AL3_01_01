#include "TitleScene.h"

void TitleScene::Initalize() {
	titleHandle_=
}

void TitleScene::Update() { 
	if (Input::GetInstance()->PushKey(DIK_SPACE)) {
		finished_ = true;
	}
}

void TitleScene::Draw() {

}
