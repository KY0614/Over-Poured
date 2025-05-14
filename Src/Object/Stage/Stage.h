#pragma once
#include <memory>
#include <map>
#include "../ActorBase.h"

class ResourceManager;
class Player;

class Stage : public ActorBase
{

public:

	//コンストラクタ
	Stage(Player& player);

	//デストラクタ
	~Stage(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;


private:

	Player& player_;

	void UpdateDebugImGui(void);
};
