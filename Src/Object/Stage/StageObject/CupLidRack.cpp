#include "../Utility/CommonUtility.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/InputManager.h"
#include "../Object/Player.h"
#include "../../UI/GaugeUI.h"
#include "../../UI/UIManager.h"
#include "../../Common/Sphere.h"
#include "CupLidRack.h"

namespace
{
	const VECTOR SPHERE_LOCAL_POS = { 0.0f,30.0f,0.0f };
	const std::string HOT_COFFEE = "HotCoffee";	//ホットコーヒーのオブジェクトID
	const std::string ICE_COFFEE = "IceCoffee";	//アイスコーヒーのオブジェクトID
}

CupLidRack::CupLidRack(const std::string objId,Player& player,
	std::vector<std::unique_ptr<StageObject>>& object) :
	StageObject(objId, player), objects_(object)
{
}

void CupLidRack::Interact(const std::string& objId)
{
	auto& items = param_.acceptedItems_;
	//objIdがインタラクト対象物に存在するかどうか
	bool isAccepted = std::find(items.begin(), items.end(), objId) != items.end();
	if (!isAccepted)return;	//存在しなかったら処理しない
	if (param_.interactTime_ <= 0.0f)return;

	auto& ins = InputManager::GetInstance();
	for (const auto& obj : objects_)
	{
		//コーヒー以外のオブジェクトは判定しない
		if (obj->GetParam().id_ != HOT_COFFEE && 
			obj->GetParam().id_ != ICE_COFFEE) continue;
		//既に蓋されているコーヒーは判定しない
		if (obj->IsLidOn())continue;
		
		//所持状態のコーヒーかどうかのフラグ
		bool isHoldingCoffee = obj->GetItemState()==ITEM_STATE::HOLD;
		//コーヒーと接触している状態で、スペースキーを押下していたらゲージをためる
		if (CommonUtility::IsHitSpheres(GetSpherePos(), GetSphereRad(),
			obj->GetSpherePos(), obj->GetSphereRad()))
		{
			//スペースキーを押下し続けるとゲージがたまっていく
			if (isHoldingCoffee && ins.IsInputPressed("Interact"))
			{
				//インタラクト時間を減少させる
				param_.interactTime_ -= SceneManager::GetInstance().GetDeltaTime();
				isActioned_ = true;
				//プレイヤーを操作できない状態にする
				player_.ChangeState(Player::STATE::STOP);
				gaugeUI_->SetActive(true);	//ゲージUIを表示
			}
			else
			{
				//スペースキーを押していなかったらゲージをリセット
				SetInteractTime(LID_PRODUCES_TIME);
				isActioned_ = false;
				//プレイヤーを操作できる状態にする
				player_.ChangeState(Player::STATE::PLAY);
				gaugeUI_->Reset();	//ゲージUIをリセット
			}
		}
	}
}

void CupLidRack::Init(VECTOR pos, float rotY, VECTOR scale)
{
	//モデルの初期化
	StageObject::Init(pos,rotY, scale);
	//当たり判定用の球の相対座標を設定
	sphere_->SetLocalPos(SPHERE_LOCAL_POS);
	const float uiOffsetY = 50.0f;	//UIの位置調整用のオフセット
	//ゲージUIの初期化
	gaugeUI_ = std::make_unique<GaugeUI>(false, LID_PRODUCES_TIME);
	gaugeUI_->Init();
	VECTOR uiPos = transform_.pos;
	uiPos.y += uiOffsetY;		//UIの位置を調整
	gaugeUI_->SetPos(uiPos);	// UIの位置を設定
	UIManager::GetInstance().AddGaugeUI(gaugeUI_.get());
}

void CupLidRack::Update(void)
{
	//アクション中でなければインタラクト時間をリセット
	if (!isActioned_)
	{
		SetInteractTime(LID_PRODUCES_TIME);
		player_.ChangeState(Player::STATE::PLAY);	//プレイヤーを操作できる状態にする
	}

	//ゲージUIの更新
	gaugeUI_->Update();
	
	//インタラクト時間が0以下になったら蓋をする
	if (param_.interactTime_ <= 0.0f)
	{
		isActioned_ = false;
		gaugeUI_->Reset();	//ゲージUIをリセット
	}

	//モデルの更新
	transform_.Update();
}

void CupLidRack::Draw(void)
{
	//モデルを描画
	StageObject::Draw();
	}
