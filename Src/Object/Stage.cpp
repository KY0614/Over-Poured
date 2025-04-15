#include <vector>
#include <map>
#include <DxLib.h>
#include "../Utility/AsoUtility.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/Camera.h"
#include "Player.h"
#include "Planet.h"
#include "Common/Collider.h"
#include "Common/Transform.h"
#include "Stage.h"

Stage::Stage(Player& player) 
	: resMng_(ResourceManager::GetInstance()), player_(player)
{

}

Stage::~Stage(void)
{
}

void Stage::Init(void)
{

}

void Stage::Update(void)
{

}

void Stage::Draw(void)
{

}

void Stage::ChangeStage(NAME type)
{
}