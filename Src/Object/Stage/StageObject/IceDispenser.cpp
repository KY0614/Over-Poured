#include "../Common/DebugDrawFormat.h"
#include "../Utility/AsoUtility.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/InputManager.h"
#include "../Object/Common/Sphere.h"
#include "../Object/Player.h"
#include "../Object/Stage/StageManager.h"
#include "IceDispenser.h"

namespace {
	const std::string ICE_CUP = "Ice_Cup";	//アイス用カップ
	const std::string CUP_WITH_ICE = "Cup_With_Ice";		//アイス用カップ
}

IceDispenser::IceDispenser(const std::string objId, const float width,
	const float height, const float depth, Player& player,
	std::vector<std::unique_ptr<StageObject>>& object) :
	StageObject(objId, width, height, depth, player), objects_(object)
{
}

void IceDispenser::Draw(void)
{
	int line = 3;	//行
	int lineHeight = 30;	//行

	VECTOR screenPos = ConvWorldPosToScreenPos(GetTransform().pos);
	// 変換成功
	DrawFormatString(static_cast<int>(screenPos.x) - 30, static_cast<int>(screenPos.y) - 150, GetColor(255, 255, 255),
		L"氷をいれるまで %2.f", param_.interactTime);

	StageObject::Draw();
}

void IceDispenser::Interact(const std::string& objId)
{
	if (GetMachineState() == MACHINE_STATE::ACTIVE)return;	//アクティブ状態だったら処理しない

	auto& items = param_.acceptedItems_;
	//objIdがインタラクト対象物に存在するかどうか
	bool isAccepted = std::find(items.begin(), items.end(), objId) != items.end();
	if (!isAccepted)return;	//存在しなかったら処理しない

	//既にマシン内にカップまたはコーヒーがある場合は何もしない
	for (const auto& obj : objects_)
	{
		//アイスカップまたはアイスコーヒー
		if ((obj->GetObjectId() == ICE_CUP || obj->GetObjectId() == CUP_WITH_ICE) &&
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
		if (obj->GetObjectId() != ICE_CUP) continue;

		if (AsoUtility::IsHitSpheres(GetSpherePos(), GetSphereRad(),
			obj->GetSpherePos(), obj->GetSphereRad()))
		{
			//スペースキー押下でマシンの場所にカップを置く(とりあえず)
			if (player_.GetHoldItem() == items.front().c_str() &&
				ins.IsTrgDown(KEY_INPUT_SPACE))
			{
				VECTOR cupPos = GetTopCenter();	//マシンの上部中央にカップを置く
				cupPos.y += obj->GetObjHeight() / 2.0f;	//少し上にずらす
				obj->ItemPlaced(GetPos());
				ChangeMachineState(MACHINE_STATE::ACTIVE);
			}
		}
	}
}

void IceDispenser::UpdateInActive(void)
{
	SetProduceTime(COFFEE_PRODUCES_TIME);
}

void IceDispenser::UpdateActive(void)
{
	param_.interactTime -= SceneManager::GetInstance().GetDeltaTime();

	//マシンの当たり判定内にPLACED状態のカップが存在するかチェック
	bool hasPlacedCup = false;
	for (const auto& obj : objects_)
	{
		if (obj->GetObjectId() != ICE_CUP)continue;

		if (AsoUtility::IsHitSpheres(obj->GetSpherePos(), obj->GetSphereRad(),
			GetSpherePos(), GetSphereRad()) &&
			obj->GetItemState() == ITEM_STATE::PLACED)
		{
			hasPlacedCup = true;
			break;
		}
		//else if (AsoUtility::IsHitSpheres(obj->GetSpherePos(), obj->GetSphereRad(),
		//	GetSpherePos(), GetSphereRad()) &&
		//	obj->GetItemState() != ITEM_STATE::PLACED)
		//{
		//	break;	//PLACED状態のカップがなければループを抜ける
		//}
	}

	//PLACED状態のカップがなければ非アクティブにする
	if (!hasPlacedCup)
	{
		ChangeMachineState(MACHINE_STATE::INACTIVE);
		return;
	}

	//インタラクト時間が過ぎたら非アクティブにする
	if (param_.interactTime <= 0.0f)
	{
		ChangeMachineState(MACHINE_STATE::INACTIVE);
		return;
	}
}
