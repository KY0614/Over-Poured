#pragma once
#include <memory>
#include <map>
#include "Common/Transform.h"
class ResourceManager;
class WarpStar;
class Planet;
class Player;

class Stage
{

public:

	//ステージ名
	enum class NAME
	{
		MAIN_PLANET
	};

	//コンストラクタ
	Stage(Player& player);

	//デストラクタ
	~Stage(void);

	void Init(void);
	void Update(void);
	void Draw(void);

	//ステージ変更
	void ChangeStage(NAME type);

	//対象ステージを取得
	std::weak_ptr<Planet> GetPlanet(NAME type);

private:

	//シングルトン参照
	ResourceManager& resMng_;

	Player& player_;

};
