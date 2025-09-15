#include "../../../Manager/GameSystem/SoundManager.h"
#include "../../../Manager/Generic/SceneManager.h"
#include "../../../Manager/Generic/InputManager.h"
#include "../../../Manager/Generic/ResourceManager.h"
#include "../../../Utility/AsoUtility.h"
#include "../../Common/Sphere.h"
#include "../Object/Player.h"
#include "../../UI/GaugeUI.h"
#include "../../UI/IconUI.h"
#include "../../UI/UIManager.h"
#include "ItemObject.h"
#include "RackObject.h"

RackObject::RackObject(const std::string objId,
	const float width, const float height,
	const float depth, Player& player) :
	StageObject(objId, width, height, depth, player)
{
	sweetsStockCnt_ = SWEETS_STOCK_MAX;
	cupsStockCnt_ = CUP_STOCK_MAX;
	hasStock_ = true;
	addInterval_ = 0.0f;
}

void RackObject::PickUp(std::string rackName,std::vector<std::unique_ptr<StageObject>>& object)
{
	if (!hasStock_)return;
	isInteract_ = true;
	auto& ins = InputManager::GetInstance();
	auto& sound = SoundManager::GetInstance();
	//�z�b�g�J�b�v�p���b�N�ɃC���^���N�g�����Ƃ��̏���
	if (rackName == "Cup_Hot_Rack" && ins.IsInputTriggered("Interact"))
	{
		sound.Play(SoundManager::SOUND::PICK_UP);
		object.emplace_back(std::make_unique<ItemObject>("Hot_Cup", 40.0f, 30.0f, 40.0f, player_));
		object.back()->Init(player_.GetSphere().GetPos());
		player_.SetHoldItem(object.back()->GetParam().id_);
		object.back()->ChangeItemState(ITEM_STATE::HOLD);
		isActioned_ = true;
		cupsStockCnt_--;
	}

	//�A�C�X�J�b�v�p���b�N�ɃC���^���N�g�����Ƃ��̏���
	if (rackName == "Cup_Ice_Rack" && ins.IsInputTriggered("Interact"))
	{
		sound.Play(SoundManager::SOUND::PICK_UP);
		object.emplace_back(std::make_unique<ItemObject>("Ice_Cup", 40.0f, 30.0f, 40.0f, player_));
		object.back()->Init(player_.GetSphere().GetPos());
		player_.SetHoldItem(object.back()->GetParam().id_);
		object.back()->ChangeItemState(ITEM_STATE::HOLD);
		isActioned_ = true;
		cupsStockCnt_--;
	}

	//�X�C�[�c�i�x���[�j�p���b�N�ɃC���^���N�g�����Ƃ��̏���
	if (rackName == "Sweets_Strawberry_Rack" && ins.IsInputTriggered("Interact"))
	{
		sound.Play(SoundManager::SOUND::PICK_UP);
		//�X�C�[�c�����o��
		object.emplace_back(std::make_unique<ItemObject>("Sweets_Strawberry", 40.0f, 30.0f, 40.0f, player_));
		object.back()->Init(player_.GetSphere().GetPos());
		object.back()->SetScale({ 1.2f,1.2f,1.2f }); // �X�C�[�c�̃T�C�Y�𒲐�
		player_.SetHoldItem(object.back()->GetParam().id_);
		object.back()->ChangeItemState(ITEM_STATE::HOLD);
		isActioned_ = true;
		sweetsStockCnt_--;
	}

	//�X�C�[�c�i�`���R�j�p���b�N�ɃC���^���N�g�����Ƃ��̏���
	if (rackName == "Sweets_Choco_Rack" && ins.IsInputTriggered("Interact"))
	{
		sound.Play(SoundManager::SOUND::PICK_UP);
		//�X�C�[�c�����o��
		object.emplace_back(std::make_unique<ItemObject>("Sweets_Choco", 40.0f, 30.0f, 40.0f, player_));
		object.back()->Init(player_.GetSphere().GetPos());
		object.back()->SetScale({ 1.2f,1.2f,1.2f }); // �X�C�[�c�̃T�C�Y�𒲐�
		player_.SetHoldItem(object.back()->GetParam().id_);
		object.back()->ChangeItemState(ITEM_STATE::HOLD);
		isActioned_ = true;
		sweetsStockCnt_--;
	}
}

void RackObject::AddStock(int addStockNum)
{
	if (hasStock_) return;
	
	auto& ins = InputManager::GetInstance();
	auto& sound = SoundManager::GetInstance();
	if (ins.IsInputPressed("Interact"))
	{
		gaugeUI_->SetActive(true);
		addInterval_ += SceneManager::GetInstance().GetDeltaTime();
		gaugeUI_->Update();
	}

	if(GetParam().id_ == "Sweets_Strawberry_Rack" ||
		GetParam().id_ == "Sweets_Choco_Rack")
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
	}
	else if(GetParam().id_ == "Cup_Hot_Rack" ||
			GetParam().id_ == "Cup_Ice_Rack")
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

	//�X�C�[�c�pUI�̏�����
	iconUI_ = std::make_unique<IconUI>(VGet(0.0f, height_ + 60.0f, 0.0f),
		transform_.pos, ResourceManager::SRC::INTERACT);
	iconUI_->SetActive(false);
	iconUI_->Init();
	iconUI_->SetUISize(70.0f);
	UIManager::GetInstance().AddIconUI(iconUI_.get());

	stockIconUI_ = std::make_unique<IconUI>(VGet(0.0f, height_ + 60.0f, 0.0f),
	transform_.pos, ResourceManager::SRC::STOCK_ICON);
	stockIconUI_->SetActive(false);
	stockIconUI_->Init();
	stockIconUI_->SetUISize(70.0f);
	UIManager::GetInstance().AddIconUI(stockIconUI_.get());

	//�������SRC�ɕϊ����ă��f���ݒ�
	ResourceManager::SRC srcType = ResourceManager::SRC::NONE;
	if (param_.id_ == "Sweets_Strawberry_Rack")
	{
		srcType = ResourceManager::SRC::SWEETS_BERRY; // �f�t�H���g�̃��b�NID��ݒ�
		cupsStockCnt_ = 0;
	}
	else if (param_.id_ == "Sweets_Choco_Rack")
	{
		srcType = ResourceManager::SRC::SWEETS_CHOCO; // �`���R���[�g���b�NID��ݒ�
		cupsStockCnt_ = 0;
	}

	if (srcType != ResourceManager::SRC::NONE)
	{
		//�X�C�[�c�pUI�̏�����
		gaugeUI_ = std::make_unique<GaugeUI>(false, ADD_INTERVAL * SWEETS_STOCK_MAX);
		gaugeUI_->Init();
		gaugeUI_->SetUISize(50.0f, 5.0f);
		VECTOR uiPos = transform_.pos;
		uiPos.y += SWEETS_UI_OFFSET_Y;	//UI�̈ʒu�𒲐�
		gaugeUI_->SetPos(uiPos); // UI�̈ʒu��ݒ�
		UIManager::GetInstance().AddGaugeUI(gaugeUI_.get());
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
			sweetsOfRack_[i].scl = AsoUtility::VECTOR_ONE;
			VECTOR sweetsPos = VAdd(pos, sweetsOffsets[i]);
			VECTOR rotPos = AsoUtility::RotXZPos(transform_.pos, sweetsPos, AsoUtility::Deg2RadF(rotY));
			sweetsOfRack_[i].pos = rotPos;
			sweetsOfRack_[i].quaRot = Quaternion();
			sweetsOfRack_[i].quaRotLocal =
				Quaternion::Euler({ AsoUtility::Deg2RadF(SWEETS_ROT_X),AsoUtility::Deg2RadF(rotY), 0.0f });
			sweetsOfRack_[i].Update();
		}
	}

	if (srcType != ResourceManager::SRC::NONE)return;

	//�J�b�v�pUI�̏�����
	gaugeUI_ = std::make_unique<GaugeUI>(false, ADD_INTERVAL * CUP_STOCK_MAX);
	gaugeUI_->Init();
	gaugeUI_->SetUISize(70.0f, 10.0f);
	VECTOR uiPos = transform_.pos;
	uiPos.y -= CUP_UI_OFFSET_Y;	//UI�̈ʒu�𒲐�
	gaugeUI_->SetPos(uiPos); // UI�̈ʒu��ݒ�
	UIManager::GetInstance().AddGaugeUI(gaugeUI_.get());
	//�ݒ肳��Ă��Ȃ�������J�b�v���f����ݒ肷��
	if (param_.id_ == "Cup_Hot_Rack")
	{
		srcType = ResourceManager::SRC::HOTCUP; // �z�b�g�J�b�v���b�NID��ݒ�
		sweetsStockCnt_ = 0;
	}
	else if (param_.id_ == "Cup_Ice_Rack")
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
		cupesOfRack_[i].scl = AsoUtility::VECTOR_ONE;
		VECTOR cupsPos = VAdd(pos, cupOffsets[i]);
		VECTOR rotPos = AsoUtility::RotXZPos(transform_.pos, cupsPos, AsoUtility::Deg2RadF(rotY));
		cupesOfRack_[i].pos = rotPos;
		cupesOfRack_[i].quaRot = Quaternion();
		cupesOfRack_[i].quaRotLocal =
			Quaternion::Euler({ AsoUtility::Deg2RadF(180.0f), AsoUtility::Deg2RadF(rotY), 0.0f });
		cupesOfRack_[i].Update();
	}
}

void RackObject::Update(void)
{
	if (isInteract_)iconUI_->SetActive(true);
	else iconUI_->SetActive(false);
	if(sweetsStockCnt_ <= 0 && cupsStockCnt_ <= 0)
	{
		stockIconUI_->SetActive(true);
		hasStock_ = false;
	}

	for(auto& sweets : sweetsOfRack_)
	{
		sweets.Update();
	}

	for(auto& cups : cupesOfRack_)
	{
		cups.Update();
	}

	StageObject::Update();
}

void RackObject::Draw(void)
{
	StageObject::Draw();

	for (int i = 0; i < cupsStockCnt_; ++i)
	{
		//���f���̕`��
		MV1DrawModel(cupesOfRack_[i].modelId);
	}

	for (int i = 0; i < sweetsStockCnt_; ++i)
	{
		//���f���̕`��
		MV1DrawModel(sweetsOfRack_[i].modelId);
	}

}
