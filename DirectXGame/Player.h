#pragma once
#include"Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

class MapChipField;

class Enemy;
#ifndef STRUCT_H
#define STRUCT_H
struct AABB {
	Vector3 min;
	Vector3 max;
};
#endif

class Player
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model"></param>
	/// <param name="viewProjection"></param>
	/// <param name="position"></param>
	void Initalize(Model* model,ViewProjection* viewProjection,const Vector3& position);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	
	enum class LRDirection
	{
		kRight,
		kLeft,
	};

	WorldTransform& GetWorldTransform();

	const Vector3& GetVelocity() const { return velocity_; }


	/// <summary>
	/// 外部からポインタを呼ぶ
	/// </summary>
	/// <param name="mapChipField"></param>
	void SetMapChipField(MapChipField* mapChipField);

	/// <summary>
	/// マップチップとの当たり判定情報
	/// </summary>
	struct  CollisonMapInfo {
		//天井のフラグ
		bool ceilingCollisionFlag=false;
		//着地フラグ
		bool landingFlag=false;
		//壁接触フラグ
		bool wallContactFlag=false;
		//移動量
		Vector3 move = {};
	};

	void mapCollision(CollisonMapInfo& info);

	/// <summary>
	/// マップ衝突の四方向の関数
	/// </summary>
	/// <param name="collisonMapInfo"></param>
	void mapCollisionDetectionUp(CollisonMapInfo* collisonMapInfoUp);
	void mapCollisionDetectionDown(CollisonMapInfo* collisonMapInfoDown);
	void mapCollisionDetectionLeft(CollisonMapInfo* collisonMapInfoLeft);
	void mapCollisionDetectionRight(CollisonMapInfo* collisonMapInfoRight);

	//四つの核の座標計算
	//右下、左下、右上、左上

	enum Corner {
		kRightBottom,
		kLeftBottom,
		kRightTop,
		kLeftTop,

		//要素数
		kNumCorner,
	};

	
	Vector3 CornnerPosition(const Vector3& center, Corner corner);


	/// <summary>
	/// 判定結果を反映する
	/// </summary>
	/// <param name="info"></param>
	void Reflection(const CollisonMapInfo& info);

	/// <summary>
	/// 天井に接触する
	/// </summary>
	void ceiling(const CollisonMapInfo& info);

	/// <summary>
	/// 着地状態の切り替えの処理
	/// </summary>
	void landing(const CollisonMapInfo& info);

	/// <summary>
	/// 壁に接触している場合の処理
	/// </summary>
	void wallContact(const CollisonMapInfo& info);

	float easeInOut(float x1, float x2, float t);
	float Lerp(float x1, float x2, float t);

	/// <summary>
	/// 自キャラのワールド座標
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldPosition();
	/// <summary>
	/// AABB取得関数
	/// </summary>
	/// <returns></returns>
	AABB GetAABB();
		
	/// <summary>
	///　衝突判定
	/// </summary>
	void OnCollision(const Enemy* enemy);

private:
	WorldTransform worldTransform_;
	ViewProjection* viewProjection_;
	LRDirection lrDirection_ = LRDirection::kRight;

	Vector3 velocity_ = {};
	static inline const float kAceeleration = 0.01f;
	Model* playerModel_=nullptr;
	static inline const float kAttenuation = 0.1f;
	static inline const float kLimitRunSpeed = 0.2f;

	//旋回開始時の角度
	float turnFirstRotationY_ = 0.0f;
	//旋回タイマー
	float turnTimer_=0.0f;
	//旋回時間
	static inline const float kTimeTurn = 0.3f;

	bool onGround_ = true;

	static inline const float kGravityAcceleration = 0.05f;
	static inline const float kLimitFallSpeed = 0.5f;
	static inline const float kJumpAcceleration = 0.7f;
	
	static inline const float kBlack = 1.0f;

	MapChipField* mapChipField_ = nullptr;


	// キャラクターの当たり判定のサイズ
	static inline const float kWidth = 1.6f;
	static inline const float kHeight = 1.6f;


	//着磁の速度減少率
	static inline const float kAttenuationLanding = 0.1f;

	//マップチップとの隙間
	static inline const float kGaq = 0.6f;

	//着磁の速度減衰率
	static inline const float kAtteuationWall = 0.2f;
};

