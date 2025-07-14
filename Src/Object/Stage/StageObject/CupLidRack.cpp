#include "../Common/DebugDrawFormat.h"
#include "../Utility/AsoUtility.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/InputManager.h"
#include "../Object/Player.h"
#include "../../UI/GaugeUI.h"
#include "../../UI/UIManager.h"
#include "CupLidRack.h"

namespace {
	const std::string HOT_COFFEE = "Hot_Coffee";	//�z�b�g�R�[�q�[
	const std::string ICE_COFFEE = "Ice_Coffee";	//�A�C�X�R�[�q�[
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
	//objId���C���^���N�g�Ώە��ɑ��݂��邩�ǂ���
	bool isAccepted = std::find(items.begin(), items.end(), objId) != items.end();
	if (!isAccepted)return;	//���݂��Ȃ������珈�����Ȃ�
	if (param_.interactTime_ <= 0.0f)return;

	auto& ins = InputManager::GetInstance();
	for (const auto& obj : objects_)
	{
		//�R�[�q�[�ȊO�̃I�u�W�F�N�g�͔��肵�Ȃ�
		if (obj->GetParam().id_ != HOT_COFFEE && 
			obj->GetParam().id_ != ICE_COFFEE) continue;
		//���ɊW����Ă���R�[�q�[�͔��肵�Ȃ�
		if (obj->IsLidOn())continue;
		
		bool isHoldingHotCoffee = obj->GetItemState()==ITEM_STATE::HOLD;

		if (AsoUtility::IsHitSpheres(GetSpherePos(), GetSphereRad(),
			obj->GetSpherePos(), obj->GetSphereRad()))
		{
			//�X�y�[�X�L�[��������������ƃQ�[�W�����܂��Ă���
			if (isHoldingHotCoffee &&
				ins.IsInputPressed("Interact"))
			{
				param_.interactTime_ -= SceneManager::GetInstance().GetDeltaTime();
				isActioned_ = true;
				player_.ChangeState(Player::STATE::STOP);
 				gaugeUI_->SetActive(true);
			}
			else
			{
				SetInteractTime(LID_PRODUCES_TIME);
				isActioned_ = false;
				player_.ChangeState(Player::STATE::PLAY);
				gaugeUI_->Reset();
			}
		}
	}
}

void CupLidRack::Init(VECTOR pos, float rotY, VECTOR scale)
{
	StageObject::Init(pos,rotY, scale);

	gaugeUI_ = std::make_unique<GaugeUI>(false, LID_PRODUCES_TIME);
	gaugeUI_->Init();
	VECTOR uiPos = transform_.pos;
	uiPos.y += 50.0f;	//UI�̈ʒu�𒲐�
	gaugeUI_->SetPos(uiPos); // UI�̈ʒu��ݒ�
	UIManager::GetInstance().AddGaugeUI(gaugeUI_.get());
}

void CupLidRack::Update(void)
{
	if (!isActioned_)
	{
		SetInteractTime(LID_PRODUCES_TIME);
		player_.ChangeState(Player::STATE::PLAY);
	}
		gaugeUI_->Update();
	
	if (param_.interactTime_ <= 0.0f)
	{
		isActioned_ = false;
		gaugeUI_->Reset();
	}

	transform_.Update();
}

void CupLidRack::Draw(void)
{
	//int line = 4;	//�s
	//int lineHeight = 30;	//�s
	////DebugDrawFormat::FormatStringRight(L"LidTime %2.f", param_.interactTime, line, lineHeight);
	//
	//VECTOR screenPos = ConvWorldPosToScreenPos(GetTransform().pos);
	//// �ϊ�����
	//DrawFormatString(static_cast<int>(screenPos.x) - 30, static_cast<int>(screenPos.y) - 150, GetColor(255, 255, 255),
	//	L"�W������ %2.f", param_.interactTime_);

	StageObject::Draw();

	gaugeUI_->Draw();

}
