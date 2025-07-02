#include "../../../Manager/Generic/SceneManager.h"
#include "../../../Manager/Generic/InputManager.h"
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
	stockCnt_ = RACK_STOCK_MAX;
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
		stockCnt_--;
	}

	//�A�C�X�J�b�v�p���b�N�ɃC���^���N�g�����Ƃ��̏���
	if (rackName == "Cup_Ice_Rack" && ins.IsInputTriggered("Interact"))
	{
		object.emplace_back(std::make_unique<ItemObject>("Ice_Cup", 40.0f, 30.0f, 40.0f, player_));
		object.back()->Init(player_.GetSphere().GetPos());
		player_.SetHoldItem(object.back()->GetParam().id_);
		object.back()->ChangeItemState(ITEM_STATE::HOLD);
		isActioned_ = true;
		stockCnt_--;
	}

	//�X�C�[�c�i�x���[�j�p���b�N�ɃC���^���N�g�����Ƃ��̏���
	if (rackName == "Sweets_Strawberry_Rack" && ins.IsInputTriggered("Interact"))
	{
		//�X�C�[�c�����o��
		object.emplace_back(std::make_unique<ItemObject>("Sweets_Strawberry", 40.0f, 30.0f, 40.0f, player_));
		object.back()->Init(player_.GetSphere().GetPos());
		player_.SetHoldItem(object.back()->GetParam().id_);
		object.back()->ChangeItemState(ITEM_STATE::HOLD);
		isActioned_ = true;
		stockCnt_--;
	}

	//�X�C�[�c�i�`���R�j�p���b�N�ɃC���^���N�g�����Ƃ��̏���
	if (rackName == "Sweets_Choco_Rack" && ins.IsInputTriggered("Interact"))
	{
		//�X�C�[�c�����o��
		object.emplace_back(std::make_unique<ItemObject>("Sweets_Choco", 40.0f, 30.0f, 40.0f, player_));
		object.back()->Init(player_.GetSphere().GetPos());
		player_.SetHoldItem(object.back()->GetParam().id_);
		object.back()->ChangeItemState(ITEM_STATE::HOLD);
		isActioned_ = true;
		stockCnt_--;
	}
}

void RackObject::AddStock(int addStockNum)
{
	if (hasStock_) return;
	
	auto& ins = InputManager::GetInstance();

	if (ins.IsInputPressed("Interact"))
	{
		gaugeUI_->SetActive(true);
		param_.interactTime_ -= SceneManager::GetInstance().GetDeltaTime();
		gaugeUI_->Update();
	}
	else {
		gaugeUI_->Reset();
	}

	if(param_.interactTime_ <= 0.0f)
	{
		param_.interactTime_ = 3.0f;
		stockCnt_ += addStockNum;
		hasStock_ = true;
		gaugeUI_->Reset();
	}
}

void RackObject::Init(VECTOR pos, float rotY)
{
	StageObject::Init(pos, rotY);

	gaugeUI_ = std::make_unique<GaugeUI>(false, param_.interactTime_);
	gaugeUI_->Init();
	gaugeUI_->SetUISize(50.0f,10.0f);
	VECTOR uiPos = transform_.pos;
	uiPos.y -= 30.0f;	//UI�̈ʒu�𒲐�
	gaugeUI_->SetPos(uiPos); // UI�̈ʒu��ݒ�
}

void RackObject::Update(void)
{
	if(stockCnt_ <= 0)
	{
		hasStock_ = false;
	}

	StageObject::Update();
}

void RackObject::Draw(void)
{
	int line = 3;	//�s
	int lineHeight = 30;	//�s

	VECTOR screenPos = ConvWorldPosToScreenPos(GetTransform().pos);
	// �ϊ�����
	DrawFormatString(static_cast<int>(screenPos.x) - 30,
		static_cast<int>(screenPos.y) - 100, GetColor(255, 255, 255),
		L"�݌� %d", stockCnt_);

	StageObject::Draw();

	gaugeUI_->Draw(); // UI��`��
}
