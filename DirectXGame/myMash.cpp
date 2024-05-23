#include "myMash.h"
// 積
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result{};
	for (int I = 0; I < 4; I++) {
		for (int J = 0; J < 4; J++) {
			result.m[I][J] = m1.m[I][0] * m2.m[0][J] + m1.m[I][1] * m2.m[1][J] + m1.m[I][2] * m2.m[2][J] + m1.m[I][3] * m2.m[3][J];
		}
	}
	return result;
}
// スカラ
Matrix4x4 MakeScalematrix(const Vector3& scale) {
	Matrix4x4 result = {0};
	result.m[0][0] = scale.x;
	result.m[1][1] = scale.y;
	result.m[2][2] = scale.z;
	result.m[3][3] = 1;

	return result;
}
// 移動
Matrix4x4 MakeTranslateMatrix(const Vector3& translate) {
	Matrix4x4 result = {0};
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (i == j) {
				result.m[i][j] = 1;
			}
		}
	}
	result.m[3][0] = translate.x;
	result.m[3][1] = translate.y;
	result.m[3][2] = translate.z;
	return result;
}
// x軸
Matrix4x4 MakeRotateXMatrix(float radian) {
	float cosTheta = std::cos(radian);
	float sinTheta = std::sin(radian);
	Matrix4x4 result{0};
	result.m[0][0] = 1.0f;
	result.m[1][1] = cosTheta;
	result.m[1][2] = sinTheta;
	result.m[2][1] = -sinTheta;
	result.m[2][2] = cosTheta;
	result.m[3][3] = 1.0f;

	return result;
}
// Y軸
Matrix4x4 MakeRotateYMatrix(float radian) {
	float cosTheta = std::cos(radian);
	float sinTheta = std::sin(radian);
	Matrix4x4 result{0};
	result.m[1][1] = 1.0f;
	result.m[0][0] = cosTheta;
	result.m[2][0] = sinTheta;
	result.m[0][2] = -sinTheta;
	result.m[2][2] = cosTheta;
	result.m[3][3] = 1.0f;

	return result;
}
// Z軸
Matrix4x4 MakeRotateZMatrix(float radian) {
	float cosTheta = std::cos(radian);
	float sinTheta = std::sin(radian);
	Matrix4x4 result{0};
	result.m[2][2] = 1.0f;
	result.m[0][0] = cosTheta;
	result.m[0][1] = sinTheta;
	result.m[1][0] = -sinTheta;
	result.m[1][1] = cosTheta;
	result.m[3][3] = 1.0f;

	return result;
}
// SRTの合成
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {
	Matrix4x4 rotateXMatrix = MakeRotateXMatrix(rotate.x);
	Matrix4x4 rotateYMatrix = MakeRotateYMatrix(rotate.y);
	Matrix4x4 rotateZMatrix = MakeRotateZMatrix(rotate.z);
	Matrix4x4 rotateXYZMatrix = Multiply(rotateXMatrix, Multiply(rotateYMatrix, rotateZMatrix));
	return Multiply(Multiply(MakeScalematrix(scale), rotateXYZMatrix), MakeTranslateMatrix(translate));
}