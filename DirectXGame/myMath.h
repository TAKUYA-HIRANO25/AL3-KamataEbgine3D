//#include"GameScene.h"
#include "Matrix4x4.h"
#include "Vector3.h"

// アフィン変換行列の作成
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rot, const Vector3& translate);

// 行列の掛け算
Matrix4x4 MatrixMultiply(Matrix4x4& m1, Matrix4x4& m2);
//演算子
Vector3& operator+=(Vector3& lhv, const Vector3& rhv);
//イージング
float Lerp(float x1, float x2, float t);
Vector3 Lerp2(const Vector3& v1, const Vector3& v2, float t);
float EaseInOut(float x1, float x2, float t);

