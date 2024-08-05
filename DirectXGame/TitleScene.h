#include "Audio.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DirectXCommon.h"

class TitleScene {
public:
	void Initialize();

	void Update();

	void Draw();

	//デスフラグ
	bool IsFinished() const { return finished_; }

private:
	//フラグ
	bool finished_ = false;

	DirectXCommon* dxCommon_ = nullptr;

	Model* modelTitle_ = nullptr;

	WorldTransform titleWorldTransform_;

	ViewProjection viewProjection_;

	// 最初の角度
	static inline const float TitlePositionStart = 0.1f;

	// 最後の角度
	static inline const float TitlePositionEnd = -0.1f;

	// アニメーションの周期
	static inline const float TitleTime = 2.0f;

	// 経過時間
	float timer_ = 0.0f;

};