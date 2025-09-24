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
	//初期化処理
	uis_.clear();
	popUpUIs_.clear();
}

void UIManager::Update(void)
{
	// 全てのUIをまとめて更新
	for (auto& ui : uis_)
	{
		ui.lock()->Update();
	}
}

void UIManager::PopUpUIUpdate(void)
{
	//ポップアップUIをまとめて更新
	for (auto& ui : popUpUIs_)
	{
		ui->Update();
	}
}

void UIManager::Draw(void)
{
	//描画するUIが無ければ処理を抜ける
	if (uis_.empty() && popUpUIs_.empty())return;

	// 全てのUIをまとめて描画
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
	//解放処理
	uis_.clear();
	popUpUIs_.clear();
}

void UIManager::Destroy(void)
{
	//シングルトンの解放
	uis_.clear();
	popUpUIs_.clear();
	delete instance_;
}

void UIManager::AddPopUpUI(int score, const VECTOR& pos)
{
	// ポップアップUIを追加(スコア用）
	popUpUIs_.emplace_back(std::make_unique<PopUpUI>(score, pos));
	popUpUIs_.back()->Init();	//初期化
}
