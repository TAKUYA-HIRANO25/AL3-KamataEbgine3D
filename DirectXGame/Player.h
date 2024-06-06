#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Input.h"

/// <summary>
/// 自キャラ
/// </summary>
class Player {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model,ViewProjection* viewProjection ,const Vector3& position);

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
	static inline const float kAcceleration = 10.0f;
	static inline const float kAttenuation = 0.2f;
	static inline const float kLimitRunSpeed = 20;
	//ジャンプ
	bool onGround_ = true;
	static inline const float kGravityAcceleration = 9.8f;
	static inline const float kLimitFallspeed = 19.6f;
	static inline const float kJumpAccleration = 10.0f;

	//向き
	enum class LRDirection{
		kRight,
		kLeft,
	};
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

