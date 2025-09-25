#include "../Manager/Generic/ResourceManager.h"
#include "PopUpUI.h"

namespace
{
    //�A�j���[�V�������ԁi�b�j
    const float ANIM_DURATION = 1.0f;
	//�ړ��Ԋu(�ړ�����)
    const float MOVE_DISTANCE = 1.0f;
    //�ő哧�ߓx
	const float ALPHA_MAX = 255.0f; 
    //�t���[�����[�g
	const float FPS_RATE = 60.0f; 
}

PopUpUI::PopUpUI(int score, const VECTOR& startPos) : score_(score), animTime_(0.0f)
{
    pos_ = startPos;
    alpha_ = ALPHA_MAX; //�������ߓx���ő�ɐݒ�
    isActive_ = true;
    numbersImgs_ = nullptr;
}

void PopUpUI::Init(void)
{
	//�X�R�A�̐����摜�n���h�����擾
    numbersImgs_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::SCORE_NUMBER).handleIds_;
}

void PopUpUI::Update(void)
{
    if (!isActive_) return;

    animTime_ += 1.0f / FPS_RATE; //60FPS��z�肵���o�ߎ��ԍX�V
    if (animTime_ >= ANIM_DURATION)
    {
        isActive_ = false; //�A�j���[�V�����I��
        return;
    }

    // �C�[�W���O�֐����g�p���Ĉʒu�Ɠ��ߓx���v�Z
    float t = animTime_ / ANIM_DURATION;
    pos_.y = pos_.y - t * MOVE_DISTANCE; //��Ɉړ�
    alpha_ = ALPHA_MAX * (MOVE_DISTANCE - t); //�t�F�[�h�A�E�g

	//���ߓx��ݒ�
    SetAlpha(alpha_);
}

void PopUpUI::Draw(void)
{
	//�\������Ă��Ȃ���Ε`�悵�Ȃ�
    if (!isActive_) return;
	//�X�R�A��`��
    DrawScore(score_, pos_, alpha_);
}

void PopUpUI::DrawScore(int score, const VECTOR& pos, float alpha)
{
    std::string str = std::to_string(score);
	const int digitWidth = 35;  //�����摜�̕�(1�̐����̕��j
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(alpha));
	const float scale = 0.4f; //�����摜�̊g�嗦

    for (int i = 0; i < str.size(); ++i)
    {
        char ch = str[i];
		//'0'����'9'�͈͓̔��̕������m�F
        if ('0' <= ch && ch <= '9')
        {
            int digit = ch - '0';
            DrawRotaGraph(
                pos.x - (str.size() * digitWidth / 2) + i * digitWidth,
                pos.y,
                scale, 0.0f,
                numbersImgs_[digit], true
            );
        }
    }
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); //�`�惂�[�h�����ɖ߂�
}
