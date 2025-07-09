#include <algorithm>
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
	highLightIdx_ = -1;

	for (int i = 0; i < ScoreManager::RANKING_NUM; ++i)
	{
		slideX_[i] = 0.0f;
		slideTime_[i] = 0.0f;
		isMove_[i] = false;
	}

	for (int i = 0; i < RANK_NUM; ++i)
	{
		rankData_[i] = {};
	}

	blinkTime_ = 0.0f;

	circleShadowImg_ = -1;
	state_ = STATE::PLAY_SCORE;
	rank_ = RANK::MAX;

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

	InitRankInfo();

	circleShadowImg_ = ResourceManager::GetInstance().
		Load(ResourceManager::SRC::UI_CIRCLESHADOW).handleId_;

	//����̃X�R�A�������L���O�ɏƂ炵���킹�ă����N�C���ʒu�𒲂ׂ�
	for (int i = 0; i < ScoreManager::RANKING_NUM; ++i)
	{
		slideX_[i] = START_SLIDE_X;

		if (scr.GetRankingScore(i) == scr.GetCurrentScore())
		{
			highLightIdx_ = i;
		}
	}

	isMove_[0] = true;

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
	//for (int i = 0; i < (int)GetRankFromScore(scr.GetCurrentScore()) + 1; ++i)
	//{
	//	if (currentScr_ <= rankData_[i].startVal_)
	//	{
	//		rankData_[i].currentRate_ = 0.0f;
	//	}
	//	else if (currentScr_ >= rankData_[i].endVal_)
	//	{
	//		rankData_[i].currentRate_ = 1.0f;
	//	}
	//	else
	//	{
	//		rankData_[i].currentRate_ =
	//			(float)(currentScr_ - rankData_[i].startVal_) /
	//			(rankData_[i].endVal_ - rankData_[i].startVal_);
	//	}

	//	// �C�[�W���O�FeaseOutCubic
	//	float t = rankData_[i].currentRate_;
	//	float eased = 1.0f - powf(1.0f - t, 3.0f);
	//	rankData_[i].displayedRate_ += (eased - rankData_[i].displayedRate_) * 0.2f; // ���炩�ɒǏ]
	//}

	//�C�[�W���O�������Ĉړ�������
	for (int i = 0; i < ScoreManager::RANKING_NUM; ++i)
	{
		if (!isMove_[i])break;
		//�ړ���
		slideTime_[i] += SceneManager::GetInstance().GetDeltaTime() * 0.5f;
		slideX_[i] = Easing::CubicOut(
			slideTime_[i], SLIDE_MAX_TIME, START_SLIDE_X, END_SLIDE_X);

		//�ړ����W���I�����W�𒴂������ɂȂ�����ړ��I��
		if (slideX_[i] > END_SLIDE_X)
		{
			slideX_[i] = END_SLIDE_X;
			slideTime_[i] = SLIDE_MAX_TIME;
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
		blinkTime_ += SceneManager::GetInstance().GetDeltaTime() * 0.5f;
	}
}

void Score::UpdateTotalScore(void)
{
	//�V�[���J��
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsInputTriggered("Interact"))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
	}
}

void Score::DrawPlayScore(void)
{
#ifdef _DEBUG
	DrawString(0, 0, L"���U���g", 0xFFFFFF);
	auto& scr = ScoreManager::GetInstance();
	//�X�R�A��ǂݍ���

	DrawFormatString(0, 20, 0xff0000, L"�X�R�A", 0xFFFFFF);

	int line = 10;	//�s
	int lineHeight = 40;	//�s
	SetFontSize(24);
	DebugDrawFormat::FormatStringRight(L"����̃X�R�A : ��%d      ",
		currentScr_, line, lineHeight);

	DrawRotaGraph(Application::SCREEN_SIZE_X / 2,
		Application::SCREEN_SIZE_Y/2,
		1.0f, 0.0f, circleShadowImg_,
		true, false);

	for (int i = (int)rank_ + 1; i >= 0; --i)
	{
		// DrawCircleGauge �̎d�l�ɍ��킹�� 0.0?100.0 �ɕϊ�
		double percent = std::clamp(rankData_[i].displayedRate_, 0.0f, 1.0f) * 100.0;
		
		// DrawCircleGauge���g�����`��
		DrawCircleGauge(
			Application::SCREEN_SIZE_X / 2,
			Application::SCREEN_SIZE_Y / 2,
			rankData_[i].displayedRate_,
			rankData_[i].gaugeImg_,
			0.0f,
			1.0f,
			false,false
		);
	}

	DrawFormatString(END_SLIDE_X, 180, 0xFFFFFF,
		L"�X�R�A�����L���O");
	for (int i = 0; i < ScoreManager::RANKING_NUM; ++i)
	{
		//�_�ŕ\���p�t���O�ƐF
		bool isBlink = (i == highLightIdx_) && (fmod(blinkTime_, 1.0f) < 0.5f);
		int col = isBlink ? 0xFFFF00 : 0xFFFFFF;

		DrawFormatString(slideX_[i], 220 + (40 * i), col,
			L"%d �� : ��%d", i + 1, scr.GetRankingScore(i));
	}
	SetFontSize(16);
#endif // _DEBUG
}

void Score::DrawTotalScore(void)
{
#ifdef _DEBUG

	auto& scr = ScoreManager::GetInstance();

	int line = 1;	//�s
	int lineHeight = 40;	//�s

	SetFontSize(24);
	DebugDrawFormat::FormatString(L"�S�v���C���[�̑��X�R�A : ��%d",
		scr.GetAggregateScore(), line, lineHeight);
	SetFontSize(16);

#endif // _DEBUG

}

void Score::CalcPercentFromRank(void)
{
	auto& scr = ScoreManager::GetInstance();
	int score = scr.GetCurrentScore();
	RANK currentRank = GetRankFromScore(score);

	for (int i = 0; i < (int)currentRank + 1; ++i)
	{
		if (score <= rankData_[i].startVal_)
		{
			rankData_[i].currentRate_ = 0.0f;
		}
		else if (score >= rankData_[i].endVal_)
		{
			rankData_[i].currentRate_ = 1.0f;
		}
		else
		{
			rankData_[i].currentRate_ =
				((float)(score) /
					rankData_[i].endVal_ - rankData_[i].startVal_) * 100.0f;
		}

		// �C�[�W���O�FeaseOutCubic
		float t = rankData_[i].currentRate_;
		float eased = 1.0f - powf(1.0f - t, 3.0f);
		rankData_[i].displayedRate_ += (eased - rankData_[i].displayedRate_) * 0.2f; // ���炩�ɒǏ]
	}
}

Score::RANK Score::GetRankFromScore(int score)
{
	if (score <= RANK_C_MAX) return RANK::C;
	if (score > RANK_C_MAX) return RANK::B;
	if (score > RANK_B_MAX) return RANK::A;
	if (score > RANK_A_MAX) return RANK::S;
	if (score > RANK_S_MAX) return RANK::S;
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
	rankData_[0].pos_ = { 0,0 };

	rankData_[1].gaugeImg_ = ResourceManager::GetInstance().
		Load(ResourceManager::SRC::RANK_B).handleId_;
	rankData_[1].startVal_ = RANK_C_MAX + 1;
	rankData_[1].endVal_ = RANK_B_MAX;
	rankData_[1].currentRate_ = 0.0f;
	rankData_[1].displayedRate_ = 0.0f;
	rankData_[1].pos_ = { 0,0 };

	rankData_[2].gaugeImg_ = ResourceManager::GetInstance().
		Load(ResourceManager::SRC::RANK_A).handleId_;
	rankData_[2].startVal_ = RANK_B_MAX + 1;
	rankData_[2].endVal_ = RANK_A_MAX;
	rankData_[2].currentRate_ = 0.0f;
	rankData_[2].displayedRate_ = 0.0f;
	rankData_[2].pos_ = { 0,0 };

	rankData_[3].gaugeImg_ = ResourceManager::GetInstance().
		Load(ResourceManager::SRC::RANK_S).handleId_;
	rankData_[3].startVal_ = RANK_A_MAX + 1;
	rankData_[3].endVal_ = RANK_S_MAX;
	rankData_[3].currentRate_ = 0.0f;
	rankData_[3].displayedRate_ = 0.0f;
	rankData_[3].pos_ = { 0,0 };
}
