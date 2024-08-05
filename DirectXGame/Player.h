#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Input.h"
#include "MathUilityForText.h"

class MapChipField;

class Enemy;

//向き
enum class LRDirection {
	kRight,
	kLeft,
};

class Player {
public:
	//マップとの当たり判定情報
	struct CollisionMapInfo {
		bool HeavenFlag = false;
		bool landing = false;
		bool WallFlag = false;
		Vector3 velocity;
	};
	//角
	enum Corner {
		kRightBottom, // 右下
		kLeftBottom,  // 左下
		kRightTop,    // 右上
		kLeftTop,     // 左上

		NumCorner // 要素数
	};
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

	const WorldTransform& GetWorldTransform() const { return worldTransform_; }

	const Vector3& GetVelocity() const { return velocity_; }

	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; };

	void InputMove();

	void CheckMapColision(CollisionMapInfo& info);

	Vector3 CornerPosition(const Vector3& center, Corner corner);

	void CheckMapCollisionTop(CollisionMapInfo& info);

	void CheckMapCollisionDown(CollisionMapInfo& info);

	void CheckMapCollisionRight(CollisionMapInfo& info);

	void CheckMapCollisionLeft(CollisionMapInfo& info);

	void CollisionMove(const CollisionMapInfo& info);

	void HitCeiling(const CollisionMapInfo& info);

	void PlayerTurning();

	void Ground(const CollisionMapInfo& info);

	void Wall(const CollisionMapInfo& info);

	Vector3 GetWorldPosition();

	AABB GetAABB();

	void OnCollision(const Enemy* enemy);

	bool IsDead() const { return isDead_; }

private:
	//移動
	Vector3 velocity_ = {};
	static inline const float kAcceleration = 0.2f;
	static inline const float kAttenuation = 0.5f;
	static inline const float kLimitRunSpeed = 0.1f;

	//ジャンプ
	bool onGround_ = true;
	bool landing = false;

	static inline const float kGravityAcceleration = 0.3f;
	static inline const float kLimitFallspeed = 0.5f;
	static inline const float kJumpAccleration = 10.0f;

	static inline const float kAttenuationLanding = 0.5f;

	static inline const float kAttenuationWall = 0.5f;

	static inline const float kSet = 0.15f;

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

	//マップチップフィールド
	MapChipField* mapChipField_ = nullptr;

	//キャラの当たり判定
	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;

	static inline const float kBlank = 0.01f;

	 static inline const float kJumpAcceleration = 1.0f;

	 // フラグ
	 bool isDead_ = false;

};

