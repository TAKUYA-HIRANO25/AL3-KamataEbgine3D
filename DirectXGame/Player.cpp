#define NOMINMAX
#include "Player.h"
#include <cassert>
#include <numbers>
#include <algorithm>
void Player::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position){
	
	// NULLチェック
	assert(model);

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;
	// 引数の内容をメンバ変数に記録
	model_ = model;

	viewProjection_ = viewProjection;
	
}

void Player::Update() {
	worldTransform_.UpdateMatrix();
	if (onGround_) {
		Vector3 acceleration = {};
		//横移動
		if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
			if (velocity_.x < 0.0f) {
				velocity_.x *= (1.0f - kAcceleration);
			}
			acceleration.x = kAcceleration;
			if (lrDirection_ != LRDirection::kRight) {
				lrDirection_ = LRDirection::kRight;
				trunFirstRotationY = worldTransform_.rotation_.y;
				turnTimer_ = 5.0f;
			}
		} else if (Input::GetInstance()->PushKey(DIK_LEFT)) {
			if (velocity_.x > 0.0f) {
				velocity_.x *= (1.0f - kAcceleration);
			}
			acceleration.x -= kAcceleration;
			if (lrDirection_ != LRDirection::kLeft) {
				lrDirection_ = LRDirection::kLeft;
				trunFirstRotationY = worldTransform_.rotation_.y;
				turnTimer_ = 5.0f;
			}
		} else {
			velocity_.x *= (1.0f - kAttenuation);
		}
		//振り向き
		if (turnTimer_ > 0.0f) {
			turnTimer_ -= 1.0f / 60.0f;

			float destinationRotationYTable[] = {
				std::numbers::pi_v<float> / 2.0f, 
				std::numbers::pi_v<float> * 3.0f / 2.0f
			};
			float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];
			worldTransform_.rotation_.y = ;

		}
		velocity_.x += acceleration.x;
		velocity_.y += acceleration.y;
		velocity_.z += acceleration.z;
		velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);
		//ジャンプ
		if (Input::GetInstance()->PushKey(DIK_UP)) {
			velocity_.y += kJumpAccleration;
		}
	} else {                      //空中
		velocity_.y += kGravityAcceleration;
		velocity_.y = std::max(velocity_.y, -kLimitFallspeed);
	}

	// 行列を定数バッファに転送

	worldTransform_.translation_.x += velocity_.x;
	
	worldTransform_.UpdateMatrix();

}

void Player::Draw(){

	// 3Dモデルを描画
	model_->Draw(worldTransform_, *viewProjection_, textureHandle_);
}