#include "../../../Manager/Generic/SceneManager.h"
#include "../../../Manager/Generic/InputManager.h"
#include "../../../Manager/Generic/ResourceManager.h"
#include "../../../Utility/AsoUtility.h"
#include "../../Common/Sphere.h"
#include "../Object/Player.h"
#include "../../UI/GaugeUI.h"
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
}

void RackObject::PickUp(std::string rackName,std::vector<std::unique_ptr<StageObject>>& object)
{
	if (!hasStock_)return;

	auto& ins = InputManager::GetInstance();
	//�z�b�g�J�b�v�p���b�N�ɃC���^���N�g�����Ƃ��̏���
	if (rackName == "Cup_Hot_Rack" && ins.IsInputTriggered("Interact"))
	{
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

	if(sweetsStockCnt_ >= SWEETS_STOCK_MAX &&
		cupsStockCnt_ >= CUP_STOCK_MAX)
	{
		hasStock_ = true;
		gaugeUI_->Reset();
		return;
	}

	if (ins.IsInputPressed("Interact"))
	{
		gaugeUI_->SetActive(true);
		addInterval_ += SceneManager::GetInstance().GetDeltaTime();
		gaugeUI_->Update();
	}

	if (addInterval_ > SWEETS_ADD_INTERVAL)
	{
		addInterval_ = 0.0f;
		sweetsStockCnt_++;
		cupsStockCnt_++;
	}
}

void RackObject::Init(VECTOR pos, float rotY)
{
	StageObject::Init(pos, rotY);

	//UI�̏�����
	gaugeUI_ = std::make_unique<GaugeUI>(false, SWEETS_ADD_INTERVAL * SWEETS_STOCK_MAX);
	gaugeUI_->Init();
	gaugeUI_->SetUISize(50.0f, 10.0f);
	VECTOR uiPos = transform_.pos;
	uiPos.y -= 30.0f;	//UI�̈ʒu�𒲐�
	gaugeUI_->SetPos(uiPos); // UI�̈ʒu��ݒ�

	//�������SRC�ɕϊ����ă��f���ݒ�
	ResourceManager::SRC srcType = ResourceManager::SRC::NONE;
	if(param_.id_ == "Sweets_Strawberry_Rack")
	{
		srcType = ResourceManager::SRC::SWEETS_BERRY; // �f�t�H���g�̃��b�NID��ݒ�
	}
	else if(param_.id_ == "Sweets_Choco_Rack")
	{
		srcType = ResourceManager::SRC::SWEETS_CHOCO; // �`���R���[�g���b�NID��ݒ�
	}
	// �e�X�C�[�c�̊���W����̃I�t�Z�b�g��z��Œ�`
	const VECTOR offsets[] = {
		{SWEETS_HALF_WIDTH,  SWEETS_HEIGHT_OFFSET,  SWEETS_Z_FRONT_OFFSET},
		{-SWEETS_HALF_WIDTH,  SWEETS_HEIGHT_OFFSET,  SWEETS_Z_FRONT_OFFSET},
		{-SWEETS_HALF_WIDTH,  SWEETS_HEIGHT_OFFSET, SWEETS_Z_BACK_OFFSET},
		{SWEETS_HALF_WIDTH,  SWEETS_HEIGHT_OFFSET, SWEETS_Z_BACK_OFFSET}
	};

	for(int i = 0; i < sweetsStockCnt_; ++i)
	{
		//���f���̊�{�ݒ�
		sweetsOfRack_[i].SetModel(ResourceManager::GetInstance().LoadModelDuplicate(srcType));
		sweetsOfRack_[i].scl = AsoUtility::VECTOR_ONE;
		sweetsOfRack_[i].pos = VAdd(pos,offsets[i]);
		sweetsOfRack_[i].quaRot = Quaternion();
		sweetsOfRack_[i].quaRotLocal =
			Quaternion::Euler({ AsoUtility::Deg2RadF(SWEETS_ROT_X), 0.0f, 0.0f });
		transform_.MakeCollider(Collider::TYPE::STAGE);
	}

	//srcType���ݒ肳��Ă���Ȃ�A�]�v�ȏ��������Ȃ�
	if (srcType != ResourceManager::SRC::NONE)return;

	//�ݒ肳��Ă��Ȃ�������J�b�v���f����ݒ肷��
	if (param_.id_ == "Cup_Hot_Rack")
	{
		srcType = ResourceManager::SRC::HOTCUP; // �z�b�g�J�b�v���b�NID��ݒ�
	}
	else if (param_.id_ == "Cup_Ice_Rack")
	{
		srcType = ResourceManager::SRC::ICECUP; // �A�C�X�J�b�v���b�NID��ݒ�
	}

	for (auto& cups : cupesOfRack_)
	{
		//���f���̊�{�ݒ�
		cups.SetModel(ResourceManager::GetInstance().LoadModelDuplicate(srcType));
		cups.scl = AsoUtility::VECTOR_ONE;
		cups.pos = pos;
		cups.quaRot = Quaternion();
		cups.quaRotLocal =
			Quaternion::Euler({ 0.0f, AsoUtility::Deg2RadF(rotY), 0.0f });
		transform_.MakeCollider(Collider::TYPE::STAGE);
	}

}

void RackObject::Update(void)
{
	if(sweetsStockCnt_ <= 0 || cupsStockCnt_ <= 0)
	{
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
	//int line = 3;	//�s
	//int lineHeight = 30;	//�s

	//VECTOR screenPos = ConvWorldPosToScreenPos(GetTransform().pos);
	//// �ϊ�����
	//DrawFormatString(static_cast<int>(screenPos.x) - 30,
	//	static_cast<int>(screenPos.y) - 100, GetColor(255, 255, 255),
	//	L"�݌� %d", sweetsStockCnt_);

	StageObject::Draw();

	for(auto& cups : cupesOfRack_)
	{
		//���f���̕`��
		MV1DrawModel(cups.modelId);
	}

	for (int i = 0; i < sweetsStockCnt_; ++i)
	{
		//���f���̕`��
		MV1DrawModel(sweetsOfRack_[i].modelId);
	}

	gaugeUI_->Draw(); // UI��`��
}
