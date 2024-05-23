#include "GameScene.h"

// 積
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);
// スカラ
Matrix4x4 MakeScalematrix(const Vector3& scale);
// 移動
Matrix4x4 MakeTranslateMatrix(const Vector3& translate);
// x軸
Matrix4x4 MakeRotateXMatrix(float radian);
// Y軸
Matrix4x4 MakeRotateYMatrix(float radian);
// Z軸
Matrix4x4 MakeRotateZMatrix(float radian);
// SRTの合成
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);