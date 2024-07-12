#pragma once
#include "Player.h"
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include<array>
#include "math.h"
#include "DeathObject.h"
class DeathParticles {
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	DeathParticles();
	/// <summary>
	/// デストラクタ
	/// </summary>
	~DeathParticles();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initalize(Model* model, ViewProjection* viewProjection, const Vector3& position);
	/// <summary>
	/// 処理
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private:
	Matrix4x4 MakeRotateZMatrix(float radian);
	Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);
	Model* deathParticlesModel_ = nullptr;
	//パーティクルの個数
	static inline const uint32_t kNumParticles = 8;
	//個数分worldTransformを新しくつくる
	std::array<WorldTransform, kNumParticles> worldTransforms_;
	ViewProjection* viewProjection_;
	Vector3 velocity_ = {};
	Player* deathParticlesPlayer_ = nullptr;

	//消滅までの時間
	static inline const float kDuration = 20.0f;
	//移動の速さ
	static inline const float kSpped = 0.1f;
	//// π
	const double PI = 3.141593;

	//ここの処理ができていない
	//分割した一個分の角度
	static inline float kAngleUnit=kNumParticles;

	//終了フラグ
	bool isFinished_ = false;
	//経過時間カウント
	float counter_ = 0.0f;

	ObjectColor objectColor_={};
	Vector4 color_;
};