#pragma once
#include <memory>
#include <map>
#include "../ActorBase.h"
#include "../../Common/Vector2.h"

class ResourceManager;
class StageObject;

class StageManager : public ActorBase
{

public:
	
	struct Size {
		int height_;
		int width_;
	};

	//コンストラクタ
	StageManager(Vector2 mapSize);

	//デストラクタ
	~StageManager(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;

private:

	Size size_;

	std::unique_ptr<StageObject> machine_;

	std::vector<std::vector<std::unique_ptr<StageObject>>> grid_;

	bool IsInBounds(int x, int y) const;

	void UpdateDebugImGui(void);
};
