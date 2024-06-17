#pragma once
#include"Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Model.h"

class MapChipField;

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
		bool ceilingCollisionFlag=false;
		bool landingFlag=false;
		bool wallContactFlag=false;
		Vector3 move = {};
	};

	void mapCollision(CollisonMapInfo& info);

	/// <summary>
	/// マップ衝突の四方向の関数
	/// </summary>
	/// <param name="collisonMapInfo"></param>
	void mapCollisionDetectionUp(CollisonMapInfo* collisonMapInfoUp);
	//void mapCollisionDetectionDown(CollisonMapInfo* collisonMapInfoDown);
	//void mapCollisionDetectionLeft(CollisonMapInfo* collisonMapInfoLeft);
	//void mapCollisionDetectionRight(CollisonMapInfo* collisonMapInfoRight);

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

	//キャラクターの当たり判定のサイズ
	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;

	/// <summary>
	/// 判定結果を反映する
	/// </summary>
	/// <param name="info"></param>
	void Reflection(const CollisonMapInfo& info);

	/// <summary>
	/// 天井に接触する
	/// </summary>
	void ceiling(const CollisonMapInfo& info);


private:
	WorldTransform worldTransform_;
	ViewProjection* viewProjection_;
	LRDirection lrDirection_ = LRDirection::kRight;

	Vector3 velocity_ = {};
	static inline const float kAceeleration = 0.01f;
	Model* playerModel_ ;
	static inline const float kAttenuation = 0.1f;
	static inline const float kLimitRunSpeed = 0.2f;

	//旋回開始時の角度
	float turnFirstRotationY_ = 0.0f;
	//旋回タイマー
	float turnTimer_=0.0f;
	//旋回時間
	static inline const float kTimeTurn = 0.3f;

	bool onGround_ = true;

	static inline const float kGravityAcceleration = 1.0f;
	static inline const float kLimitFallSpeed = 2.0f;
	static inline const float kJumpAcceleration = 2.0f;

	static inline const float kAttenuationLanding = 1.0f;
	
	static inline const float kBlack = 0.0f;
	//bool upKey_ = false;
	

	MapChipField* mapChipField_ = nullptr;

};
