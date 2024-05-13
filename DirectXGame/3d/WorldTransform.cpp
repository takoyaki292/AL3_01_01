#include "WorldTransform.h"
#include <cmath>


void WorldTransform::UpdateMatrix() 
{
	matWorld_ = MakeAffineMatrix(scale_, rotation_, translation_);
	TransferMatrix();
}

Matrix4x4 WorldTransform::Mu(Matrix4x4 matrix1, Matrix4x4 matrix2) 
{ 
	Matrix4x4 result{};

	result.m[0][0] = matrix1.m[0][0] * matrix2.m[0][0] + matrix1.m[0][1] * matrix2.m[1][0] +
	                 matrix1.m[0][2] * matrix2.m[2][0] + matrix1.m[0][3] * matrix2.m[3][0];
	result.m[0][1] = matrix1.m[0][0] * matrix2.m[0][1] + matrix1.m[0][1] * matrix2.m[1][1] +
	                 matrix1.m[0][2] * matrix2.m[2][1] + matrix1.m[0][3] * matrix2.m[3][1];
	result.m[0][2] = matrix1.m[0][0] * matrix2.m[0][2] + matrix1.m[0][1] * matrix2.m[1][2] +
	                 matrix1.m[0][2] * matrix2.m[2][2] + matrix1.m[0][3] * matrix2.m[3][2];
	result.m[0][3] = matrix1.m[0][0] * matrix2.m[0][3] + matrix1.m[0][1] * matrix2.m[1][3] +
	                 matrix1.m[0][2] * matrix2.m[2][3] + matrix1.m[0][3] * matrix2.m[3][3];

	result.m[1][0] = matrix1.m[1][0] * matrix2.m[0][0] + matrix1.m[1][1] * matrix2.m[1][0] +
	                 matrix1.m[1][2] * matrix2.m[2][0] + matrix1.m[1][3] * matrix2.m[3][0];
	result.m[1][1] = matrix1.m[1][0] * matrix2.m[0][1] + matrix1.m[1][1] * matrix2.m[1][1] +
	                 matrix1.m[1][2] * matrix2.m[2][1] + matrix1.m[1][3] * matrix2.m[3][1];
	result.m[1][2] = matrix1.m[1][0] * matrix2.m[0][2] + matrix1.m[1][1] * matrix2.m[1][2] +
	                 matrix1.m[1][2] * matrix2.m[2][2] + matrix1.m[1][3] * matrix2.m[3][2];
	result.m[1][3] = matrix1.m[1][0] * matrix2.m[0][3] + matrix1.m[1][1] * matrix2.m[1][3] +
	                 matrix1.m[1][2] * matrix2.m[2][3] + matrix1.m[1][3] * matrix2.m[3][3];

	result.m[2][0] = matrix1.m[2][0] * matrix2.m[0][0] + matrix1.m[2][1] * matrix2.m[1][0] +
	                 matrix1.m[2][2] * matrix2.m[2][0] + matrix1.m[2][3] * matrix2.m[3][0];
	result.m[2][1] = matrix1.m[2][0] * matrix2.m[0][1] + matrix1.m[2][1] * matrix2.m[1][1] +
	                 matrix1.m[2][2] * matrix2.m[2][1] + matrix1.m[2][3] * matrix2.m[3][1];
	result.m[2][2] = matrix1.m[2][0] * matrix2.m[0][2] + matrix1.m[2][1] * matrix2.m[1][2] +
	                 matrix1.m[2][2] * matrix2.m[2][2] + matrix1.m[2][3] * matrix2.m[3][2];
	result.m[2][3] = matrix1.m[2][0] * matrix2.m[0][3] + matrix1.m[2][1] * matrix2.m[1][3] +
	                 matrix1.m[2][2] * matrix2.m[2][3] + matrix1.m[2][3] * matrix2.m[3][3];

	result.m[3][0] = matrix1.m[3][0] * matrix2.m[0][0] + matrix1.m[3][1] * matrix2.m[1][0] +
	                 matrix1.m[3][2] * matrix2.m[2][0] + matrix1.m[3][3] * matrix2.m[3][0];
	result.m[3][1] = matrix1.m[3][0] * matrix2.m[0][1] + matrix1.m[3][1] * matrix2.m[1][1] +
	                 matrix1.m[3][2] * matrix2.m[2][1] + matrix1.m[3][3] * matrix2.m[3][1];
	result.m[3][2] = matrix1.m[3][0] * matrix2.m[0][2] + matrix1.m[3][1] * matrix2.m[1][2] +
	                 matrix1.m[3][2] * matrix2.m[2][2] + matrix1.m[3][3] * matrix2.m[3][2];
	result.m[3][3] = matrix1.m[3][0] * matrix2.m[0][3] + matrix1.m[3][1] * matrix2.m[1][3] +
	                 matrix1.m[3][2] * matrix2.m[2][3] + matrix1.m[3][3] * matrix2.m[3][3];
	//	result.m[3][3] = 5;

	return result;

}
Matrix4x4 WorldTransform::MakeAffineMatrix(
    const Vector3& scale, const Vector3& rotate, const Vector3& translate) 
{
	Matrix4x4 s = 
	{
		scale.x, 0, 0, 0,
		0, scale.y, 0, 0,
		0, 0, scale.z, 0,
		0, 0, 0, 1
	};
	Matrix4x4 rotateX = 
	{
		1,0,0,0,
		0,std::cos(rotate.x),std::sin(rotate.x),0,
		0,std::sin(-rotate.x),std::cos(rotate.x),0,
		0,0,0,1
	};

	Matrix4x4 rotateY = 
	{
		std::cos(rotate.y), 0, std::sin(-rotate.y), 0, 
		0, 1, 0, 0,
	    std::sin(rotate.y), 0, std::cos(rotate.y),  0, 
		0, 0, 0, 1
	};

	Matrix4x4 rotateZ = 
	{
		std::cos(rotate.z),std::sin(rotate.z),0,0,
		std::sin(-rotate.z),std::cos(rotate.z),0,0,
		0,0,1,0,
		0,0,0,1
	};

	Matrix4x4 t=
	{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0, 
		translate.x, translate.y, translate.z, 1
	};
	Matrix4x4 r = {};
	r = Mu(rotateX, Mu(rotateY, rotateZ));
	Matrix4x4 sr = Mu(s, r);
	Matrix4x4 srt = Mu(sr, t);
	return srt;
}