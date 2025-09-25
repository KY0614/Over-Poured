#include <DxLib.h>
#include <algorithm>
#include <cmath>
#include "../Application.h"
#include "../Manager/GameSystem/SoundManager.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/InputManager.h"
#include "../Manager/Generic/ResourceManager.h"
#include "../Common/Easing.h"
#include "Score.h"

namespace
{
	//�X�R�A���Z�X�s�[�h
	const int ADD_SCORE_SPEED = 8;		//��{���Z�X�s�[�h
	const int ADD_CURRENT_SPEED = 5;	//���݃X�R�A�̉��Z�X�s�[�h
	const int ADD_TOTALSCORE_SPEED = 123;//���X�R�A���Z�X�s�[�h

	//�C�[�W���O�֘A
	const float START_SLIDE_X = -500.0f;	//X�J�n�ʒu
	const float END_SLIDE_X = (float)(Application::SCREEN_SIZE_X / 6);	//X�I���ʒu
	const float START_SLIDE_Y = 690.0f;	//Y�J�n�ʒu
	const float END_SLIDE_Y = 420.0f;	//Y�I���ʒu
	const float NEXT_SLIDE_START_X = -125.0f;	//���̃C�[�W���O���J�n����ڕW�ʒu
	const float SLIDE_MAX_TIME = 1.0f;	//�ڕW����
	const float BLINK_SPEED = 0.5f;		//�n�C���C�g�_�ł̊Ԋu

	//�Q�[�W�֘A
	const float MAX_GAUGE_TIME = 1.0f;		//�Q�[�W�̃C�[�W���O�ڕW����
	const float FIRST_GAUGE_SPEED = 0.5f;	//�ŏ��ɕ\������Q�[�W�̃X�s�[�h
	const float CURRENT_GAUGE_SPEED = 0.5f;	//�Ō�ɕ\������Q�[�W�̃X�s�[�h
	const int GAUGE_POS_X = Application::SCREEN_SIZE_X / 2 + 450;
	const int GAUGE_POS_Y = Application::SCREEN_SIZE_Y / 2 - 150;

	//�����N�֘A
	const int RANK_C_MAX = 500;		//C�����N�̍ő�l
	const int RANK_B_MAX = 1000;	//B�����N�̍ő�l
	const int RANK_A_MAX = 1500;	//A�����N�̍ő�l
	const int RANK_S_MAX = 1999;	//S�����N�̍ő�l

	const int RANK_C_MIN = 0;		//C�����N�̍ŏ��l
	const int RANK_B_MIN = 501;		//B�����N�̍ŏ��l
	const int RANK_A_MIN = 1001;	//A�����N�̍ŏ��l
	const int RANK_S_MIN = 1501;	//S�����N�̍ŏ��l

	const float RANK_SCORE_MARGIN_X = 150.0f;	//�����L���O�X�R�A�����x���̑傫�������炷�p
	const float RANK_SCORE_MARGIN_Y = 120.0f;	//�����L���O���̏c�Ԋu�i�`�悷��ۂɂ��炷���߁j
	const int RANK_SCORE_POS_Y = 100;			//�����L���OY���W

	const float RANK_GAUGE_SCALE = 3.0f;		//�����N�Q�[�W�̊g�嗦
	const float RANK_PERCENT_MAX = 100.0f;		//�Q�[�W�̃p�[�Z���g�\���̍ő�l
	const float RANK_LOGO_SCALE = 0.5f;			//�����N���S�̊g�嗦
	const float RANK_LAVEL_SCALE = 0.8f;		//�����L���O���x���̊g�嗦

	const int RANK_LAVEL_POS_X = 306;		//�����L���O���x����X���W
	const int RANK_LAVEL_HEIGHT = 128;		//�����L���O���x���̍���

	//�L�[�����𑣂��摜�̍���
	const int PUSHLOGO_HEIGHT = 1024;
	const float PUSHLOGO_SCALE = 2.0f;

	//���̍ő�{�����[��
	const int VOLUME_MAX = 256;

	//�`��̖��邳�̍ő�l
	const int BRIGHT_MAX = 255;
	//�n�C���C�g���̖��邳
	const int BRIGHT_HIGHLIGHT = 100;

	//���݂̃X�R�A��`�悷��X���W
	const int CURRENT_SCORE_POS_X = 612;	

	//�o�i�[�̊g�嗦(�s���N�̂��)
	const float BANNER_SCALE = 2.0f;	
	//�o�i�[�̍��W(�����)
	const int BLUE_BANNER_POS_X = Application::SCREEN_SIZE_X / 4;	//X���W
	const int BLUE_BANNER_POS_Y = Application::SCREEN_SIZE_Y / 4 - 150;	//Y���W
	//�o�i�[�̃T�C�Y(�����)
	const int BLUE_BANNER_WIDTH = 322;	//��
	const int BLUE_BANNER_HEIGHT = 555;	//����
	const float BLUE_BANNER_SCALE = 2.2f;	//�g�嗦
}

Score::Score(void)  
{  
    isCurrentScrDraw_ = false;  
    isRankingScrDraw_ = false;  
    isGaugeDraw_ = false;  
    currentScr_ = 0;  
    highLightIdx_ = -1;  
    currentRankIdx_ = -1;  

    for (int i = 0; i < ScoreManager::RANKING_NUM; ++i)  
    {  
        slideX_[i] = 0.0f;  
        slideXTime_[i] = 0.0f;  
        isRankScrMove_[i] = false;  
    }  

    for (int i = 0; i < RANK_NUM; ++i)  
    {  
        rankData_[i] = {};  
    }  

    blinkTime_ = 0.0f;  
    gaugeTime_ = 0.0f;  

    circleShadowImg_ = -1;  
    currentScrImg_ = -1;
    rankingBackImg_ = -1; 
	pushImg_ = -1;
    ranksImgs_ = nullptr; 

    state_ = STATE::PLAY_SCORE;  
    rank_ = RANK::MAX;  
    numberImgs_ = nullptr;  
	rankLabelImgs_ = nullptr;
    decoImg_ = 0;  
    aspectRatio_ = 0.0f;  

    stateChange_.emplace(STATE::PLAY_SCORE, std::bind(&Score::ChangePlayScore, this));  
}

Score::~Score(void)
{
}

void Score::Init(void)
{
	//�X�R�A�ǂݍ���
	auto& scr = ScoreManager::GetInstance();
	scr.LoadScore();

	//�����N���Ƃ̏�񏉊���
	InitRankInfo();

	//�摜�ǂݍ���
	LoadImages();

	//����̃X�R�A�������L���O�ɏƂ炵���킹�ă����N�C���ʒu��������
	for (int i = 0; i < ScoreManager::RANKING_NUM; ++i)
	{
		slideX_[i] = START_SLIDE_X;

		if (scr.GetRankingScore(i) == scr.GetCurrentScore())
		{
			highLightIdx_ = i;
		}
	}

	//�P�ʂ̃����N����C�[�W���O�J�n�i�ړ����t���O�𗧂Ă�j
	isRankScrMove_[0] = true;
	//���݂̃X�R�A�\���t���O�����Z�b�g
	currentRankIdx_ = 0;
	//����̃X�R�A���烉���N���擾
	rank_ = GetRankFromScore(scr.GetCurrentScore());

	//��ʂ̑傫���ɍ��킹�Ċg�嗦��ς���
	aspectRatio_ = static_cast<float>(Application::SCREEN_SIZE_Y) /
		static_cast<float>(Application::SCREEN_MAX_SIZE_Y);

	ChangeState(STATE::PLAY_SCORE);
}

void Score::Update(void)
{
	stateUpdate_();
}

void Score::Draw(void)
{
	stateDraw_();
}

void Score::ChangeState(STATE state)
{
	state_ = state;

	stateChange_[state_]();
}

void Score::ChangePlayScore(void)
{
	stateUpdate_ = std::bind(&Score::UpdatePlayScore, this);
	stateDraw_ = std::bind(&Score::DrawPlayScore, this);
}

void Score::UpdatePlayScore(void)
{
	auto& sound = SoundManager::GetInstance();
	//�X�e�[�g�J��
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsInputTriggered("Interact"))
	{
		//ChangeState(STATE::TOTAL_SCORE);
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
	}

	auto& scr = ScoreManager::GetInstance();
	//����̃X�R�A�܂łO������Z����
	if (currentScr_ >= scr.GetCurrentScore())
	{
		currentScr_ = scr.GetCurrentScore();
		isCurrentScrDraw_ = true;
	}
	else
	{
		currentScr_ += ADD_SCORE_SPEED;
	}

	CalcPercentFromRank();

	//�C�[�W���O�������Ĉړ�������
	for (int i = 0; i < ScoreManager::RANKING_NUM; ++i)
	{
		if (!isRankScrMove_[i])break;
		//�ړ���
		slideXTime_[i] += SceneManager::GetInstance().GetDeltaTime() * 0.5f;
		slideX_[i] = Easing::CubicOut(
			slideXTime_[i], SLIDE_MAX_TIME, START_SLIDE_X, END_SLIDE_X);

		//�ړ����W���I�����W�𒴂������ɂȂ�����ړ��I��
		if (slideX_[i] > END_SLIDE_X)
		{
			slideX_[i] = END_SLIDE_X;
			slideXTime_[i] = SLIDE_MAX_TIME;
			if (i >= RANK_NUM - 1)isRankingScrDraw_ = true;
		}
	}
	//�f�B���C��t���邽�߁A
	//�I�����W����O���炢�̂Ƃ���Ŏ����ړ�������
	for (int i = 0; i < (ScoreManager::RANKING_NUM - 1); ++i)
	{
		if (!isRankScrMove_[i + 1] && slideX_[i] > NEXT_SLIDE_START_X)
		{
			isRankScrMove_[i + 1] = true;
			break;
		}
	}

	//����̃X�R�A�������N�C�����Ă�����_�ł�����
	if (highLightIdx_ != -1)
	{
		blinkTime_ += SceneManager::GetInstance().GetDeltaTime() * BLINK_SPEED;
	}

	if (isRankingScrDraw_ && isCurrentScrDraw_ && isGaugeDraw_ )
	{
		//�����N�ɉ���������̉���炷
		if (rank_ == RANK::C)sound.Play(SoundManager::SOUND::NORMAL);
		if (rank_ == RANK::B)sound.Play(SoundManager::SOUND::GOOD);
		if (rank_ >= RANK::A)sound.Play(SoundManager::SOUND::GREATE);
	}
}

void Score::DrawPlayScore(void)
{
	auto& scr = ScoreManager::GetInstance();

	//�����L���O�̔w�i�i����j
	DrawRotaGraph3(
		BLUE_BANNER_POS_X,
		BLUE_BANNER_POS_Y,
		BLUE_BANNER_WIDTH / 2,
		BLUE_BANNER_HEIGHT / 2,
		aspectRatio_ * RANK_GAUGE_SCALE, aspectRatio_ * BLUE_BANNER_SCALE,
		0.0f, rankingBackImg_,
		true);

	//�o�i�[(����)�̃T�C�Y
	const int BANNER_SIZE = 500;
	const float scaleX = static_cast<float>(Application::SCREEN_SIZE_X) /
		static_cast<float>(Application::SCREEN_MAX_SIZE_X);
	//�����i�����N�̔w�i�̃s���N�̂ق�)
	DrawRotaGraph3(Application::SCREEN_SIZE_X - (BANNER_SIZE * scaleX),
		BANNER_SIZE * aspectRatio_,
		BANNER_SIZE / 2, BANNER_SIZE / 2,
		scaleX * BANNER_SCALE, aspectRatio_ * BANNER_SCALE,
		0.0f,
		decoImg_,
		true, false);

	//���݂̃X�R�A
	DrawVariableScore(currentScr_, CURRENT_SCORE_POS_X,
		Application::SCREEN_SIZE_Y - RANK_LAVEL_HEIGHT, aspectRatio_);

	//�u���݂̃X�R�A�v���x��
	DrawRotaGraph(RANK_LAVEL_POS_X,
				Application::SCREEN_SIZE_Y - RANK_LAVEL_HEIGHT,
				aspectRatio_, 0.0f,
				currentScrImg_, true);

	//�Q�[�W�̔w�i
	DrawRotaGraph(GAUGE_POS_X,
		GAUGE_POS_Y,
		aspectRatio_ * RANK_GAUGE_SCALE, 0.0f, circleShadowImg_,
		true, false);

	//�Q�[�W�{��
	for (int i = 0; i < static_cast<int>(rank_) + 1; ++i)
	{
		//DrawCircleGauge���g�����`��
		DrawCircleGauge(
			GAUGE_POS_X,
			GAUGE_POS_Y,
			rankData_[i].displayedRate_ * RANK_PERCENT_MAX,
			rankData_[i].gaugeImg_,
			0.0f,
			RANK_GAUGE_SCALE,
			false,false
		);
	}

	if (rankData_[(int)rank_].isFull_)
	{
		switch (rank_)
		{
		case Score::RANK::C:
			DrawRotaGraph(GAUGE_POS_X,
				GAUGE_POS_Y,
				aspectRatio_ * RANK_LOGO_SCALE, 0.0f, ranksImgs_[0],
				true, false);
			break;

		case Score::RANK::B:
				DrawRotaGraph(GAUGE_POS_X,
					GAUGE_POS_Y,
					aspectRatio_ * RANK_LOGO_SCALE, 0.0f, ranksImgs_[1],
					true, false);
			break;

		case Score::RANK::A:
			DrawRotaGraph(GAUGE_POS_X,
				GAUGE_POS_Y,
				aspectRatio_ * RANK_LOGO_SCALE, 0.0f, ranksImgs_[2],
				true, false);
			break;

		case Score::RANK::S:
			DrawRotaGraph(GAUGE_POS_X,
				GAUGE_POS_Y,
				aspectRatio_ * RANK_LOGO_SCALE, 0.0f, ranksImgs_[3],
				true, false);
			break;

		default:
			break;
		}

		int logoScl = (int)((float)(PUSHLOGO_HEIGHT / 2) * aspectRatio_);
		//pushspace�̉摜
		DrawRotaGraph(
			Application::SCREEN_SIZE_X / 2,
			Application::SCREEN_SIZE_Y / 2 + logoScl,
			PUSHLOGO_SCALE, 0.0, pushImg_, true);
	}
	const float stepBlink = 1.0f;	//�_�ł̃X�e�b�v
	for (int i = 0; i < ScoreManager::RANKING_NUM; ++i)
	{
		//�_�ŕ\���p�t���O�ƐF
		bool isBlink = (i == highLightIdx_) && (fmod(blinkTime_, stepBlink) < BLINK_SPEED);
		//�_�Œ��Ȃ�F��ς���
		int col = isBlink ? BRIGHT_HIGHLIGHT : BRIGHT_MAX;
		//�����L���O�X�R�A
		DrawRankingScore(scr.GetRankingScore(i),
			(slideX_[i] + (RANK_SCORE_MARGIN_X * aspectRatio_)),
			RANK_SCORE_POS_Y + (RANK_SCORE_MARGIN_Y * i * aspectRatio_), col);
		//�����L���O���x��(�P�ʁF�Ƃ��̂��)
		DrawRotaGraph(slideX_[i], RANK_SCORE_POS_Y + (RANK_SCORE_MARGIN_Y * i * aspectRatio_),
			aspectRatio_ * RANK_LAVEL_SCALE, 0.0f, rankLabelImgs_[i],
			true, false);
	}
}

void Score::CalcPercentFromRank(void)
{
	auto& scr = ScoreManager::GetInstance();
	int score = scr.GetCurrentScore();
	//���݂̃����N���C���f�b�N�X�Ƃ��ĕێ�
	int rankIdx = static_cast<int>(rank_);

	//���݂̃����N���Ⴂ�����N�ƌ��݂̃����N�̃Q�[�W���v�Z
	for (int i = 0; i <= currentRankIdx_; ++i)
	{
		RankInfo& data = rankData_[i];
		int prevIdx = 0;
		if (i > 0)prevIdx = i - 1;
		//���݂̃����N���Ⴂ�����N�̏���
		if (i < rankIdx)
		{
			if (data.isFull_)continue;
			//�B���ς݂Ȃ̂ŃQ�[�W�͖��^��
			data.currentRate_ = 1.0f;
			
			//�O�̃����N�Q�[�W�����ɖ��^���ō�������^���̏ꍇ�A
			//�Q�[�W�̎n�܂�̂ق��̑��x�𑁂����Ă�������
			if (rankData_[prevIdx].isFull_)
			{
				//�Q�[�W���C�[�W���O�������Ċ��炩�ɑ��₵�Ă���
				gaugeTime_ += SceneManager::GetInstance().GetDeltaTime();
				data.displayedRate_ = Easing::Linear(
					gaugeTime_, MAX_GAUGE_TIME,
					0.0f, data.currentRate_);
			}
			else 
			{
				//�Q�[�W���C�[�W���O�������Ċ��炩�ɑ��₵�Ă���
				gaugeTime_ += SceneManager::GetInstance().GetDeltaTime() * FIRST_GAUGE_SPEED;
				//�Q�[�W���C�[�W���O�������Ċ��炩�ɑ��₵�Ă���
				data.displayedRate_ = Easing::CubicIn(
					gaugeTime_, MAX_GAUGE_TIME,
					0.0f, data.currentRate_);
			}
			// �C�[�W���O�I������
			if (rankData_[i].displayedRate_ > rankData_[i].currentRate_)
			{
				data.displayedRate_ = data.currentRate_;
				currentRankIdx_++;		//���̃����N��
				gaugeTime_ = 0.0f;      //���Z�b�g
				data.isFull_ = true;
			}
		}//���݂̃����N�̏���
		else if (i == currentRankIdx_)
		{
			if (data.isFull_)break;
			//�X�R�A�����݂̃����N�͈̔͂���v�Z����i���100�������ăp�[�Z���g�ɂ���j
			rankData_[i].currentRate_ = (
				static_cast<float>(score - rankData_[rankIdx].startVal_) /
			static_cast<float>(rankData_[rankIdx].endVal_ - rankData_[rankIdx].startVal_));

			//�Q�[�W���C�[�W���O�������Ċ��炩�ɑ��₵�Ă���
			gaugeTime_ += SceneManager::GetInstance().GetDeltaTime() * CURRENT_GAUGE_SPEED;
			data.displayedRate_ = Easing::CubicOut(
				gaugeTime_, MAX_GAUGE_TIME,
				0.0f, data.currentRate_);

			//�C�[�W���O�I������
			if (rankData_[i].displayedRate_ >= rankData_[i].currentRate_)
			{
				data.displayedRate_ = data.currentRate_;
				gaugeTime_ = 0.0f;      //���Z�b�g
				data.isFull_ = true;
				isGaugeDraw_ = true;
			}
		}
	}
}

Score::RANK Score::GetRankFromScore(int score)
{
	//�X�R�A���烉���N���擾
	if (score <= RANK_C_MAX) return RANK::C;
	else if (score < RANK_A_MIN) return RANK::B;
	else if (score < RANK_S_MIN) return RANK::A;
	else return RANK::S;
}

void Score::InitRankInfo(void)
{
	//�����N���Ƃɏ�����
	rankData_[0].gaugeImg_ = ResourceManager::GetInstance().
		Load(ResourceManager::SRC::RANK_C).handleId_;
	rankData_[0].startVal_ = 0;
	rankData_[0].endVal_ = RANK_C_MAX;
	rankData_[0].currentRate_ = 0.0f;
	rankData_[0].displayedRate_ = 0.0f;

	rankData_[1].gaugeImg_ = ResourceManager::GetInstance().
		Load(ResourceManager::SRC::RANK_B).handleId_;
	rankData_[1].startVal_ = RANK_C_MAX + 1;	//C�����N�̎��̒l����J�n
	rankData_[1].endVal_ = RANK_B_MAX;
	rankData_[1].currentRate_ = 0.0f;
	rankData_[1].displayedRate_ = 0.0f;

	rankData_[2].gaugeImg_ = ResourceManager::GetInstance().
		Load(ResourceManager::SRC::RANK_A).handleId_;
	rankData_[2].startVal_ = RANK_B_MAX + 1;	//B�����N�̎��̒l����J�n
	rankData_[2].endVal_ = RANK_A_MAX;
	rankData_[2].currentRate_ = 0.0f;
	rankData_[2].displayedRate_ = 0.0f;

	rankData_[3].gaugeImg_ = ResourceManager::GetInstance().
		Load(ResourceManager::SRC::RANK_S).handleId_;
	rankData_[3].startVal_ = RANK_A_MAX + 1;	//A�����N�̎��̒l����J�n
	rankData_[3].endVal_ = RANK_S_MAX;
	rankData_[3].currentRate_ = 0.0f;
	rankData_[3].displayedRate_ = 0.0f;
}

void Score::DrawVariableScore(int score, int posX, int posY,float scale)
{
	//�X�R�A�𕶎���ɕϊ�
	std::string str = std::to_string(score);
	//1����������̕�
	const int digitWidth = RANK_LAVEL_HEIGHT * scale;

	for (int i = 0; i < str.size(); ++i)
	{
		char ch = str[i];
		//0�`9�̕����Ȃ�
		if ('0' <= ch && ch <= '9')
		{
			//�`��
			int digit = ch - '0';
			DrawRotaGraph(
				posX + static_cast<int>(i * digitWidth * scale), posY,
				scale, 0.0f,
				numberImgs_[digit], true);
		}
	}
}

void Score::DrawRankingScore(int score, int posX, int posY, int hightLight)
{
	//�X�R�A�𕶎���ɕϊ�
	std::string str = std::to_string(score);
	//1����������̕�
	const int digitWidth = 80;
	//�g�嗦
	const float scale = RANK_LAVEL_SCALE;
	//��ʔ䗦���l�����Ċg�嗦������
	const float drawScale = aspectRatio_ * scale;
	for (int i = 0; i < str.size(); ++i)
	{
		char ch = str[i];
		//0�`9�̕����Ȃ�
		if ('0' <= ch && ch <= '9')
		{
			int digit = ch - '0';
			//�����N�C�����Ă���X�R�A�Ȃ�F��ς���
			SetDrawBright(BRIGHT_MAX, BRIGHT_MAX, hightLight);
			DrawRotaGraph(
				posX + static_cast<int>(i * digitWidth * aspectRatio_), posY,
				drawScale, 0.0f,
				numberImgs_[digit], true);
			SetDrawBright(BRIGHT_MAX, BRIGHT_MAX, BRIGHT_MAX);
		}
	}
}

void Score::LoadImages(void)
{
	//�~�Q�[�W�̔w�i�摜�ǂݍ���
	circleShadowImg_ = ResourceManager::GetInstance().
		Load(ResourceManager::SRC::UI_CIRCLESHADOW).handleId_;

	//�X�R�A�p�����摜�ǂݍ���
	numberImgs_ = ResourceManager::GetInstance().
		Load(ResourceManager::SRC::SCORE_NUMBER).handleIds_;

	//�����L���O���x���摜�ǂݍ���
	rankLabelImgs_ = ResourceManager::GetInstance().
		Load(ResourceManager::SRC::RANKING_LABEL).handleIds_;

	//���݃X�R�A���x���摜�ǂݍ���
	currentScrImg_ = ResourceManager::GetInstance().
		Load(ResourceManager::SRC::CURRENT_SCORE).handleId_;

	//�����L���O�̔w�i�摜�ǂݍ���
	rankingBackImg_ = ResourceManager::GetInstance().
		Load(ResourceManager::SRC::RANKING_BACK).handleId_;

	//�����摜�ǂݍ���
	decoImg_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::PINK_BANNER).handleId_;

	//�����N���Ƃ̕����摜
	ranksImgs_ = ResourceManager::GetInstance().
		Load(ResourceManager::SRC::RANKS).handleIds_;

	//�摜�ǂݍ���
	pushImg_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::PUSH_SPACE).handleId_;
}

void Score::InitSound(void)
{
	auto& sound = SoundManager::GetInstance();
	//�����NC�̎��̔���̉�
	sound.Add(SoundManager::TYPE::SE, SoundManager::SOUND::NORMAL,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::SCORE_NORMAL).handleId_);
	sound.AdjustVolume(SoundManager::SOUND::NORMAL, VOLUME_MAX / 2);
	//�����NB�̎��̔���̉�
	sound.Add(SoundManager::TYPE::SE, SoundManager::SOUND::GOOD,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::SCORE_GOOD).handleId_);
	sound.AdjustVolume(SoundManager::SOUND::GOOD, VOLUME_MAX / 2);
	//�����NA�ȏ�̎��̔���̉�
	sound.Add(SoundManager::TYPE::SE, SoundManager::SOUND::GREATE,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::SCORE_GREATE).handleId_);
	sound.AdjustVolume(SoundManager::SOUND::GREATE, VOLUME_MAX / 2);
}