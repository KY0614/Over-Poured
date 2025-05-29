#include "../Common/DebugDrawFormat.h"
#include "../Utility/AsoUtility.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/InputManager.h"
#include "../Object/Common/Sphere.h"
#include "../Object/Player.h"
#include "../Object/Stage/StageManager.h"
#include "Machine.h"

namespace {
	const std::string NO_ITEM = "";	//ホット用カップ
	const std::string HOT_CUP = "Hot_Cup";	//ホット用カップ
	const std::string ICE_CUP = "Ice_Cup";	//アイス用カップ
	const std::string CUP_WITH_ICE = "Cup_With_Ice";		//アイス用カップ
	const std::string COFFEE_MACHINE = "Coffee_Machine";	//コーヒーマシン
}

Machine::Machine(const std::string objId, const float width,
	const float height, const float depth,Player& player,
	std::vector<std::unique_ptr<StageObject>>& object) :
	StageObject(objId, width, height, depth,player),objects_(object)
{
	SetProduceTime(COFFEE_PRODUCES_TIME);
}

void Machine::Interact(const std::string& objId, std::vector<std::unique_ptr<StageObject>>& object)
{
	auto& items = param_.acceptedItems_;
	//objIdがインタラクト対象物に存在するかどうか
	bool isAccepted = std::find(items.begin(), items.end(), objId) != items.end();
	if (!isAccepted)return;	//存在しなかったら処理しない

	auto& ins = InputManager::GetInstance();
	bool setCup = false;
	for (const auto& obj : object)
	{
		if (player_.GetIsHolding())
		{
			//スペースキー押下でマシンの場所にカップを置く(とりあえず)
			if (player_.GetHoldItem() == items.front().c_str() &&
				ins.IsTrgDown(KEY_INPUT_SPACE))
			{
				obj->ItemPlaced(StageManager::MACHINE_POS);
				ChangeMachineState(MACHINE_STATE::ACTIVE);
			}
		}
	}
}

void Machine::UpdateInActive(void)
{
	SetProduceTime(COFFEE_PRODUCES_TIME);
}

void Machine::UpdateActive(void)
{
	param_.interactTime -= SceneManager::GetInstance().GetDeltaTime();

	auto& pSphere = player_.GetSphere();
	for (const auto& obj : objects_)
	{
		if (param_.interactTime <= 0.0f ||
			(AsoUtility::IsHitSpheres(obj->GetSpherePos(), obj->GetSphereRad(),
				GetSpherePos(), GetSphereRad())&& obj->GetItemState() != ITEM_STATE::PLACED))ChangeMachineState(MACHINE_STATE::INACTIVE);
	}
}

void Machine::Init(void)
{
	StageObject::Init();
}

void Machine::Update(void)
{
	StageObject::Update();
}

void Machine::Draw(void)
{
	int line = 3;	//行
	int lineHeight = 30;	//行
	DebugDrawFormat::FormatStringRight(L"iteractTime %2.f", param_.interactTime, line, lineHeight);
	StageObject::Draw();
}