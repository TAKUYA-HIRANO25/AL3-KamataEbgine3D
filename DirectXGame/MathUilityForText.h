#include <Vector3.h>

// 2項演算子オーバーロード
// Vextor3の足算
Vector3& operator+=(Vector3& lhv, const Vector3& rhv);

const Vector3 operator+(const Vector3& v1, const Vector3& v2);

Vector3& operator*=(Vector3& v, float s);

const Vector3 operator*(const Vector3& v, float s);

// イージング関数の宣言
float Lerp(float x1, float x2, float t);

Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t);

float EaseInOut(float x1, float x2, float t);
