#include "WorldTransform.h"
#include "Model.h"
#include "MathUilityForText.h"

class Player;

class Enemy {
public:
	///<summary>
	///初期化
	///</summary>
	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position);

	///<summary>
	///更新
	///</summary>
	void Update();

	/// summary>
	///描画
	///</summary>
	void Draw();

	Vector3 GetWorldPosition();

	AABB HitAABB();

	void OnCollision(const Player* player);

private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	ViewProjection* viewProjection_ = nullptr;

	//歩行の速さ
	static inline const float kWalkSpeed = 0.01f;

	//速度
	Vector3 velocity_ = {};

	//最初の角度
	static inline const float kWalkMotionAngleStart = 2.0f;
	//最後の角度
	static inline const float kWalkMotionAngleEnd = -2.0f;
	//アニメーションの周期となる時間
	static inline const float kWalkMotionTime = 1.0f;
	//経過時間
	float walkTimer_ = 0.0f;

	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;

};