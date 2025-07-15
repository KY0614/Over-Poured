#include <DxLib.h>
#include <algorithm>
#include <cmath>
#include "../Application.h"
#include "../Common/DebugDrawFormat.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/InputManager.h"
#include "../Manager/Generic/ResourceManager.h"
#include "../Common/Easing.h"
#include "Score.h"

Score::Score(void)
{
	currentScr_ = 0;
	totalScr_ = 0;
	highLightIdx_ = -1;
	currentRankIdx_ = -1;

	for (int i = 0; i < ScoreManager::RANKING_NUM; ++i)
	{
		slideX_[i] = 0.0f;
		slideXTime_[i] = 0.0f;
		isMove_[i] = false;
	}

	for (int i = 0; i < RANK_NUM; ++i)
	{
		rankData_[i] = {};
	}

	blinkTime_ = 0.0f;
	gaugeTime_ = 0.0f;
	slideY_ = 0.0f;
	slideYTime_ = 0.0f;

	circleShadowImg_ = -1;
	state_ = STATE::PLAY_SCORE;
	rank_ = RANK::MAX;
	phase_ = TOTALSCR_PHASE::COUNT_UP;
	numberImgs_ = nullptr;
	rankingImgs_ = nullptr;

	stateChange_.emplace(STATE::PLAY_SCORE, std::bind(&Score::ChangePlayScore, this));
	stateChange_.emplace(STATE::TOTAL_SCORE, std::bind(&Score::ChangeTotalScore, this));
}

Score::~Score(void)
{
}

void Score::Init(void)
{
	auto& scr = ScoreManager::GetInstance();
	scr.LoadScore();

	//�����N���Ƃ̏�񏉊���
	InitRankInfo();

	//�摜�ǂݍ���
	circleShadowImg_ = ResourceManager::GetInstance().
		Load(ResourceManager::SRC::UI_CIRCLESHADOW).handleId_;
	
	//�摜�ǂݍ���
	numberImgs_ = ResourceManager::GetInstance().
		Load(ResourceManager::SRC::SCORE_NUMBER).handleIds_;
		
	//�摜�ǂݍ���
	rankingImgs_ = ResourceManager::GetInstance().
		Load(ResourceManager::SRC::RANKING).handleIds_;
		
	//�摜�ǂݍ���
	currentScrImg_ = ResourceManager::GetInstance().
		Load(ResourceManager::SRC::CURRENT_SCORE).handleId_;

	//����̃X�R�A�������L���O�ɏƂ炵���킹�ă����N�C���ʒu�𒲂ׂ�
	for (int i = 0; i < ScoreManager::RANKING_NUM; ++i)
	{
		slideX_[i] = START_SLIDE_X;

		if (scr.GetRankingScore(i) == scr.GetCurrentScore())
		{
			highLightIdx_ = i;
		}
	}
	slideY_ = START_SLIDE_Y;
	isMove_[0] = true;
	currentRankIdx_ = 0;
	rank_ = GetRankFromScore(scr.GetCurrentScore());

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

void Score::ChangeTotalScore(void)
{
	stateUpdate_ = std::bind(&Score::UpdateTotalScore, this);
	stateDraw_ = std::bind(&Score::DrawTotalScore, this);
}

void Score::UpdatePlayScore(void)
{
	//�X�e�[�g�J��
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsInputTriggered("Interact"))
	{
		ChangeState(STATE::TOTAL_SCORE);
	}

	auto& scr = ScoreManager::GetInstance();
	//����̃X�R�A�܂łO������Z����
	if (currentScr_ >= scr.GetCurrentScore())
	{
		currentScr_ = scr.GetCurrentScore();
	}
	else
	{
		currentScr_ += ADD_SCORE_SPEED;
	}

	CalcPercentFromRank();

	//�C�[�W���O�������Ĉړ�������
	for (int i = 0; i < ScoreManager::RANKING_NUM; ++i)
	{
		if (!isMove_[i])break;
		//�ړ���
		slideXTime_[i] += SceneManager::GetInstance().GetDeltaTime() * 0.5f;
		slideX_[i] = Easing::CubicOut(
			slideXTime_[i], SLIDE_MAX_TIME, START_SLIDE_X, END_SLIDE_X);

		//�ړ����W���I�����W�𒴂������ɂȂ�����ړ��I��
		if (slideX_[i] > END_SLIDE_X)
		{
			slideX_[i] = END_SLIDE_X;
			slideXTime_[i] = SLIDE_MAX_TIME;
		}
	}
	//�f�B���C��t���邽�߁A
	//�I�����W����O���炢�̂Ƃ���Ŏ����ړ�������
	for (int i = 0; i < (ScoreManager::RANKING_NUM - 1); ++i)
	{
		if (!isMove_[i + 1] && slideX_[i] > NEXT_SLIDE_START_X)
		{
			isMove_[i + 1] = true;
			break;
		}
	}

	//����̃X�R�A�������N�C�����Ă�����_�ł�����
	if (highLightIdx_ != -1)
	{
		blinkTime_ += SceneManager::GetInstance().GetDeltaTime() * BLINK_SPEED;
	}
}

void Score::UpdateTotalScore(void)
{
	auto& scr = ScoreManager::GetInstance();

	switch (phase_)
	{
	case TOTALSCR_PHASE::COUNT_UP:
		//���X�R�A�܂ŉ��Z����i����̃X�R�A�����������j
		if (totalScr_ >= scr.GetAggregateScore() - currentScr_)
		{
			totalScr_ = scr.GetAggregateScore() - currentScr_;
			phase_ = TOTALSCR_PHASE::SHOW_CURRENT;
		}
		else
		{
			totalScr_ += ADD_TOTALSCORE_SPEED;
		}
		break;

	case TOTALSCR_PHASE::SHOW_CURRENT:
		//�ړ���
		slideYTime_ += SceneManager::GetInstance().GetDeltaTime() * 0.5f;
		slideY_ = Easing::CubicOut(
			slideYTime_, SLIDE_MAX_TIME, START_SLIDE_Y, END_SLIDE_Y);

		//�ړ����W���I�����W�𒴂������ɂȂ�����ړ��I��
		if (slideY_ < END_SLIDE_Y)
		{
			slideY_ = END_SLIDE_Y;
			slideYTime_ = 0.0f;
			phase_ = TOTALSCR_PHASE::MOVE_TO_TOTAL;
		}

		break;

	case TOTALSCR_PHASE::MOVE_TO_TOTAL:
		//�ړ���
		slideYTime_ += SceneManager::GetInstance().GetDeltaTime() * 0.5f;
		slideY_ = Easing::BackIn(
			slideYTime_, SLIDE_MAX_TIME, END_SLIDE_Y, 320.0f,0.0f);

		//�ړ����W���I�����W�𒴂������ɂȂ�����ړ��I��
		if (slideY_ < 320.0f)
		{
			slideY_ = 320.0f;
			slideYTime_ = 0.0f;
			phase_ = TOTALSCR_PHASE::MERGE;
		}
		break;

	case TOTALSCR_PHASE::MERGE:
		slideY_ = -100.0f;
		//����̃X�R�A�����Z����
		if (totalScr_ >= scr.GetAggregateScore())
		{
			totalScr_ = scr.GetAggregateScore();
			phase_ = TOTALSCR_PHASE::FINISH;
		}
		else
		{
			totalScr_ += ADD_CURRENT_SPEED;
		}
		break;

	case TOTALSCR_PHASE::FINISH:
		// ���͎�t or ���̉��o�֑J��
		break;
	}

	//�V�[���J�� 
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsInputTriggered("Interact"))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
	}
}

void Score::DrawPlayScore(void)
{
	auto& scr = ScoreManager::GetInstance();

	//���݂̃X�R�A
	DrawVariableScore(currentScr_, Application::SCREEN_SIZE_X / 2,
		Application::SCREEN_SIZE_Y / 2 + 256);

	//�u���݂̃X�R�A�v���x��
	DrawRotaGraph(Application::SCREEN_SIZE_X / 2,
				Application::SCREEN_SIZE_Y / 2 + 256,
				1.0f, 0.0f,
				currentScrImg_, true);

	//�Q�[�W�̔w�i
	DrawRotaGraph(GAUGE_POS_X,
		GAUGE_POS_Y,
		3.0f, 0.0f, circleShadowImg_,
		true, false);

	//�Q�[�W�{��
	for (int i = 0; i < static_cast<int>(rank_) + 1; ++i)
	{
		// DrawCircleGauge���g�����`��
		DrawCircleGauge(
			GAUGE_POS_X,
			GAUGE_POS_Y,
			rankData_[i].displayedRate_ * 100.0f,
			rankData_[i].gaugeImg_,
			0.0f,
			3.0f,
			false,false
		);
	}
	if (rankData_[(int)rank_].isFull_)
	{
		switch (rank_)
		{
		case Score::RANK::C:
			DrawString(Application::SCREEN_SIZE_X / 2 + 320,
				Application::SCREEN_SIZE_Y / 2 - 80, L"C",
				0xFFFFFF);
			break;

		case Score::RANK::B:
			DrawString(Application::SCREEN_SIZE_X / 2 + 320,
				Application::SCREEN_SIZE_Y / 2 - 80, L"B",
				0xFFFFFF);
			break;

		case Score::RANK::A:
			DrawString(Application::SCREEN_SIZE_X / 2 + 320,
				Application::SCREEN_SIZE_Y / 2 - 80, L"A",
				0xFFFFFF);
			break;

		case Score::RANK::S:
			DrawString(Application::SCREEN_SIZE_X / 2 + 320,
				Application::SCREEN_SIZE_Y / 2 - 80, L"S",
				0xFFFFFF);
			break;

		default:
			break;
		}
	}

	for (int i = 0; i < ScoreManager::RANKING_NUM; ++i)
	{
		//�_�ŕ\���p�t���O�ƐF
		bool isBlink = (i == highLightIdx_) && (fmod(blinkTime_, 1.0f) < 0.5f);
		int col = isBlink ? 0xFFFF00 : 0xFFFFFF;

		DrawRankingScore(scr.GetRankingScore(i),
			(slideX_[i] + RANK_SCORE_MARIGINE_X),
			RANK_SCORE_POS_Y + (RANK_SCORE_MARIGINE_Y * i));

		DrawRotaGraph(slideX_[i], RANK_SCORE_POS_Y + (RANK_SCORE_MARIGINE_Y * i),
			0.6f, 0.0f, rankingImgs_[i],
			true, false);
	}
}

void Score::DrawTotalScore(void)
{
#ifdef _DEBUG

	auto& scr = ScoreManager::GetInstance();

	int line = 1;	//�s
	int lineHeight = 40;	//�s

	SetFontSize(24);		
	DrawFormatString(Application::SCREEN_SIZE_X / 2 - 200,
			Application::SCREEN_SIZE_Y / 2,
			0xFFFFFF, L"�S�v���C���[�̑��X�R�A : �� % d", totalScr_);
	
	if (phase_ >= TOTALSCR_PHASE::SHOW_CURRENT && phase_ < TOTALSCR_PHASE::MERGE)
	{
		DrawFormatString(Application::SCREEN_SIZE_X / 2 - 200, slideY_, 0xFFFFFF,
			L"����̃X�R�A : ��%d", currentScr_);
	}

	SetFontSize(16);

#endif // _DEBUG

}

void Score::CalcPercentFromRank(void)
{
	auto& scr = ScoreManager::GetInstance();
	int score = scr.GetCurrentScore();
	//���݂̃����N���C���f�b�N�X�Ƃ��ĕێ�
	int rankIdx = static_cast<int>(rank_);

	//
	for (int i = 0; i <= currentRankIdx_; ++i)
	{
		auto& data = rankData_[i];
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
			}
		}
	}
}

Score::RANK Score::GetRankFromScore(int score)
{
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
	rankData_[1].startVal_ = RANK_C_MAX + 1;
	rankData_[1].endVal_ = RANK_B_MAX;
	rankData_[1].currentRate_ = 0.0f;
	rankData_[1].displayedRate_ = 0.0f;

	rankData_[2].gaugeImg_ = ResourceManager::GetInstance().
		Load(ResourceManager::SRC::RANK_A).handleId_;
	rankData_[2].startVal_ = RANK_B_MAX + 1;
	rankData_[2].endVal_ = RANK_A_MAX;
	rankData_[2].currentRate_ = 0.0f;
	rankData_[2].displayedRate_ = 0.0f;

	rankData_[3].gaugeImg_ = ResourceManager::GetInstance().
		Load(ResourceManager::SRC::RANK_S).handleId_;
	rankData_[3].startVal_ = RANK_A_MAX + 1;
	rankData_[3].endVal_ = RANK_S_MAX;
	rankData_[3].currentRate_ = 0.0f;
	rankData_[3].displayedRate_ = 0.0f;
}

void Score::DrawVariableScore(int score, int posX, int posY)
{
	std::string str = std::to_string(score);
	const int digitWidth = 70;
	const float scale = 1.0f;

	// �������̕`�敝���v�Z
	int totalWidth = static_cast<int>(str.size() * digitWidth * scale);
	// �E�񂹁F��ʉE�[���� totalWidth �����ւ��炷
	int marigineX = Application::SCREEN_SIZE_X - totalWidth;

	for (int i = 0; i < str.size(); ++i)
	{
		char ch = str[i];
		if ('0' <= ch && ch <= '9')
		{
			int digit = ch - '0';
			DrawRotaGraph(
				marigineX + static_cast<int>(i * digitWidth * scale), posY,
				0.8f, 0.0f,
				numberImgs_[digit], true);
		}
	}
}

void Score::DrawRankingScore(int score, int posX, int posY)
{
	std::string str = std::to_string(score);
	const int digitWidth = 80;
	const float scale = 0.5f;

	for (int i = 0; i < str.size(); ++i)
	{
		char ch = str[i];
		if ('0' <= ch && ch <= '9')
		{
			int digit = ch - '0';
			DrawRotaGraph(
				posX + static_cast<int>(i * digitWidth * scale), posY,
				scale, 0.0f,
				numberImgs_[digit], true);
		}
	}
}

void Score::DrawScore(int score, int posX, int posY)
{
	std::string str = std::to_string(score);
	const int digitWidth = 70;
	for (int i = 0; i < str.size(); ++i)
	{
		char ch = str[i];
		if ('0' <= ch && ch <= '9')
		{
			int digit = ch - '0';
			DrawRotaGraph(
				50 + i * digitWidth, 50,
				0.8f, 0.0f,
				numberImgs_[digit], true);
		}
	}
}