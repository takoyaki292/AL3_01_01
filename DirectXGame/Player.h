#pragma once
#include"Model.h"
class Player
{
	public:
	/// <summary>
	/// ‰Šú‰»
	/// </summary>
	/// <param name="model"></param>
	/// <param name="viewProjection"></param>
	/// <param name="position"></param>
	void Initalize(Model* model,ViewProjection* viewProjection,const Vector3&position);

	/// <summary>
	/// –ˆƒtƒŒ[ƒ€ˆ—
	/// </summary>
	void Update();

	/// <summary>
	/// •`‰æˆ—
	/// </summary>
	void Draw();

	WorldTransform* worldTransform_;
	private:
};
