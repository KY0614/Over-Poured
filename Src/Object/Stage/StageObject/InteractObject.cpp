#include "../Common/DebugDrawFormat.h"
#include "../Utility/AsoUtility.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/InputManager.h"
#include "../Object/Common/Sphere.h"
#include "../Object/Player.h"
#include "InteractObject.h"

InteractObject::InteractObject(const std::string objId,
	const float width, const float height,
	const float depth, Player& player,
	std::vector<std::unique_ptr<StageObject>>& object) :
	StageObject(objId, width, height, depth, player), objects_(object)
{
}

void InteractObject::Draw(void)
{
	int line = 3;	//行
	int lineHeight = 30;	//行

	VECTOR screenPos = ConvWorldPosToScreenPos(GetTransform().pos);
	// 変換成功
	DrawFormatString(static_cast<int>(screenPos.x) - 30, static_cast<int>(screenPos.y) - 150, GetColor(255, 255, 255),
		L"interactTime %2.f", param_.interactTime);

	StageObject::Draw();
}

void InteractObject::Interact(const std::string& objId)
{
	auto& items = param_.acceptedItems_;
	//objIdがインタラクト対象物に存在するかどうか
	bool isAccepted = std::find(items.begin(), items.end(), objId) != items.end();
	if (!isAccepted)return;	//存在しなかったら処理しない

}

void InteractObject::UpdateInActive(void)
{
}

void InteractObject::UpdateActive(void)
{
}

void InteractObject::IceDispenser(void)
{
	if (GetMachineState() == MACHINE_STATE::ACTIVE)return;	//アクティブ状態だったら処理しない

	
	for (const auto& obj : objects_)
	{
		//既にマシン内にカップまたは氷入りカップがある場合は何もしない
		if ((obj->GetObjectId() == "Ice_Cup" || obj->GetObjectId() == "Cup_With_Ice") &&
			AsoUtility::IsHitSpheres(GetSpherePos(), GetSphereRad(),
				obj->GetSpherePos(), obj->GetSphereRad()) &&
			obj->GetItemState() == ITEM_STATE::PLACED)
		{
			// 既にセット済み
			return;
		}
	}

	auto& ins = InputManager::GetInstance();
	for (const auto& obj : objects_)
	{
		//アイス用カップ以外のオブジェクトは判定しない
		if (obj->GetObjectId() != "Ice_Cup") continue;

		if (AsoUtility::IsHitSpheres(GetSpherePos(), GetSphereRad(),
			obj->GetSpherePos(), obj->GetSphereRad()))
		{
			//スペースキー押下でマシンの場所にカップを置く(とりあえず)
			if (/*player_.GetHoldItem() == items.front().c_str() &&
				*/ins.IsTrgDown(KEY_INPUT_SPACE))
			{
				VECTOR cupPos = GetTopCenter();	//マシンの上部中央にカップを置く
				cupPos.y += obj->GetObjHeight() / 2.0f;	//少し上にずらす
				obj->ItemPlaced(GetPos());
				ChangeMachineState(MACHINE_STATE::ACTIVE);
			}
		}
	}
}

void InteractObject::CoffeeMachine(void)
{
	//既にマシン内にカップまたはコーヒーがある場合は何もしない
	for (const auto& obj : objects_)
	{
		// ホットカップまたはホットコーヒー
		if ((obj->GetObjectId() == "Hot_Cup" ||
			obj->GetObjectId() == "Hot_Coffee" ||
			obj->GetObjectId() == "Cup_With_Ice" ||
			obj->GetObjectId() == "Ice_Coffee") &&
			AsoUtility::IsHitSpheres(GetSpherePos(), GetSphereRad(),
				obj->GetSpherePos(), obj->GetSphereRad()) &&
			obj->GetItemState() == ITEM_STATE::PLACED)
		{
			// 既にセット済み
			return;
		}
	}

	auto& ins = InputManager::GetInstance();
	for (const auto& obj : objects_)
	{
		//カップ以外のオブジェクトは判定しない
		if (obj->GetObjectId() != "Hot_Cup" && obj->GetObjectId() != "Cup_With_Ice") continue;

		if (AsoUtility::IsHitSpheres(GetSpherePos(), GetSphereRad(),
			obj->GetSpherePos(), obj->GetSphereRad()))
		{
			//スペースキー押下でマシンの場所にカップを置く
			if (/*(player_.GetHoldItem() == items.front().c_str() ||
				player_.GetHoldItem() == items.back().c_str()) &&*/
				ins.IsInputTriggered("Interact"))
			{
				VECTOR cupPos = GetTopCenter();	//マシンの上部中央にカップを置く
				cupPos.y += obj->GetObjHeight() / 2.0f;	//少し上にずらす
				obj->ItemPlaced(cupPos);
				ChangeMachineState(MACHINE_STATE::ACTIVE);
			}
		}
	}
}

void InteractObject::CupLidRack(void)
{
	auto& ins = InputManager::GetInstance();
	for (const auto& obj : objects_)
	{
		//コーヒー以外のオブジェクトは判定しない
		if (obj->GetObjectId() != "Hot_Coffee" &&
			obj->GetObjectId() != "Ice_Coffee") continue;
		//既に蓋されているコーヒーは判定しない
		if (obj->IsLidOn())continue;

		bool isHoldingHotCoffee = obj->GetItemState() == ITEM_STATE::HOLD;

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
