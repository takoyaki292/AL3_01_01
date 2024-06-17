#pragma once

/// <summary>
/// 3次元ベクトル
/// </summary>
struct Vector3 final {
	float x;
	float y;
	float z;

	Vector3 operator+=(const Vector3& num) {
		x = this->x + num.x;
		y = this->y + num.y;
		z = this->z + num.z;
		return *this;
	}

	Vector3 operator-=(const Vector3& num) {
		x = this->x - num.x;
		y = this->y - num.y;
		z = this->z - num.z;
		return *this;
	}
};

inline Vector3 operator+(const Vector3& num, const Vector3& twoNum){ 
	Vector3 result;
	result.x = num.x + twoNum.x;
	result.y = num.y + twoNum.y;
	result.z = num.z + twoNum.z;
	return result;
};