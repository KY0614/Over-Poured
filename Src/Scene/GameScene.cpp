#include <DxLib.h>
#include "../Common/Easing.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/Camera.h"
#include "../Manager/Generic/InputManager.h"
#include "../Manager/Generic/ResourceManager.h"
#include"../Manager/GameSystem/OrderCustomerManager.h"
#include"../Manager/GameSystem/Timer.h"
#include"../Manager/GameSystem/SoundManager.h"
#include "../Object/Common/Collider.h"
#include "../Object/Stage/StageManager.h"
#include "../Object/Player.h"
#include "../Manager/GameSystem//ScoreManager.h"
#include "../Object/UI/UIManager.h"
#include "PauseScene.h"
#include "GameScene.h"

GameScene::GameScene(void) : 
	update_(&GameScene::UpdateCountDown),
	draw_(&GameScene::DrawCountDown)
{
	player_ = nullptr;
	stage_ = nullptr;
	customer_ = nullptr;
	timer_ = nullptr;
	score_ = 0;
	numbersImgs_ = nullptr;
	countImgs_ = nullptr;
	cntDownIdx_ = 0;
	scale_ = 0.0f;
	sclTime_ = 0.0f;
	timeUpImg_ = 0;
	remainderSE_ = false;
}

GameScene::~GameScene(void)
{
}

void GameScene::Init(void)
{
	auto& sound = SoundManager::GetInstance();
	//bgm�ǉ�&�Đ�
	sound.Add(SoundManager::TYPE::BGM, SoundManager::SOUND::GAME,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::GAME_BGM).handleId_);
	sound.AdjustVolume(SoundManager::SOUND::GAME, 256 / 3);
	sound.Play(SoundManager::SOUND::GAME);

	//�J�E���g�_�E����SE
	sound.Add(SoundManager::TYPE::SE, SoundManager::SOUND::COUNT_DOWN,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::COUNT_DOWN).handleId_);
	sound.AdjustVolume(SoundManager::SOUND::COUNT_DOWN, 256 / 2);

	//�X�^�[�g��SE
	sound.Add(SoundManager::TYPE::SE, SoundManager::SOUND::GAME_START,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::GAME_START).handleId_);
	sound.AdjustVolume(SoundManager::SOUND::GAME_START, 256 / 3);

	//�^�C�}�[�c��R�O�b��SE
	sound.Add(SoundManager::TYPE::SE, SoundManager::SOUND::TIMER,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::TIMER).handleId_);
	sound.AdjustVolume(SoundManager::SOUND::TIMER, 256 / 3);

	//�^�C�}�[�c��1�O�b��SE
	sound.Add(SoundManager::TYPE::BGM, SoundManager::SOUND::TIMER_FAST,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::TIMER_FAST).handleId_);
	sound.AdjustVolume(SoundManager::SOUND::TIMER_FAST, 256 / 3);

	//�^�C���A�b�v��SE
	sound.Add(SoundManager::TYPE::SE, SoundManager::SOUND::GAME_FINISH,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::GAME_FINISH).handleId_);
	sound.AdjustVolume(SoundManager::SOUND::GAME_FINISH, 256 / 3);

	//�X�R�A�p�摜
	numbersImgs_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::SCORE_NUMBER).handleIds_;

	//�J�E���g�_�E���p�摜
	countImgs_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::COUNTDOWN_NUMBER).handleIds_;

	//�^�C���A�b�v�p�摜
	timeUpImg_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::TIME_UP).handleId_;

	//�v���C���[
	player_ = std::make_unique<Player>();
	player_->Init();

	//�X�e�[�W
	stage_ = std::make_unique<StageManager>(*player_);
	stage_->Init();

	//�X�e�[�W�̃R���C�_�[��ǉ�
	player_->AddCollider(stage_->GetCounterTran().collider);
	int tableNum = stage_->GetTableNum();
	for(int i = 0; i < tableNum; ++i)
	{
		player_->AddCollider(stage_->GetTableTran(i).collider);
	}
	player_->AddCollider(stage_->GetShowCase().collider);
	player_->AddCollider(stage_->GetDustBox().collider);
	player_->AddCollider(stage_->GetFloorTran().collider);

	//�q�ƒ���
	customer_ = std::make_unique<OrderCustomerManager>();
	customer_->Init();

	//�^�C�}�[(2:00�j�Q��
	timer_ = std::make_unique<Timer>(MAX_MINUTE_TIME, MAX_SECOND_TIME);

	//�ŏ��̂��q�̒������e���󂯎���Đݒ�
	stage_->SetCurrentOrder(customer_->GetOrderData());

	//�J����
	mainCamera->SetFollow(&player_->GetTransform());
	mainCamera->ChangeMode(Camera::MODE::TOP_FIXED);
}

void GameScene::Update(void)
{
	(this->*update_)();
}

void GameScene::Draw(void)
{
	(this->*draw_)();
}

void GameScene::UpdateCountDown(void)
{
	SoundManager& sound = SoundManager::GetInstance();
	if (sclTime_ == 0.0f &&
		cntDownIdx_ < MAX_COUNT_DOWN - 1)sound.Play(SoundManager::SOUND::COUNT_DOWN);
	else if (sclTime_ == 0.0f &&
		cntDownIdx_ >= MAX_COUNT_DOWN - 1)sound.Play(SoundManager::SOUND::GAME_START);
	//�g�傳���鎞�Ԃ��X�V
	sclTime_ += SceneManager::GetInstance().GetDeltaTime();
	//�J�E���g�_�E���摜�̊g�嗦���C�[�W���O�Ŋ��炩�Ɋg��
	scale_ = Easing::QuintOut(
		sclTime_, CNTDOWN_SCL_MAX_TIME, 0.0f, CNTDOWN_IMG_MAX_SCL);
	//�g�嗦�𐧌��i0.0f�`2.0f�܂�)
	scale_ = std::clamp(abs(scale_), 0.0f, CNTDOWN_IMG_MAX_SCL);
	//�g�傳���鎞�Ԃ��g��ő厞�Ԃ𒴂�����
	//�J�E���g�_�E���̃C���f�b�N�X��i�߂�(�J�E���g�_�E���摜��ύX����)
	if (sclTime_ >= CNTDOWN_SCL_MAX_TIME)
	{
		cntDownIdx_++;
		sclTime_ = 0.0f;
		scale_ = 0.0f;
		//start�摜�܂ł�������Q�[���J�n
		if (cntDownIdx_ >= MAX_COUNT_DOWN)
		{
			update_ = &GameScene::UpdateGame;
			draw_ = &GameScene::DrawGame;
		}
	}
}

void GameScene::UpdateGame(void)
{
	InputManager& ins = InputManager::GetInstance();

	customer_->CheckServeAndOrder(stage_->GetServeItems());

	if (customer_->GetIsMoving())
	{
		//�ŏ��̂��q�̒������󂯎��
		stage_->SetCurrentOrder(customer_->GetOrderData());
	}

	if (ins.IsInputTriggered("pause"))
	{
		//�|�[�Y�{�^���������ꂽ��|�[�Y�V�[���֑J��
		SceneManager::GetInstance().PushScene(std::make_unique<PauseScene>());
	}

	//���������̏��i���񋟂��ꂽ��
	if (stage_->IsServed())
	{
		//�X�R�A�̉��Z
		score_ += customer_->GetOrderScore(stage_->GetServeItems());
		customer_->IsServe();		//�������o��
		stage_->ResetServeData();	//�T�[�u�����A�C�e�������Z�b�g
	}

	//�c��R�O�b�ɂȂ�����`�N�^�N��
	if (!remainderSE_ &&
		timer_->GetMinute() <= 0 &&
		timer_->GetSecond() == SECOND_SOUND_TIME)
	{
		SoundManager::GetInstance().Play(SoundManager::SOUND::TIMER);
		remainderSE_ = true;
	}
	//�c��P�O�b�ɂȂ����瑬���`�N�^�N��
	if (remainderSE_ &&
		timer_->GetMinute() <= 0 &&
		timer_->GetSecond() == SECOND_SOUND_TIME_FAST)
	{
		SoundManager::GetInstance().Play(SoundManager::SOUND::TIMER_FAST);
		remainderSE_ = false;
	}

	//�^�C�}�[���O�ɂȂ�����҂ҁ[���Ė炷
	if (timer_->IsEnd())
	{
		SoundManager::GetInstance().Stop(SoundManager::SOUND::TIMER_FAST);
		SoundManager::GetInstance().Play(SoundManager::SOUND::GAME_FINISH);
		//�^�C���A�b�v�֐��ֈڍs
		update_ = &GameScene::UpdateFinish;
		draw_ = &GameScene::DrawFinish;
	}

	stage_->Update();

	player_->Update();

	customer_->Update();

	timer_->Update();
}

void GameScene::UpdateFinish(void)
{
	ScoreManager& scr = ScoreManager::GetInstance();

	//�J�E���g�_�E���̎��Ɠ����悤�ɃC�[�W���O�ŉ摜���g�傳����
	//�g�傳���鎞�Ԃ��X�V
	sclTime_ += SceneManager::GetInstance().GetDeltaTime();
	//�摜�̊g�嗦���C�[�W���O�Ŋ��炩�Ɋg��
	scale_ = Easing::QuintOut(
		sclTime_, CNTDOWN_SCL_MAX_TIME, 0.0f, CNTDOWN_IMG_MAX_SCL);
	//�g�嗦�𐧌��i0.0f�`2.0f�܂�)
	scale_ = std::clamp(abs(scale_), 0.0f, CNTDOWN_IMG_MAX_SCL);
	//�g�傳���鎞�Ԃ��g��ő厞�Ԃ𒴂�����
	//�X�R�A��ۑ����ă��U���g��ʂ֑J��
	if (sclTime_ >= CNTDOWN_SCL_MAX_TIME)
	{
		if (cntDownIdx_ >= MAX_COUNT_DOWN)
		{
			scale_ = CNTDOWN_IMG_MAX_SCL;
			scr.SetCurrentScore(score_);
			scr.SaveScore(score_);
			SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::RESULT);
			return;
		}
		cntDownIdx_++;
		sclTime_ = 0.0f;
		scale_ = 0.0f;
	}
}

void GameScene::DrawCountDown(void)
{
	//�X�e�[�W�`��
	stage_->Draw();
	//�v���C���[�`��
	player_->Draw();

	DrawRotaGraph(
		Application::SCREEN_SIZE_X / 2,
		Application::SCREEN_SIZE_Y / 2 - 50,
		scale_,
		0.0,
		countImgs_[cntDownIdx_],
		true
	);
}

void GameScene::DrawGame(void)
{
	//���q�ƒ����`��
	customer_->Draw();
	//�X�e�[�W�`��
	stage_->Draw();

	//�v���C���[�`��
	player_->Draw();
	//UI�`��
	UIManager::GetInstance().Draw();
	//�^�C�}�[�`��
	timer_->Draw();
	//�X�R�A�`��
	DrawScore(score_);
}

void GameScene::DrawFinish(void)
{
	DrawGame();
	//�^�C���A�b�v�摜���g�債�ĕ`��
	DrawRotaGraph(
		Application::SCREEN_SIZE_X / 2,
		Application::SCREEN_SIZE_Y / 2 - 50,
		scale_,
		0.0,
		timeUpImg_,
		true
	);
}

void GameScene::DrawScore(int score)
{
	std::string str = std::to_string(score);
	const int digitWidth = 70;
	for (int i = 0; i < str.size(); ++i)
	{
		//������̊e���𐔎��ɕϊ����ĕ`��
		char ch = str[i];
		if ('0' <= ch && ch <= '9')
		{
			int digit = ch - '0';
			DrawRotaGraph(
				50 + i * digitWidth, 50,
				0.8f,0.0f,
				numbersImgs_[digit], true);
		}
	}
}
