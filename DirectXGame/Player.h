#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Input.h"

/// //向き
enum class LRDirection {
	kRight,
	kLeft,
};

class Player {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model, uint32_t textureHandle,ViewProjection* viewProjection, const Vector3& position);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private:
	//移動
	Vector3 velocity_ = {};
	static inline const float kAcceleration = 0.1f;
	static inline const float kAttenuation = 0.1f;
	static inline const float kLimitRunSpeed = 0.2f;

	//ジャンプ
	bool onGround_ = true;
	bool landing = false;

	static inline const float kGravityAcceleration = 0.5f;
	static inline const float kLimitFallspeed = 0.5f;
	static inline const float kJumpAccleration = 3.0f;

	float trunFirstRotationY = 0.0f;
	float turnTimer_ = 0.0f;
	static inline const float kTimeTurn = 0.3f;
	LRDirection lrDirection_ = LRDirection::kRight;
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	ViewProjection* viewProjection_ = nullptr;

};

