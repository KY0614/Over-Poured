#include <DxLib.h>
#include "../Application.h"
#include "../Manager/GameSystem/SoundManager.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/InputManager.h"
#include "../Manager/Generic/ResourceManager.h"
#include "TutorialScene.h"

namespace
{
	//�w�i�摜�֘A
	const int BACK_IMG_MARGINE = 250;	//�������W���班�������Ԋu��������
	const int BACK_IMG_SCALE = 500;		//�w�i�摜�̑傫��
	const int CURSOR_IMG_MARGINE = 300;	//�������W���班�������Ԋu��������

	//�J�[�\���_�Ŋ֘A
	const float HIGH_LIGHT_INTERVAL = 1.2f;	//�_�ł̊Ԋu
	//�_�ł̃t���[����
	const int BLINK_FRAME = 60;
	//�_�Ő؂�ւ��̃t���[����
	const int BLINK_TOGGLE_FRAME = 2;

	//���S�摜�̑傫��
	static constexpr int LOGO_HEIGHT = 1024;

	//�����p�摜�֘A
	const int TUTORIAL_IMG_MAX_NUM = 3;		//�����p�摜�̍ő喇��
	const int INDEX_MAX = TUTORIAL_IMG_MAX_NUM - 1;	//�����摜�̍ő�C���f�b�N�X��(�O����Ȃ̂łP�����Ă���)
}

TutorialScene::TutorialScene(void) :
	update_(&TutorialScene::UpdateOperation),
	draw_(&TutorialScene::DrawOperation)
{
	tutorialImgs_ = nullptr;
	tutorialBackImg_ = 0;
	cursorImg_ = nullptr;
	decoImg_ = 0;
	imgIdx_ = 0;
	blinkTime_ = 0;
	blinkIdx_ = 0;
	pushImg_ = 0;

	isBlinkR_ = false;
	isBlinkL_ = false;
	isPushButton_ = false;
}

TutorialScene::~TutorialScene(void)
{
}

void TutorialScene::Init(void)
{
	//�T�E���h������
	InitSound();
	//�摜�ǂݍ���
	LoadImages();

	//�J�[�\���_�ŏ�����
	isBlinkR_ = true;
}

void TutorialScene::Update(void)
{
	(this->*update_)();
}

void TutorialScene::Draw(void)
{
	(this->*draw_)();
}

void TutorialScene::UpdateOperation(void)
{
	//�J�[�\���_��
	CursorHightlight();
	//�y�[�W����
	ProcessPageChange();
}

void TutorialScene::DrawOperation(void)
{
	//�摜�`��
	ImageDraw();
}

void TutorialScene::LoadImages(void)
{
	//�����p�摜�ǂݍ���
	tutorialImgs_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::TUTORIAL).handleIds_;

	//�����w�i�摜�ǂݍ���
	tutorialBackImg_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::TUTORIAL_BACK).handleId_;

	//�J�[�\���w�i�摜�ǂݍ���
	cursorImg_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::CURSOR_R).handleIds_;

	//�����摜�ǂݍ���
	decoImg_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::PINK_BANNER).handleId_;

	//�摜�ǂݍ���
	pushImg_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::PUSH_SPACE).handleId_;

}

void TutorialScene::InitSound(void)
{
	auto& sound = SoundManager::GetInstance();
	sound.Add(SoundManager::TYPE::SE, SoundManager::SOUND::TUTORIAL,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::TUTORIAL_BGM).handleId_);
	sound.AdjustVolume(SoundManager::SOUND::TUTORIAL, 256 / 2);
	sound.Play(SoundManager::SOUND::TUTORIAL);

	//�y�[�W��i�܂���SE
	sound.Add(SoundManager::TYPE::SE, SoundManager::SOUND::NEXT_PAGE,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::NEXT_PAGE).handleId_);
	sound.AdjustVolume(SoundManager::SOUND::NEXT_PAGE, 256 / 2);

	//�y�[�W��߂点��SE
	sound.Add(SoundManager::TYPE::SE, SoundManager::SOUND::RETURN_PAGE,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::RETURN_PAGE).handleId_);
	sound.AdjustVolume(SoundManager::SOUND::RETURN_PAGE, 256 / 2);

	//�y�[�W���i�܂Ȃ�SE
	sound.Add(SoundManager::TYPE::SE, SoundManager::SOUND::NOT_PAGE,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::NOT_PAGE).handleId_);
	sound.AdjustVolume(SoundManager::SOUND::NOT_PAGE, 256 / 2);

	//�J�X�J�����R����
	sound.Add(SoundManager::TYPE::SE, SoundManager::SOUND::OPEN_DOOR,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::OPEN_DOOR).handleId_);
	sound.AdjustVolume(SoundManager::SOUND::OPEN_DOOR, 256 / 2);
}

void TutorialScene::CursorHightlight(void)
{
	//�_�Ŏ��ԍX�V
	blinkTime_++;
	//�_�ŃC���f�b�N�X�X�V
	blinkIdx_ = (blinkTime_ / BLINK_FRAME) % BLINK_TOGGLE_FRAME;

	//�_�ŕ\�L
	isBlinkL_ = false;
	isBlinkR_ = false;
	//�y�[�W�̒[�ɂ���ē_�ł�����J�[�\����ς���
	if (imgIdx_ == 0)	//�ŏ��̃y�[�W
	{
		isBlinkR_ = true;
	}
	else if (imgIdx_ > 0 &&		//�r���̃y�[�W
		imgIdx_ < INDEX_MAX)
	{
		isBlinkL_ = true;
		isBlinkR_ = true;
	}
	else {				//�Ō�̃y�[�W
		isBlinkL_ = true;
		isPushButton_ = true;
	}
}

void TutorialScene::ProcessPageChange(void)
{
	InputManager& ins = InputManager::GetInstance();
	SoundManager& sound = SoundManager::GetInstance();

	//�V�[���J��(�`���[�g���A���̃y�[�W���[�܂ł�������j
	if (isPushButton_ &&
		ins.IsInputTriggered("Interact"))
	{
		sound.Play(SoundManager::SOUND::OPEN_DOOR);
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME);
		return;
	}

	//�y�[�W��i�܂���
	if (ins.IsInputTriggered("Right"))
	{
		if (imgIdx_ < INDEX_MAX)
		{
			sound.Play(SoundManager::SOUND::NEXT_PAGE);
			imgIdx_++;
		}
		else
		{//�y�[�W���Ȃ��Ƃ��͈ႤSE
			imgIdx_ = INDEX_MAX;
			sound.Play(SoundManager::SOUND::NOT_PAGE);
		}
	}
	//�y�[�W��߂点��
	if (ins.IsInputTriggered("Left"))
	{
		if (imgIdx_ > 0)
		{
			sound.Play(SoundManager::SOUND::RETURN_PAGE);
			imgIdx_--;
		}
		else
		{//�y�[�W���Ȃ��Ƃ��͈ႤSE
			imgIdx_ = 0;
			sound.Play(SoundManager::SOUND::NOT_PAGE);
		}
	}
}

void TutorialScene::ImageDraw(void)
{
	//��ʂ̑傫���ɍ��킹�Ċg�嗦��ς���
	float aspectRatio = static_cast<float>(Application::SCREEN_SIZE_Y) /
		static_cast<float>(Application::SCREEN_MAX_SIZE_Y);
	//�����摜�̑傫��
	const float size = 0.8f;
	//�����̊g�嗦�iY���̂ݕς���j
	const float decoScaleY = 2.5f;
	//�����摜�̂��炷�ʒu
	const int backImgMarginY = 20;

	//�����摜�w�i�p
	DrawRotaGraph(Application::SCREEN_SIZE_X / 2,
		Application::SCREEN_SIZE_Y/2 - backImgMarginY,
		aspectRatio, 0.0f,
		tutorialBackImg_,
		true);
	//�����i���j
	DrawRotaGraph3(BACK_IMG_MARGINE, BACK_IMG_MARGINE,
		BACK_IMG_SCALE / 2, BACK_IMG_SCALE / 2,
		aspectRatio, aspectRatio * decoScaleY,
		0.0f,
		decoImg_,
		true, false);
	//�����i�E�j
	DrawRotaGraph3(
		Application::SCREEN_SIZE_X - BACK_IMG_MARGINE,
		BACK_IMG_MARGINE,
		BACK_IMG_SCALE / 2, BACK_IMG_SCALE / 2,
		aspectRatio, aspectRatio * decoScaleY,
		0.0f,
		decoImg_,
		true, false);
	//�����摜
	DrawRotaGraph(Application::SCREEN_SIZE_X / 2,
		Application::SCREEN_SIZE_Y / 2 ,
		aspectRatio * size, 0.0f,
		tutorialImgs_[imgIdx_],
		true);
	//�_�ŕ\��
	if (isBlinkR_)
	{
		//�E�p
		DrawRotaGraph((Application::SCREEN_SIZE_X - CURSOR_IMG_MARGINE),
			(Application::SCREEN_SIZE_Y - CURSOR_IMG_MARGINE / 2),
			aspectRatio, 0.0f,
			cursorImg_[blinkIdx_],
			true
		);
	}
	else
	{
		//���]�p(�E�p�j
		DrawRotaGraph((Application::SCREEN_SIZE_X - CURSOR_IMG_MARGINE),
			(Application::SCREEN_SIZE_Y - CURSOR_IMG_MARGINE / 2),
			aspectRatio, 0.0f,
			cursorImg_[1],
			true
		);
	}
	if (isBlinkL_)
	{
		//���]�p(���p�j
		DrawRotaGraph(CURSOR_IMG_MARGINE,
			Application::SCREEN_SIZE_Y - CURSOR_IMG_MARGINE / 2,
			aspectRatio, 0.0f,
			cursorImg_[blinkIdx_],
			true, true
		);
	}
	else
	{
		//���]�p(���p�j
		DrawRotaGraph(CURSOR_IMG_MARGINE,
			Application::SCREEN_SIZE_Y - CURSOR_IMG_MARGINE / 2,
			aspectRatio, 0.0f,
			cursorImg_[1],
			true, true
		);
	}
	//pushspace�̈ʒu�i�摜�̑傫�����l�����āj
	const int logoPosY = static_cast<int>(static_cast<float>(LOGO_HEIGHT / 2) * aspectRatio);
	//pushspace�̊g�嗦
	const float logoScale = 2.0f;
	if (isPushButton_)
	{
		//pushspace�̉摜
		DrawRotaGraph(
			Application::SCREEN_SIZE_X / 2,
			Application::SCREEN_SIZE_Y / 2 + logoPosY,
			logoScale, 0.0, pushImg_, true);
	}
}
