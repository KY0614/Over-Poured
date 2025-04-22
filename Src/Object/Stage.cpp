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
	//ƒ‚ƒfƒ‹§Œä‚ÌŠî–{î•ñ
	transform_.SetModel(
		ResourceManager::GetInstance().LoadModelDuplicate(
			ResourceManager::SRC::STAGE));
	transform_.scl = AsoUtility::VECTOR_ONE;
	transform_.pos = AsoUtility::VECTOR_ZERO;
	transform_.quaRot = Quaternion::Euler(
		0.0f,
		AsoUtility::Deg2RadF(0.0f),
		0.0f
	);
	transform_.quaRotLocal = Quaternion();
	transform_.Update();
}

void Stage::Update(void)
{
	transform_.Update();
}

void Stage::Draw(void)
{
	//ƒ‚ƒfƒ‹‚Ì•`‰æ
	MV1DrawModel(transform_.modelId);
}
