#pragma once
#include <memory>
#include <map>
#include "../ActorBase.h"

class ResourceManager;
class Player;

class Stage : public ActorBase
{

public:

	struct ModelData{
	std::string id_;
	std::string type_;
	std::string name_;
	std::string modelPath_;
	bool interactable_;
	bool placeable;
	std::string produces;
	};

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
