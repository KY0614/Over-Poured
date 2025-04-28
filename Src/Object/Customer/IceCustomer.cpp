#include "../../Application.h"
#include "../../Utility/AsoUtility.h"
#include "../../Manager/Generic/SceneManager.h"
#include "../../Manager/Generic/ResourceManager.h"
#include "../Common/AnimationController.h"
#include "IceCustomer.h"

IceCustomer::IceCustomer(void)
{
}

void IceCustomer::Draw(void)
{
	MV1DrawModel(transform_.modelId);
}

void IceCustomer::SetParam(void)
{
	//���f���̊�{�ݒ�
	transform_.SetModel(ResourceManager::GetInstance().LoadModelDuplicate(
		ResourceManager::SRC::CUSTOMER));

	transform_.Update();

	SetType(TYPE::ICE);

	//�e�X�g�Ń��f���̐F��ɂ���
	MV1SetMaterialDifColor(transform_.modelId, 0, GetColorF(0.0f, 0.0f, 1.0f, 1.0f));
}

void IceCustomer::InitAnimation(void)
{
	std::string path = Application::PATH_MODEL + "Customer/";
	animationController_ = std::make_unique<AnimationController>(transform_.modelId);
	animationController_->Add((int)STATE::IDLE, path + "Idle2.mv1", 20.0f);

	animationController_->Play((int)STATE::IDLE);
}
