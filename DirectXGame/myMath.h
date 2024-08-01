#include <Matrix4x4.h>
#include <Vector3.h>
#include <cmath>
// アフィン変換行列の作成
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rot, const Vector3& translate);

// 行列の掛け算
Matrix4x4 MatrixMultiply(Matrix4x4& m1, Matrix4x4& m2);

class myMath {
public:
	static Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);
	static Matrix4x4 MakeTranslateMatrix(const Vector3& translate);
	static Matrix4x4 MakeScaleMatrix(const Vector3& scale);
	static Matrix4x4 MakeRotateXMatrix(float radius);
	static Matrix4x4 MakeRotateYMatrix(float radius);
	static Matrix4x4 MakeRotateZMatrix(float radius);
	static Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);
	static Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);
};