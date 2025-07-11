#include <DxLib.h>
#include "../Application.h"
#include "FpsControl.h"

FpsControl::FpsControl()
    : currentTime_(0), prevFrmTime_(0),
    frmCnt_(0), updatreFrmRateTime_(0),
    frmRate_(0.0f)
{   
}

void FpsControl::Init(void)
{
    currentTime_ = 0;
    prevFrmTime_ = 0;  

    frmCnt_ = 0;  
    updatreFrmRateTime_ = 0;  

    frmRate_ = 0.0f; 
}

bool FpsControl::UpdateFrameRate(void)
{
    Sleep(1);   //�V�X�e���ɏ�����Ԃ�

    currentTime_ = GetNowCount();

    if (currentTime_ - prevFrmTime_ >= FRAME_RATE)
    {
        prevFrmTime_ = currentTime_;

        frmCnt_++;

        return true;
    }

    return false;
}

void FpsControl::CalcFrameRate(void)
{
    //�O��̃t���[�����[�g�X�V����̌o�ߎ��Ԃ����߂�
    int nDifTime = currentTime_ - updatreFrmRateTime_;

    //�O��̃t���[�����[�g���X�V����
    //1�b�ȏ�o�߂��Ă�����t���[�����[�g���X�V����
    if (nDifTime > ONE_FRAME_TIME)
    {
        //�t���[�����[�g�񐔂��~���b�ɍ��킹��
        //�����܂ŏo�����̂�float�ɂ���
        float fFrameCnt = (float)(frmCnt_ * ONE_FRAME_TIME);

        //�t���[�����[�g���v�Z
        //���z�ʂ�Ȃ�6000 / 1000��60�ɂȂ�
        frmRate_ = fFrameCnt / nDifTime;

        //�t���[�����[�g���N���A
        frmCnt_ = 0;

        //�t���[�����[�g�X�V���Ԃ��X�V
        updatreFrmRateTime_ = currentTime_;
    }
}

void FpsControl::DrawFrameRate(void)
{
    DrawFormatString(0, 0, 0xFF0000, L"FPS[%.2f]", frmRate_);
}
