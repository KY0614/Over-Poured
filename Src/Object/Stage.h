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

	//�X�e�[�W��
	enum class NAME
	{
		MAIN_PLANET
	};

	//�R���X�g���N�^
	Stage(Player& player);

	//�f�X�g���N�^
	~Stage(void);

	void Init(void);
	void Update(void);
	void Draw(void);

	//�X�e�[�W�ύX
	void ChangeStage(NAME type);

	//�ΏۃX�e�[�W���擾
	std::weak_ptr<Planet> GetPlanet(NAME type);

private:

	//�V���O���g���Q��
	ResourceManager& resMng_;

	Player& player_;

};
