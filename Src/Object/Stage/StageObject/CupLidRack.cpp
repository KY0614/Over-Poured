#include "../Common/DebugDrawFormat.h"
#include "../Utility/AsoUtility.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/InputManager.h"
#include "../Object/Player.h"
#include "CupLidRack.h"

namespace {
	const std::string HOT_COFFEE = "Hot_Coffee";	//ホットコーヒー
	const std::string ICE_COFFEE = "Ice_Coffee";	//アイスコーヒー
}

CupLidRack::CupLidRack(const std::string objId, const float width,
	const float height, const float depth, Player& player,
	std::vector<std::unique_ptr<StageObject>>& object) :
	StageObject(objId, width, height, depth, player), objects_(object)
{
}

void CupLidRack::Interact(const std::string& objId)
{
	auto& items = param_.acceptedItems_;
	//objIdがインタラクト対象物に存在するかどうか
	bool isAccepted = std::find(items.begin(), items.end(), objId) != items.end();
	if (!isAccepted)return;	//存在しなかったら処理しない
	if (param_.interactTime <= 0.0f)return;

	auto& ins = InputManager::GetInstance();
	for (const auto& obj : objects_)
	{
		//コーヒー以外のオブジェクトは判定しない
		if (obj->GetObjectId() != HOT_COFFEE && 
			obj->GetObjectId() != ICE_COFFEE) continue;
		//既に蓋されているコーヒーは判定しない
		if (obj->IsLidOn())continue;
		
		bool isHoldingHotCoffee = obj->GetItemState()==ITEM_STATE::HOLD;

		if (AsoUtility::IsHitSpheres(GetSpherePos(), GetSphereRad(),
			obj->GetSpherePos(), obj->GetSphereRad()))
		{
			//スペースキーを押下し続けるとゲージがたまっていく
			if (isHoldingHotCoffee &&
				ins.IsInputPressed("Interact"))
			{
				param_.interactTime -= SceneManager::GetInstance().GetDeltaTime();
				isActioned_ = true;
				player_.ChangeState(Player::STATE::STOP);
			}
			else
			{
				param_.interactTime = 3.0f;
				isActioned_ = false;
				player_.ChangeState(Player::STATE::PLAY);
			}
		}
	}
}

void CupLidRack::Update(void)
{
	if (!isActioned_)
	{
		param_.interactTime = 3.0f;
		player_.ChangeState(Player::STATE::PLAY);
	}
	if (param_.interactTime <= 0.0f)
	{
		isActioned_ = false;
	}

	transform_.Update();
}

void CupLidRack::Draw(void)
{
	int line = 4;	//行
	int lineHeight = 30;	//行
	//DebugDrawFormat::FormatStringRight(L"LidTime %2.f", param_.interactTime, line, lineHeight);
	
	VECTOR screenPos = ConvWorldPosToScreenPos(GetTransform().pos);
	// 変換成功
	DrawFormatString(static_cast<int>(screenPos.x) - 30, static_cast<int>(screenPos.y) - 150, GetColor(255, 255, 255),
		L"蓋をする %2.f", param_.interactTime);
	
	StageObject::Draw();
}
