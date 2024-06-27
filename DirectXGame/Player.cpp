#define NOMINMAX
#include "Player.h"
#include <cassert>
#include <numbers>
#include <algorithm>
#include "myMath.h"


void Player::Initialize(Model* model, uint32_t textureHandle, ViewProjection* viewProjection, const Vector3& position) {
	
	// NULLチェック
	assert(model);
	textureHandle_ = textureHandle;
	model_ = model;
	viewProjection_ = viewProjection;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;

	
}

void Player::Update() {
	if (onGround_) {
		// 移動
		if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT)) {

			Vector3 acceleration = {};
			if (Input::GetInstance()->PushKey(DIK_RIGHT)) {

				if (velocity_.x < 0.0f) {
					velocity_.x *= (1.0f - kAcceleration);
				}

				acceleration.x += kAcceleration;

				if (lrDirection_ != LRDirection::kRight) {
					lrDirection_ = LRDirection::kRight;
					trunFirstRotationY = worldTransform_.rotation_.y;
					turnTimer_ = 0.2f;
				}

			} else if (Input::GetInstance()->PushKey(DIK_LEFT)) {
				if (velocity_.x > 0.0f) {
					velocity_.x *= (1.0f - kAcceleration);
				}

				acceleration.x -= kAcceleration;

				if (lrDirection_ != LRDirection::kLeft) {
					lrDirection_ = LRDirection::kLeft;
					trunFirstRotationY = worldTransform_.rotation_.y;
					turnTimer_ = 0.2f;
				}
			}

			velocity_.x += acceleration.x;
			velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);

		// 振り向き
			if (turnTimer_ > 0.0f) {
				turnTimer_ -= 1.0f / 60.0f;

				float destinationRotationYTable[] = {std::numbers::pi_v<float> / 2.0f, std::numbers::pi_v<float> * 3.0f / 2.0f};

				float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];

				worldTransform_.rotation_.y = EaseInOut(destinationRotationY, trunFirstRotationY, turnTimer_ / kTimeTurn);
			}

		} else {
			velocity_.x *= (1.0f - kAcceleration);
		}
		// ジャンプ
		if (Input::GetInstance()->PushKey(DIK_UP)) {
			velocity_.y += kJumpAccleration;
			landing = false;
		}
	}
	// 空中
	else {
		velocity_.y += -kGravityAcceleration;

		velocity_.y = std::max(velocity_.y, -kLimitFallspeed);


	}
	if (velocity_.y < 1) {
		if (worldTransform_.translation_.y <= 1.5f) {
			landing = true;
		}
	}
	//	worldTransform_.translation_ += velocity_;
	worldTransform_.translation_.x += velocity_.x;
	worldTransform_.translation_.y += velocity_.y;
	worldTransform_.translation_.z += velocity_.z;

	if (onGround_) {
		if (velocity_.y > 1.0) {
			onGround_ = false;
		}
	} 
	else {
		if (landing) {
			worldTransform_.translation_.y = 1.5f;

			velocity_.x *= (1.0f - kAcceleration);

			velocity_.y = 0.0f;

			onGround_ = true;
		}
	}

	worldTransform_.UpdateMatrix();
	worldTransform_.TransferMatrix();
}

void Player::Draw(){

	// 3Dモデルを描画
	model_->Draw(worldTransform_, *viewProjection_, textureHandle_);
}