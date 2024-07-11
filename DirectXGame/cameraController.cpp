#define NOMINMAX
#include "cameraController.h"
#include "Player.h"
#include <algorithm>

void CameraController::Initialize(){
	viewProjection_.Initialize();

};

void CameraController::Update(){ 
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();
	const Vector3& targetVelocity = target_->GetVelocity();

	viewProjection_.translation_ = targetWorldTransform.translation_ + targetOffset_;

	targetcoordinates = targetWorldTransform.translation_ + targetOffset_ + targetVelocity * kVelocityBiss;

	viewProjection_.translation_ = Lerp(viewProjection_.translation_, targetcoordinates, KInterpolationRate);

	viewProjection_.translation_.x = std::max(viewProjection_.translation_.x, targetWorldTransform.translation_.x + Margin.left);
	viewProjection_.translation_.x = std::min(viewProjection_.translation_.x, targetWorldTransform.translation_.x + Margin.right);
	viewProjection_.translation_.y = std::max(viewProjection_.translation_.y, targetWorldTransform.translation_.x + Margin.bottom);
	viewProjection_.translation_.y = std::min(viewProjection_.translation_.y, targetWorldTransform.translation_.x + Margin.top);

	viewProjection_.translation_.x = std::max(viewProjection_.translation_.x, movableArea_.left);
	viewProjection_.translation_.x = std::min(viewProjection_.translation_.x, movableArea_.right);
	viewProjection_.translation_.y = std::max(viewProjection_.translation_.y, movableArea_.bottom);
	viewProjection_.translation_.y = std::min(viewProjection_.translation_.y, movableArea_.top);

	viewProjection_.UpdateMatrix();

};

void CameraController::Reset(){ 
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();
	viewProjection_.translation_ = targetWorldTransform.translation_ + targetOffset_;


};

