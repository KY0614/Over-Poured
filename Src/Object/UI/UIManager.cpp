#include "PopUpUI.h"
#include "UIManager.h"

UIManager* UIManager::instance_ = nullptr;

void UIManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new UIManager();
	}
	instance_->Init();
}

UIManager& UIManager::GetInstance(void)
{
	if (instance_ == nullptr)
	{
		UIManager::CreateInstance();
	}
	return *instance_;
}

UIManager::UIManager(void)
{
	uis_.clear();
	popUpUIs_.clear();
}

UIManager::~UIManager(void)
{
	uis_.clear();
	popUpUIs_.clear();
}

void UIManager::Init(void)
{
	//����������
	uis_.clear();
	popUpUIs_.clear();
}

void UIManager::Update(void)
{
	// �S�Ă�UI���܂Ƃ߂čX�V
	for (auto& ui : uis_)
	{
		ui.lock()->Update();
	}
}

void UIManager::PopUpUIUpdate(void)
{
	//�|�b�v�A�b�vUI���܂Ƃ߂čX�V
	for (auto& ui : popUpUIs_)
	{
		ui->Update();
	}
}

void UIManager::Draw(void)
{
	//�`�悷��UI��������Ώ����𔲂���
	if (uis_.empty() && popUpUIs_.empty())return;

	// �S�Ă�UI���܂Ƃ߂ĕ`��
	for (auto& ui : uis_)
	{
		ui.lock()->Draw();
	}
	for (auto& ui : popUpUIs_)
	{
		ui->Draw();
	}
}

void UIManager::Release(void)
{
	//�������
	uis_.clear();
	popUpUIs_.clear();
}

void UIManager::Destroy(void)
{
	//�V���O���g���̉��
	uis_.clear();
	popUpUIs_.clear();
	delete instance_;
}

void UIManager::AddPopUpUI(int score, const VECTOR& pos)
{
	// �|�b�v�A�b�vUI��ǉ�(�X�R�A�p�j
	popUpUIs_.emplace_back(std::make_unique<PopUpUI>(score, pos));
	popUpUIs_.back()->Init();	//������
}
