#include "Input.h"

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


};