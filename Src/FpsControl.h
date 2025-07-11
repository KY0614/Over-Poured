#pragma once

class FpsControl
{
public:
    static constexpr float FRAME_RATE = (1000 / 60);
    static constexpr int ONE_FRAME_TIME = 1000;

    FpsControl();

    void Init(void);

    bool UpdateFrameRate(void);

    void CalcFrameRate(void);

    void DrawFrameRate(void);

private:
    int currentTime_;   //���݂̎���
    int prevFrmTime_;   //�O��̃t���[�����s���̎���

    int frmCnt_;      //�t���[���J�E���g�p
    int updatreFrmRateTime_;    //�t���[�����[�g���X�V��������

    float frmRate_;     //�t���[�����[�g

};