#include "ViewProjection.h"
#include "MathUilityForText.h"

class Player;

class CameraController {
public:
	struct Rect {
		float left = 0.0f;
		float right = 1.0f;
		float bottom = 0.0f;
		float top = 1.0f;
	};

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	void SetTarget(Player* target) { target_ = target; };

	void Reset();

	void SetMovableArea(Rect area) { movableArea_ = area; };

	const ViewProjection& GetViewProjection() const { return viewProjection_; }

private:
	static inline const float kVelocityBiss = 30.0f;
	static inline const float KInterpolationRate = 1.5f;
	ViewProjection viewProjection_;
	Player* target_ = nullptr;
	Vector3 targetOffset_ = {0, 0, -15.0f};
	Vector3 targetcoordinates;
	Rect movableArea_ = {0, 100, 0, 100};
	static inline const Rect Margin = {-3.0f, 3.0f, -4.0f, 4.0f};
};
