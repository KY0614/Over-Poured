#include "../Utility/AsoUtility.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/InputManager.h"
#include "../Object/Player.h"
#include "../../UI/GaugeUI.h"
#include "../../UI/UIManager.h"
#include "../../Common/Sphere.h"
#include "CupLidRack.h"

CupLidRack::CupLidRack(const std::string objId,Player& player,
	std::vector<std::unique_ptr<StageObject>>& object) :
	StageObject(objId, player), objects_(object)
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
		
		//������Ԃ̃R�[�q�[���ǂ����̃t���O
		bool isHoldingCoffee = obj->GetItemState()==ITEM_STATE::HOLD;

		if (AsoUtility::IsHitSpheres(GetSpherePos(), GetSphereRad(),
			obj->GetSpherePos(), obj->GetSphereRad()))
		{
			//�X�y�[�X�L�[��������������ƃQ�[�W�����܂��Ă���
			if (isHoldingCoffee && ins.IsInputPressed("Interact"))
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
	sphere_->SetLocalPos(VGet(0.0f,30.0f,0.0f));

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
	//���f����`��
	StageObject::Draw();
	}
