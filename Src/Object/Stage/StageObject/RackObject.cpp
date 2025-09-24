#include "../../../Manager/GameSystem/SoundManager.h"
#include "../../../Manager/Generic/SceneManager.h"
#include "../../../Manager/Generic/InputManager.h"
#include "../../../Manager/Generic/ResourceManager.h"
#include "../../../Utility/CommonUtility.h"
#include "../../Common/Sphere.h"
#include "../Object/Player.h"
#include "../../UI/GaugeUI.h"
#include "../../UI/IconUI.h"
#include "../../UI/UIManager.h"
#include "ItemObject.h"
#include "RackObject.h"

namespace 
{
	//�X�C�[�c�̑傫��
	const float SWEETS_SCALE = 1.2f;				
}

RackObject::RackObject(const std::string objId,Player& player) :
	StageObject(objId, player)
{
	sweetsStockCnt_ = SWEETS_STOCK_MAX;
	cupsStockCnt_ = CUP_STOCK_MAX;
	hasStock_ = true;
	addInterval_ = 0.0f;
}

void RackObject::PickUp(std::string rackName,std::vector<std::unique_ptr<StageObject>>& object)
{
	if (!hasStock_)return;
	//isInteract_ = true;
	iconUI_->SetActive(true);
	auto& ins = InputManager::GetInstance();
	auto& sound = SoundManager::GetInstance();
	//�z�b�g�J�b�v�p���b�N�ɃC���^���N�g�����Ƃ��̏���
	if (rackName == HOT_CUP_RACK && ins.IsInputTriggered("Interact"))
	{
		//SE�Đ�
		sound.Play(SoundManager::SOUND::PICK_UP);
		//�z�b�g�J�b�v�����o��
		object.emplace_back(std::make_unique<ItemObject>(HOT_CUP,player_));
		object.back()->Init(player_.GetSphere().GetPos());
		player_.SetHoldItem(object.back()->GetParam().id_);
		object.back()->ChangeItemState(ITEM_STATE::HOLD);	//�����Ă����ԂɕύX
		isActioned_ = true;
		cupsStockCnt_--;	//�݌ɂ����炷
	}

	//�A�C�X�J�b�v�p���b�N�ɃC���^���N�g�����Ƃ��̏���
	if (rackName == ICE_CUP_RACK && ins.IsInputTriggered("Interact"))
	{
		//SE�Đ�
		sound.Play(SoundManager::SOUND::PICK_UP);
		//�A�C�X�J�b�v�����o��
		object.emplace_back(std::make_unique<ItemObject>(ICE_CUP, player_));
		object.back()->Init(player_.GetSphere().GetPos());
		player_.SetHoldItem(object.back()->GetParam().id_);
		object.back()->ChangeItemState(ITEM_STATE::HOLD);	//�����Ă����ԂɕύX
		isActioned_ = true;
		cupsStockCnt_--;	//�݌ɂ����炷
	}

	//�X�C�[�c�i�x���[�j�p���b�N�ɃC���^���N�g�����Ƃ��̏���
	if (rackName == BERRY_SWEETS_RACK && ins.IsInputTriggered("Interact"))
	{
		//SE�Đ�
		sound.Play(SoundManager::SOUND::PICK_UP);
		//�X�C�[�c�����o��
		object.emplace_back(std::make_unique<ItemObject>(BERRY_SWEETS, player_));
		object.back()->Init(player_.GetSphere().GetPos());
		object.back()->SetScale({ SWEETS_SCALE,SWEETS_SCALE,SWEETS_SCALE }); // �X�C�[�c�̃T�C�Y�𒲐�
		player_.SetHoldItem(object.back()->GetParam().id_);
		object.back()->ChangeItemState(ITEM_STATE::HOLD);	//�����Ă����ԂɕύX
		isActioned_ = true;
		sweetsStockCnt_--;	//�݌ɂ����炷
	}

	//�X�C�[�c�i�`���R�j�p���b�N�ɃC���^���N�g�����Ƃ��̏���
	if (rackName == CHOCO_SWEETS_RACK && ins.IsInputTriggered("Interact"))
	{
		sound.Play(SoundManager::SOUND::PICK_UP);
		//�X�C�[�c�����o��
		object.emplace_back(std::make_unique<ItemObject>(CHOCO_SWEETS, player_));
		object.back()->Init(player_.GetSphere().GetPos());
		object.back()->SetScale({ SWEETS_SCALE,SWEETS_SCALE,SWEETS_SCALE }); // �X�C�[�c�̃T�C�Y�𒲐�
		player_.SetHoldItem(object.back()->GetParam().id_);
		object.back()->ChangeItemState(ITEM_STATE::HOLD);	//�����Ă����ԂɕύX
		isActioned_ = true;
		sweetsStockCnt_--;	//�݌ɂ����炷
	}
}

void RackObject::AddStock(void)
{
	//�݌ɂ�����ꍇ�͏������Ȃ�
	if (hasStock_) return;
	
	auto& ins = InputManager::GetInstance();
	auto& sound = SoundManager::GetInstance();
	if (ins.IsInputPressed("Interact"))
	{
		gaugeUI_->SetActive(true);
		addInterval_ += SceneManager::GetInstance().GetDeltaTime();
		gaugeUI_->Update();
	}
	//�X�C�[�c�̍݌ɒǉ�����
	if(GetParam().id_ == BERRY_SWEETS_RACK ||
		GetParam().id_ == CHOCO_SWEETS_RACK)
	{
		//�X�C�[�c�̍݌ɂ�ǉ�
		if (addInterval_ > ADD_INTERVAL)
		{
			addInterval_ = 0.0f;
			sweetsStockCnt_++;
			sound.Play(SoundManager::SOUND::ADD_STOCK);
		}
		//�X�C�[�c�̍݌ɂ��ő�ɒB������A�݌ɂ���ɐݒ�
		if (sweetsStockCnt_ >= SWEETS_STOCK_MAX)
		{
			hasStock_ = true;
			gaugeUI_->Reset();
			stockIconUI_->SetActive(false);
			return;
		}
	}//�J�b�v�̍݌ɒǉ�����
	else if(GetParam().id_ == HOT_CUP_RACK ||
			GetParam().id_ == ICE_CUP_RACK)
	{
		//�J�b�v�̍݌ɂ�ǉ�
		if (addInterval_ > ADD_INTERVAL)
		{
			addInterval_ = 0.0f;
			cupsStockCnt_++;
			sound.Play(SoundManager::SOUND::ADD_STOCK);
		}
		//�J�b�v�̍݌ɂ��ő�ɒB������A�݌ɂ���ɐݒ�
		if (cupsStockCnt_ >= CUP_STOCK_MAX)
		{
			hasStock_ = true;
			gaugeUI_->Reset();
			stockIconUI_->SetActive(false);
			return;
		}
	}
}

void RackObject::Init(VECTOR pos, float rotY, VECTOR scale)
{
	StageObject::Init(pos, rotY, scale);
	//UI�̃T�C�Y�ƈʒu
	const float iconUISize = 70.0f;	//UI�̃T�C�Y
	const VECTOR iconUIPos = { 0.0f, 60.0f, 0.0f };	//UI�̈ʒu
	//�X�C�[�c�pUI�̏�����
	iconUI_ = std::make_unique<IconUI>(iconUIPos,
		transform_.pos, ResourceManager::SRC::INTERACT);
	iconUI_->SetActive(false);
	iconUI_->Init();
	iconUI_->SetUISize(iconUISize);
	UIManager::GetInstance().AddIconUI(iconUI_.get());	//UI�Ǘ��ɒǉ�

	stockIconUI_ = std::make_unique<IconUI>(iconUIPos,
	transform_.pos, ResourceManager::SRC::STOCK_ICON);
	stockIconUI_->SetActive(false);
	stockIconUI_->Init();
	stockIconUI_->SetUISize(iconUISize);
	UIManager::GetInstance().AddIconUI(stockIconUI_.get());	//UI�Ǘ��ɒǉ�

	//�������SRC�ɕϊ����ă��f���ݒ�
	ResourceManager::SRC srcType = ResourceManager::SRC::NONE;
	if (param_.id_ == BERRY_SWEETS_RACK)
	{
		srcType = ResourceManager::SRC::SWEETS_BERRY; //�x���[�̃��b�NID��ݒ�
		cupsStockCnt_ = 0;	//�J�b�v�̍݌ɂ�0�ɂ���
	}
	else if (param_.id_ == CHOCO_SWEETS_RACK)
	{
		srcType = ResourceManager::SRC::SWEETS_CHOCO; //�`���R���[�g���b�NID��ݒ�
		cupsStockCnt_ = 0;	//�J�b�v�̍݌ɂ�0�ɂ���
	}

	if (srcType != ResourceManager::SRC::NONE)
	{
		//�Q�[�WUI�̃T�C�Y
		const float sweetsGaugeWidth = 50.0f;	//UI�̕�	
		const float sweetsGaugeHeight = 5.0f;	//UI�̍���
		//�X�C�[�c�pUI�̏�����
		gaugeUI_ = std::make_unique<GaugeUI>(false, ADD_INTERVAL * SWEETS_STOCK_MAX);
		gaugeUI_->Init();
		gaugeUI_->SetUISize(sweetsGaugeWidth, sweetsGaugeHeight);
		VECTOR uiPos = transform_.pos;
		uiPos.y += SWEETS_UI_OFFSET_Y;	//UI�̈ʒu�𒲐�
		gaugeUI_->SetPos(uiPos); // UI�̈ʒu��ݒ�
		UIManager::GetInstance().AddGaugeUI(gaugeUI_.get());	//UI�Ǘ��ɒǉ�

		// �e�X�C�[�c�̊���W����̃I�t�Z�b�g��z��Œ�`
		const VECTOR sweetsOffsets[] = {
			{SWEETS_HALF_WIDTH,  SWEETS_HEIGHT_OFFSET, SWEETS_Z_BACK_OFFSET},
			{-SWEETS_HALF_WIDTH,  SWEETS_HEIGHT_OFFSET, SWEETS_Z_BACK_OFFSET},
			{-SWEETS_HALF_WIDTH,  SWEETS_HEIGHT_OFFSET,  SWEETS_Z_FRONT_OFFSET},
			{SWEETS_HALF_WIDTH,  SWEETS_HEIGHT_OFFSET,  SWEETS_Z_FRONT_OFFSET}
		};

		for (int i = 0; i < sweetsStockCnt_; ++i)
		{
			//���f���̊�{�ݒ�
			sweetsOfRack_[i].SetModel(ResourceManager::GetInstance().LoadModelDuplicate(srcType));
			sweetsOfRack_[i].scl = CommonUtility::VECTOR_ONE;
			VECTOR sweetsPos = VAdd(pos, sweetsOffsets[i]);
			VECTOR rotPos = CommonUtility::RotXZPos(transform_.pos, sweetsPos, CommonUtility::Deg2RadF(rotY));
			sweetsOfRack_[i].pos = rotPos;
			sweetsOfRack_[i].quaRot = Quaternion();
			sweetsOfRack_[i].quaRotLocal =
				Quaternion::Euler({ CommonUtility::Deg2RadF(SWEETS_ROT_X),CommonUtility::Deg2RadF(rotY), 0.0f });
			sweetsOfRack_[i].Update();
		}
	}

	if (srcType != ResourceManager::SRC::NONE)return;

	const float cupGaugeWidth = 70.0f;	//UI�̕�
	const float cupGaugeHeight = 10.0f;	//UI�̍���
	//�J�b�v�pUI�̏�����
	gaugeUI_ = std::make_unique<GaugeUI>(false, ADD_INTERVAL * CUP_STOCK_MAX);
	gaugeUI_->Init();
	gaugeUI_->SetUISize(cupGaugeWidth, cupGaugeHeight);
	VECTOR uiPos = transform_.pos;
	uiPos.y -= CUP_UI_OFFSET_Y;	//UI�̈ʒu�𒲐�
	gaugeUI_->SetPos(uiPos); // UI�̈ʒu��ݒ�
	UIManager::GetInstance().AddGaugeUI(gaugeUI_.get());	//UI�Ǘ��ɒǉ�

	//�ݒ肳��Ă��Ȃ�������J�b�v���f����ݒ肷��
	if (param_.id_ == HOT_CUP_RACK)
	{
		srcType = ResourceManager::SRC::HOTCUP; // �z�b�g�J�b�v���b�NID��ݒ�
		sweetsStockCnt_ = 0;
	}
	else if (param_.id_ == ICE_CUP_RACK)
	{
		srcType = ResourceManager::SRC::ICECUP; // �A�C�X�J�b�v���b�NID��ݒ�
		sweetsStockCnt_ = 0;
	}

	// �e�J�b�v�̊���W����̃I�t�Z�b�g��z��Œ�`
	float cupHeightOffset = 8.0f;
	const VECTOR cupOffsets[] = {
		{CUPS_HALF_WIDTH,  CUPS_HEIGHT_OFFSET,  CUPS_Z_OFFSET},
		{-CUPS_HALF_WIDTH, CUPS_HEIGHT_OFFSET,  CUPS_Z_OFFSET},
		//2�i�ڂ̃J�b�v
		{CUPS_HALF_WIDTH,  CUPS_HEIGHT_OFFSET + cupHeightOffset,  CUPS_Z_OFFSET},
		{-CUPS_HALF_WIDTH, CUPS_HEIGHT_OFFSET + cupHeightOffset,  CUPS_Z_OFFSET},
		//3�i�ڂ̃J�b�v								�{���炷
		{CUPS_HALF_WIDTH,  CUPS_HEIGHT_OFFSET + (cupHeightOffset * 2.0f),  CUPS_Z_OFFSET}
	};

	for (int i = 0; i < cupsStockCnt_; ++i)
	{
		//���f���̊�{�ݒ�
		cupesOfRack_[i].SetModel(ResourceManager::GetInstance().LoadModelDuplicate(srcType));
		cupesOfRack_[i].scl = CommonUtility::VECTOR_ONE;
		VECTOR cupsPos = VAdd(pos, cupOffsets[i]);
		VECTOR rotPos = CommonUtility::RotXZPos(transform_.pos, cupsPos, CommonUtility::Deg2RadF(rotY));
		cupesOfRack_[i].pos = rotPos;
		cupesOfRack_[i].quaRot = Quaternion();
		cupesOfRack_[i].quaRotLocal =
			Quaternion::Euler({ CommonUtility::Deg2RadF(180.0f), CommonUtility::Deg2RadF(rotY), 0.0f });
		cupesOfRack_[i].Update();
	}
}

void RackObject::Update(void)
{
	//�A�C�R���͔�\���ɂ��Ă���
	iconUI_->SetActive(false);
	//�݌ɂ��Ȃ��ꍇ��!�}�[�N�̃A�C�R����\��
	if(sweetsStockCnt_ <= 0 && cupsStockCnt_ <= 0)
	{
		stockIconUI_->SetActive(true);
		hasStock_ = false;
	}
	//���f���̍X�V
	for(auto& sweets : sweetsOfRack_)
	{
		sweets.Update();
	}
	//���f���̍X�V
	for(auto& cups : cupesOfRack_)
	{
		cups.Update();
	}

	//�ʏ�̍X�V����
	StageObject::Update();
}

void RackObject::Draw(void)
{
	//�e�N���X�̕`��
	StageObject::Draw();

	//���b�N�̒��̃J�b�v���f����`��
	for (int i = 0; i < cupsStockCnt_; ++i)
	{
		MV1DrawModel(cupesOfRack_[i].modelId);
	}
	//���b�N�̒��̃X�C�[�c���f����`��
	for (int i = 0; i < sweetsStockCnt_; ++i)
	{
		MV1DrawModel(sweetsOfRack_[i].modelId);
	}
}
