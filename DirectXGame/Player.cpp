#define NOMINMAX
#include "Player.h"
#include <cassert>
#include <numbers>
#include <algorithm>
#include "myMath.h"
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
	
	CollisionMapInfo collisionMapInfo = {};

	collisionMapInfo.velocity = velocity_;

	CheckMapColision(collisionMapInfo);

	CollisionMove(collisionMapInfo);

	HitCeiling(collisionMapInfo);

	Wall(collisionMapInfo);

	Ground(collisionMapInfo);

	PlayerTurning();

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

		} else {
			velocity_.x *= (1.0f - kAcceleration);
		}
		// ジャンプ
		if (Input::GetInstance()->PushKey(DIK_UP)) {
			velocity_.y += kJumpAccleration/60.0f;
			landing = false;
		}
	}
	// 空中
	else {
		velocity_.y += -kGravityAcceleration/60.0f;

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
	CheckMapCollisionDown(info);
	CheckMapCollisionRight(info);
	CheckMapCollisionLeft(info);

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

	if (info.velocity.y <= 0) {
		return;
	}

	std::array<Vector3, NumCorner> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.velocity, static_cast<Corner>(i));
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
		//めり込みを排除する方向に移動量を設定
		MapChipField::IndexSet indexSet = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + Vector3(0, +kHeight / 2.0f, 0));
		//めり込み先ブロックの範囲矩形
		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.velocity.y = std::max(0.0f, rect.bottom - worldTransform_.translation_.y - (+kHeight / 2.0f + kBlank));

		//天井に当たった
		info.HeavenFlag = true;
	}


}

void Player::CheckMapCollisionDown(CollisionMapInfo& info) {
	//下降あり？
	if (info.velocity.y >= 0) {
		return;
	}

	//移動後の4つの角の座標
	std::array<Vector3, NumCorner> positionNew;

	for (uint32_t i = 0; i < positionNew.size(); ++i) {
		positionNew[i] = CornerPosition(worldTransform_.translation_ + info.velocity, static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	//真下の当たり判定を行う
	bool hit = false;

	//左下点の判定
	MapChipField::IndexSet DownIndexSet;
	DownIndexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kLeftBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(DownIndexSet.xIndex, DownIndexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	//右下点の判定
	DownIndexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kRightBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(DownIndexSet.xIndex, DownIndexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	//ブロックにヒット？
	if (hit) {
		//めり込みを排除する方向に移動量を設定する
		MapChipField::IndexSet indexSet = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_  - Vector3(0, kHeight / 2.0f, 0));
		//めり込み先のブロックの範囲矩形
		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.velocity.y = std::min(0.0f, rect.top - worldTransform_.translation_.y - (+kHeight / 2.0f + kBlank));
		//地面に当たった
		info.landing = true;
	}
}

void Player::CheckMapCollisionRight(CollisionMapInfo& info) {
	//右移動あり？
	if (info.velocity.x <= 0) {
		return;
	}

	//移動後の4つの角の座標
	std::array<Vector3, NumCorner> positionNew;

	for (uint32_t i = 0; i < positionNew.size(); ++i) {
		positionNew[i] = CornerPosition(worldTransform_.translation_ + info.velocity, static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	// 右の当たり判定を行う
	bool hit = false;

	//右上点の判定
	MapChipField::IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kRightTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	//右下点の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kRightBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	if (hit) {
		//めり込みを排除する方向に移動量を設定する
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + Vector3(+kWidth / 2.0f, 0, 0));
		//めり込み先ブロックの範囲矩形
		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.velocity.x = std::min(0.0f, rect.left - (worldTransform_.translation_.x + kWidth / 2.0f + kBlank));
		//壁に当たった
		info.WallFlag = true;
	}
}

void Player::CheckMapCollisionLeft(CollisionMapInfo& info) {
	//左移動あり？
	if (info.velocity.x >= 0) {
		return;
	}

	//移動後の4つの角の座標
	std::array<Vector3, NumCorner> positionNew;

	for (uint32_t i = 0; i < positionNew.size(); ++i) {
		positionNew[i] = CornerPosition(worldTransform_.translation_ + info.velocity, static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	//左の当たり判定を行う
	bool hit = false;

	//左上点の判定
	MapChipField::IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kLeftTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	//左下点の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kLeftBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	if (hit) {
		//めり込みを排除する方向に移動量を設定する
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ - Vector3(kWidth / 2.0f, 0, 0));
		//めり込み先ブロックの範囲矩形
		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.velocity.x = std::max(0.0f, rect.right - (worldTransform_.translation_.x - kWidth / 2.0f - kBlank));
		//壁に当たった
		info.WallFlag = true;
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
	}
}

void Player::PlayerTurning() {

	if (turnTimer_ > 0.0f) {
		turnTimer_ -= 1.0f / 60.0f;

		// 左右の自キャラ角度テーブル
		float destinationRotationYTable[] = {std::numbers::pi_v<float> / 2.0f, std::numbers::pi_v<float> * 3.0f / 2.0f};
		// 状態に応じた角度を取得する
		float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];

		// 補間の割合を計算（0から1までの値）
		float t = (kTimeTurn - turnTimer_) / kTimeTurn;
		t = std::clamp(t, 0.0f, 1.0f);

		// 自キャラの角度を設定する
		worldTransform_.rotation_.y = Lerp(trunFirstRotationY, destinationRotationY, t);
	}
}

void Player::Ground(const CollisionMapInfo& info) {
	if (onGround_) {
		// ジャンプ開始
		if (velocity_.y > 0.0f) {
			//空中状態に移行
			onGround_ = false;

		} else {
			MapChipType mapChipType;

			//移動後の4つの角の座標
			std::array<Vector3, NumCorner> positionNew;

			for (uint32_t i = 0; i < positionNew.size(); ++i) {
				positionNew[i] = CornerPosition(worldTransform_.translation_ + info.velocity, static_cast<Corner>(i));
			}

			//真下の判定
			bool hit = false;

			//左下点の判定
			MapChipField::IndexSet DownIndexSet;
			DownIndexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kLeftBottom] + Vector3(0, -kSet, 0));
			mapChipType = mapChipField_->GetMapChipTypeByIndex(DownIndexSet.xIndex, DownIndexSet.yIndex);
			if (mapChipType == MapChipType::kBlock) {
				hit = true;
			}

			//右下点の判定
			DownIndexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kRightBottom] + Vector3(0, -kSet, 0));
			mapChipType = mapChipField_->GetMapChipTypeByIndex(DownIndexSet.xIndex, DownIndexSet.yIndex);
			if (mapChipType == MapChipType::kBlock) {
				hit = true;
			}
			if (!hit) {
				//空中状切り替え
				onGround_ = false;
			}
		}
	} else {
		// 着地フラグ
		if (info.landing) {
			//着地状態切り替え
			onGround_ = true;
			//X速度を減速
			velocity_.x *= (1.0f - kAttenuationLanding);
			//Y速度をゼロに
			velocity_.y = 0.0f;
		}
	}
}

void Player::Wall(const CollisionMapInfo& info) { 
	if (info.WallFlag) {
		velocity_.x *= (1.0f - kAttenuationWall);
	}
}

Vector3 Player::GetWorldPosition() {
	Vector3 worldPos;

	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}

AABB Player::GetAABB() {
	Vector3 worldPos = GetWorldPosition();

	AABB aabb;

	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};

	return aabb;
}

void Player::OnCollision(const Enemy* enemy) {
	(void)enemy;
	
	isDead_ = true;
}

void Player::OnCollision2(const Goal* goal) {
	(void)goal;

	isDead_ = true;
}