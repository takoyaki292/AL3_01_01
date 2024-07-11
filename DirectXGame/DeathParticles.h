#pragma once
#include "Player.h"
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include<array>
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
	Model* deathParticlesModel_ = nullptr;
	//パーティクルの個数
	static inline const uint32_t kNumParticles = 8;
	//個数分worldTransformを新しくつくる
	std::array<WorldTransform, kNumParticles> worldTransforms_;
	ViewProjection* viewProjection_;
	Vector3 velocity_ = {};
	Player* deathParticlesPlayer_ = nullptr;
};