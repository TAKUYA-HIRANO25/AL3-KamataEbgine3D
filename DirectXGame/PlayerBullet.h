#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "TextureManager.h"
class PlayerBullet {
public:
	PlayerBullet() = default;
	~PlayerBullet();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Vector3 playerPosition);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// ワールド行列のみの更新処理
	/// </summary>
	void UpdateMatrix() { worldTransform_.UpdateMatrix(); }

	/// <summary>
	/// ImGuiの更新処理
	/// </summary>
	void UpdateImGui();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw(const ViewProjection& viewProjection);

private:
	// モデル
	Model* model_ = nullptr;

	// テクスチャ
	uint32_t textureHandle_;

	// ワールド行列
	WorldTransform worldTransform_;

	// 速度
	Vector3 velocity_ = {0, 0, 0};

	// 生存フラグ
	bool isAlive_ = false;

};
