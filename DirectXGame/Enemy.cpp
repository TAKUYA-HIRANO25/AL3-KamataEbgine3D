#define NOMINMAX
#include "Enemy.h"
#include "MathUilityForText.h"
#include <DebugText.h>
#include <Input.h>
#include <algorithm>
#include <cassert>
#include <cmath>
#include <numbers>

void Enemy::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position) {
	// NULLチェック
	assert(model);
	model_ = model;
	viewProjection_ = viewProjection;
	// ワールド変換の初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / -2.0f;

	// 速度の設定
	velocity_ = {-kWalkSpeed, 0, 0};

	walkTimer_ = 0.0f;
}

void Enemy::Update() {
	// 移動
	worldTransform_.translation_ += velocity_;

	// タイマーを加算
	walkTimer_ += 1.0f / 60.0f;
	// 回転アニメーション
	float param = std::sin((2 * std::numbers::pi_v<float>)*walkTimer_ / kWalkMotionTime);
	float radian = kWalkMotionAngleStart + kWalkMotionAngleEnd * (param + 1.0f) / 2.0f;
	worldTransform_.rotation_.x = sinf((radian ) / 2);
	// 行列を定数バッファに転送
	worldTransform_.UpdateMatrix();
}

void Enemy::Draw() {
	model_->Draw(worldTransform_, *viewProjection_); 
}