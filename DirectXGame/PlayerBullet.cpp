#include "PlayerBullet.h"
#include <cassert>
#include <imgui.h>
void PlayerBullet::Initialize(Vector3 playerPosition) {

	assert(model_ == nullptr);

	// モデルの生成
	model_ = new Model();
	model_ = Model::CreateSphere();

	// テクスチャ初期化
	textureHandle_ = TextureManager::Load("uvChecker.png");

	// ワールド行列の初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = playerPosition;

	// 生存フラグ
	isAlive_ = false;
}

void PlayerBullet::Update() {

	// ワールド行列の更新
	worldTransform_.UpdateMatrix();
	worldTransform_.TransferMatrix();
}

void PlayerBullet::UpdateImGui() {
	// playerBulletの情報表示
	ImGui::Text("PlayerBullet");
	ImGui::Text("isAlive : %s", isAlive_ ? "true" : "false");
	ImGui::Text("position : %f, %f, %f", worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z);
}

void PlayerBullet::Draw(const ViewProjection& viewProjection) {

	// モデル描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}