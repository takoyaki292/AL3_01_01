#pragma once
#include "Input.h"
class TitleScene {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initalize();
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	bool IsFinished() const { return finished_; };
private:
	//終了フラグ
	bool finished_ = false;
	Input* input_ = nullptr;

	uint32_t titleHandle_ = 0;
};