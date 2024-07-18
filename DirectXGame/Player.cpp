#define NOMINMAX
#include "Player.h"
#include <cassert>
#include <numbers>
#include <algorithm>
#include "myMath.h"
#include "MathUilityForText.h"
#include "MapChipField.h"

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
	InputMove();
	
	CollisionMapInfo collisionMapInfo;

	collisionMapInfo.velocity = velocity_;

	CheckMapColision(collisionMapInfo);

	CollisionMove(collisionMapInfo);

	HitCeiling(collisionMapInfo);

	worldTransform_.UpdateMatrix();

}

void Player::Draw(){

	// 3Dモデルを描画
	model_->Draw(worldTransform_, *viewProjection_, textureHandle_);
}

void Player::InputMove() {
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
	if (velocity_.y < 0) {
		if (worldTransform_.translation_.y <= 1.0f) {
			landing = true;
		}
	}
	//	worldTransform_.translation_ += velocity_;
	worldTransform_.translation_.x += velocity_.x;
	worldTransform_.translation_.y += velocity_.y;
	worldTransform_.translation_.z += velocity_.z;

	if (onGround_) {
		if (velocity_.y > 0.0f) {
			onGround_ = false;
		}
	} else {
		if (landing) {
			worldTransform_.translation_.y = 1.0f;

			velocity_.x *= (1.0f - kAcceleration);

			velocity_.y = 0.0f;

			onGround_ = true;
		}
	}
}

void Player::CheckMapColision(CollisionMapInfo& info) { 
	CheckMapCollisionTop(info);

}

Vector3 Player::CornerPosition(const Vector3& center, Corner corner) { 
	Vector3 offsetTable[NumCorner] = {
	    {+kWidth / 2.0f, -kHeight / 2.0f, 0},
        {-kWidth / 2.0f, -kHeight / 2.0f, 0},
        {+kWidth / 2.0f, +kHeight / 2.0f, 0},
        {-kWidth / 2.0f, +kHeight / 2.0f, 0}
    };

	return center + offsetTable[static_cast<uint32_t>(corner)];
}

void Player::CheckMapCollisionTop(CollisionMapInfo& info) { 
	std::array<Vector3, NumCorner> positionsNew;
	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.velocity, static_cast<Corner>(i));
	}

	if (info.velocity.y <= 0) {
		return;
	}

	MapChipType mapChipType;
	//真上の当たり判定
	bool hit = false;
	//左上
	MapChipField::IndexSet TopIndexSet;
	TopIndexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(TopIndexSet.xIndex, TopIndexSet.yIndex);

	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	//右上
	TopIndexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(TopIndexSet.xIndex, TopIndexSet.yIndex);

	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	if (hit) {
		// めり込みを排除する方向に移動量を設定
		MapChipField::IndexSet indexSet = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + Vector3(0, +kHeight / 2.0f, 0));
		// めり込み先ブロックの範囲矩形
		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.velocity.y = std::max(0.0f, rect.bottom - worldTransform_.translation_.y - (+kHeight / 2.0f + kBlank));
		// 天井に当たったことを記録
		info.HeavenFlag = true;
	}


}

void Player::CollisionMove(const CollisionMapInfo& info) { 
	//移動
	worldTransform_.translation_ += info.velocity;
}

void Player::HitCeiling(const CollisionMapInfo& info) {
	// 天井に当たった
	if (info.HeavenFlag) {
		velocity_.y = 0;
		// 位置修正
		worldTransform_.translation_.y += info.velocity.y;
	}
}