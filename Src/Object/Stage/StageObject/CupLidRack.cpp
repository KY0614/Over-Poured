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
	const std::string HOT_COFFEE = "HotCoffee";	//�z�b�g�R�[�q�[�̃I�u�W�F�N�gID
	const std::string ICE_COFFEE = "IceCoffee";	//�A�C�X�R�[�q�[�̃I�u�W�F�N�gID
}

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
		//�R�[�q�[�ƐڐG���Ă����ԂŁA�X�y�[�X�L�[���������Ă�����Q�[�W�����߂�
		if (CommonUtility::IsHitSpheres(GetSpherePos(), GetSphereRad(),
			obj->GetSpherePos(), obj->GetSphereRad()))
		{
			//�X�y�[�X�L�[��������������ƃQ�[�W�����܂��Ă���
			if (isHoldingCoffee && ins.IsInputPressed("Interact"))
			{
				//�C���^���N�g���Ԃ�����������
				param_.interactTime_ -= SceneManager::GetInstance().GetDeltaTime();
				isActioned_ = true;
				//�v���C���[�𑀍�ł��Ȃ���Ԃɂ���
				player_.ChangeState(Player::STATE::STOP);
				gaugeUI_->SetActive(true);	//�Q�[�WUI��\��
			}
			else
			{
				//�X�y�[�X�L�[�������Ă��Ȃ�������Q�[�W�����Z�b�g
				SetInteractTime(LID_PRODUCES_TIME);
				isActioned_ = false;
				//�v���C���[�𑀍�ł����Ԃɂ���
				player_.ChangeState(Player::STATE::PLAY);
				gaugeUI_->Reset();	//�Q�[�WUI�����Z�b�g
			}
		}
	}
}

void CupLidRack::Init(VECTOR pos, float rotY, VECTOR scale)
{
	//���f���̏�����
	StageObject::Init(pos,rotY, scale);
	//�����蔻��p�̋��̑��΍��W��ݒ�
	sphere_->SetLocalPos(SPHERE_LOCAL_POS);
	const float uiOffsetY = 50.0f;	//UI�̈ʒu�����p�̃I�t�Z�b�g
	//�Q�[�WUI�̏�����
	gaugeUI_ = std::make_unique<GaugeUI>(false, LID_PRODUCES_TIME);
	gaugeUI_->Init();
	VECTOR uiPos = transform_.pos;
	uiPos.y += uiOffsetY;		//UI�̈ʒu�𒲐�
	gaugeUI_->SetPos(uiPos);	// UI�̈ʒu��ݒ�
	UIManager::GetInstance().AddGaugeUI(gaugeUI_.get());
}

void CupLidRack::Update(void)
{
	//�A�N�V�������łȂ���΃C���^���N�g���Ԃ����Z�b�g
	if (!isActioned_)
	{
		SetInteractTime(LID_PRODUCES_TIME);
		player_.ChangeState(Player::STATE::PLAY);	//�v���C���[�𑀍�ł����Ԃɂ���
	}

	//�Q�[�WUI�̍X�V
	gaugeUI_->Update();
	
	//�C���^���N�g���Ԃ�0�ȉ��ɂȂ�����W������
	if (param_.interactTime_ <= 0.0f)
	{
		isActioned_ = false;
		gaugeUI_->Reset();	//�Q�[�WUI�����Z�b�g
	}

	//���f���̍X�V
	transform_.Update();
}

void CupLidRack::Draw(void)
{
	//���f����`��
	StageObject::Draw();
	}
