#include <memory>
#include "../Application.h"
#include "../Manager/Generic/ResourceManager.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/InputManager.h"
#include "../Manager/GameSystem/SoundManager.h"
#include "../Scene/TitleScene.h"
#include "../Scene/PauseScene/ExplainScene.h"
#include "../Scene/PauseScene/KeyConfigScene.h"
#include "PauseScene.h"

namespace 
{
	//�w�i�摜�̑傫���i�c���j
	const int BACK_IMG_SCALE = 1080;
	//�o���E���ŃA�j���[�V�����̃t���[����
	const int APPEAR_INTERVAL = 15;
	//�g�̗]��
	const int MARGINE_SIZE = 30;
	//���j���[���X�g�̍���
	const int MNEU_LIST_HEIGHT = 216;
	//���j���[���X�g��X���W
	const int MENU_LIST_POS_X = 280;
	//���j���[���X�g�̊J�nY���W
	const int MENU_START_Y = 200;
	//���j���[���X�g�̊g�嗦
	const float MENU_LIST_SCALE = 0.8f;
	//�I�𒆃��j���[�̃}�[�W��(���ɂ��炷��)
	const int SELECT_MENU_MARGINE = 25;
}

PauseScene::PauseScene(void) :
	update_(&PauseScene::UpdateAppear),
	draw_(&PauseScene::DrawProcess)
{
	menuList_ = {
		L"�`���[�g���A��",
		L"�L�[�ݒ�",
		L"�Q�[���ɖ߂�",
		L"�^�C�g���ɖ߂�",
		L"�Q�[���I��"
	};

	menuFuncTable_ = {
	{L"�`���[�g���A��",[this]()
		{
			std::unique_ptr<ExplainScene> scene = std::make_unique<ExplainScene>();
			SceneManager::GetInstance().PushScene(std::move(scene));
		}
	},
	{ L"�L�[�ݒ�",[this]()
		{
			std::unique_ptr<KeyConfigScene> scene = std::make_unique<KeyConfigScene>();
			SceneManager::GetInstance().PushScene(std::move(scene));
		}
	},
	{ L"�Q�[���ɖ߂�",[this]()
		{
			update_ = &PauseScene::UpdateDisappear;
			draw_ = &PauseScene::DrawProcess;
			return;
		}
	},
	{ L"�^�C�g���ɖ߂�",[this]()
		{
			SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
			return;
		}
	},
	{ L"�Q�[���I��",[this]()
		{
			Application::GetInstance().Destroy();
			return;
		}
	}
	};


	backImg_ = -1;
	menuListImg_ = nullptr;
	menuCursorImg_ = -1;
	//���\�[�X�̏�����
	auto& resM = ResourceManager::GetInstance();
	resM.InitPause();

	frame_ = 0;
	cursorIdx_ = 0;
}

PauseScene::~PauseScene(void)
{
}

void PauseScene::Init(void)
{
	//�T�E���h�̏�����
	InitSound();
	//�摜�̓ǂݍ���
	LoadImages();
}

void PauseScene::Update(void)
{
	(this->*update_)();
}

void PauseScene::Draw(void)
{
	(this->*draw_)();
}

void PauseScene::UpdateAppear(void)
{
	if (++frame_ >= APPEAR_INTERVAL) {
		update_ = &PauseScene::UpdateNormal;
		draw_ = &PauseScene::DrawNormal;
	}
}

void PauseScene::UpdateDisappear(void)
{
	if (--frame_ <= 0) {
		SceneManager::GetInstance().PopScene();
		return;
	}
}

void PauseScene::DrawProcess(void)
{
	//���S���珙�X�ɍL����
	const Application::Size& wSize = Application::GetInstance().GetWindowSize();
	int centerY = wSize.height_ / 2;		//��ʒ��SY
	int frameHalfHeight = (wSize.height_ - MARGINE_SIZE * 2) / 2;	//�g�̍����̔���(�㉺�ɍL����̂Ŕ�����OK)

	//�t�F�[�h����0�`1�܂ł͈̔͂̒l�̊������v�Z
	//�o���E���Ŏ��̍����ω���(0.0�`1.0)
	float rate = static_cast<float>(frame_) /
		static_cast<float>(APPEAR_INTERVAL);
	frameHalfHeight *= rate;
	//�w�i�摜�̕`��
	DrawExtendGraph(MARGINE_SIZE,
		centerY - frameHalfHeight,
		Application::SCREEN_SIZE_X - MARGINE_SIZE,
		centerY + frameHalfHeight,
		backImg_, true);
}

void PauseScene::UpdateNormal(void)
{	
	auto& sound = SoundManager::GetInstance();
	InputManager& ins = InputManager::GetInstance();
	//�|�[�Y���j���[�����
	if (ins.IsInputTriggered("pause"))
	{
		sound.Play(SoundManager::SOUND::MENU_CLOSE);
		update_ = &PauseScene::UpdateDisappear;
		draw_ = &PauseScene::DrawProcess;
		return;
	}
	//���j���[����
	if (ins.IsInputTriggered("Down"))
	{
		sound.Play(SoundManager::SOUND::NEXT_PAGE);
		cursorIdx_ = (cursorIdx_ + 1) % menuList_.size();
	}
	else if (ins.IsInputTriggered("Up"))
	{
		sound.Play(SoundManager::SOUND::NEXT_PAGE);
		cursorIdx_ = (cursorIdx_ + menuList_.size() - 1) % menuList_.size();
	}
	//����
	if (ins.IsInputTriggered("Interact"))
	{
		sound.Play(SoundManager::SOUND::RETURN_PAGE);
		auto selectedName = menuList_[cursorIdx_];
		menuFuncTable_[selectedName]();
		return;
	}
}

void PauseScene::DrawNormal(void)
{
	//�w�i�摜�̕`��
	DrawExtendGraph(MARGINE_SIZE,
		MARGINE_SIZE,
		Application::SCREEN_SIZE_X - MARGINE_SIZE,
		Application::SCREEN_SIZE_Y - MARGINE_SIZE,
		backImg_, true);

	//���j���[���X�g�̕`��
	DrawMenuList();
}

void PauseScene::DrawMenuList(void)
{
	//��ʂ̑傫���ɍ��킹�Ċg�嗦��ς���
	float scale = static_cast<float>(Application::SCREEN_SIZE_Y) /
		static_cast<float>(Application::SCREEN_MAX_SIZE_Y);

	const int line_start_X = MENU_LIST_POS_X * scale;

	int lineY = MENU_START_Y * scale;

	auto currentStr = menuList_[cursorIdx_];

	for (int i = 0; i < menuList_.size();++i) 
	{
		int lineX = 0;

		//������̕����擾
		int stringWidth = GetDrawStringWidth(menuList_[i].c_str(), menuList_[i].size());

		//�J�[�\����X���W�𓮓I�Ɍv�Z
		int cursor_X = ((Application::SCREEN_SIZE_X / 2 - line_start_X) - (stringWidth));

		if (menuList_[i] == currentStr)
		{
			DrawRotaGraph(cursor_X ,
				lineY,
				scale, 0.0f, menuCursorImg_, true
			);
			lineX += SELECT_MENU_MARGINE * scale;
		}

		DrawRotaGraph(Application::SCREEN_SIZE_X / 2 + lineX * scale,
			(MENU_START_Y * scale) + (MNEU_LIST_HEIGHT * MENU_LIST_SCALE * scale * i),
			scale * MENU_LIST_SCALE, 0.0f, menuListImg_[i], true
		);

		lineY += MNEU_LIST_HEIGHT * MENU_LIST_SCALE * scale;
	}
}

void PauseScene::LoadImages(void)
{
	//�w�i�摜
	backImg_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::TUTORIAL_BACK).handleId_;
	//���j���[���X�g�摜
	menuListImg_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::MENU_LIST).handleIds_;
	//���j���[�J�[�\���摜
	menuCursorImg_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::MENU_CURSOR).handleId_;
}

void PauseScene::InitSound(void)
{
	auto& sound = SoundManager::GetInstance();
	//�J�[�\��SE
	sound.Add(SoundManager::TYPE::SE, SoundManager::SOUND::NEXT_PAGE,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::NEXT_PAGE).handleId_);
	sound.AdjustVolume(SoundManager::SOUND::NEXT_PAGE, VOLUME_MAX / 2);
	//����SE
	sound.Add(SoundManager::TYPE::SE, SoundManager::SOUND::RETURN_PAGE,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::RETURN_PAGE).handleId_);
	sound.AdjustVolume(SoundManager::SOUND::RETURN_PAGE, VOLUME_MAX / 2);

	//���j���[���Ƃ���SE
	sound.Add(SoundManager::TYPE::SE, SoundManager::SOUND::MENU_CLOSE,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::MENU_CLOSE).handleId_);
	sound.AdjustVolume(SoundManager::SOUND::MENU_CLOSE, VOLUME_MAX / 2);
}