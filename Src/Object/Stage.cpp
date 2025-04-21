#include <vector>
#include <map>
#include <DxLib.h>
#include "../Utility/AsoUtility.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/ResourceManager.h"
#include "../Manager/Generic/Camera.h"
#include "Player.h"
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