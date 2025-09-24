#pragma once
#include "UIBase.h"

class GaugeUI : public UIBase
{
public:
	//�f�t�H���g��UI�T�C�Y
	static constexpr float UI_DEFAULT_WIDTH = 80.0f;	//��
	static constexpr float UI_DEFAULT_HEIGHT = 10.0f;	//����

	//�R���X�g���N�^
    GaugeUI(bool isCircle, float activeTime);
	//�f�X�g���N�^
	~GaugeUI(void) = default;

	/// <summary>
	/// ����������
	/// </summary>
	void Init(void) override;

	/// <summary>
	/// �X�V����
	/// </summary>
    void Update(void) override;

	/// <summary>
	/// �`�揈��
	/// </summary>
    void Draw(void) override;

	/// <summary>
	/// �\�����Ԃ�ݒ�
	/// </summary>
	/// <param name="time">�\�����鎞��</param>
	void SetActiveTime(const float time) { activeTime_ = time; }

	/// <summary>
	/// UI�̃T�C�Y��ݒ�
	/// </summary>
	/// <param name="width"></param>
	/// <param name="height"></param>
	void SetUISize(const float width, const float height) { width_ = width; height_ = height; }

	/// <summary>
	/// UI�̕\�������Z�b�g
	/// </summary>
	void Reset(void);

private:
	//�e�̉摜ID
	int shadowImg_;

	//�~�`�̉摜ID
	int circleImg_;
	
	//�~�`�̉e�摜ID
	int cShadowImg_;	
	
	//�ő�\������
    float activeTime_;
	
	//�o�ߎ���
    float currentTime_;
	
	//true: �~�`, false: �����`
    bool isCircle_;     

	//UI�̃T�C�Y
	float width_;  //UI�̕�
	float height_; //UI�̍���

	/// <summary>
	/// �~�Q�[�W��`��
	/// </summary>
	/// <param name="progress">�Q�[�W�̐i�s�x</param>
	void DrawCircleGauge(float progress);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="progress">�Q�[�W�̐i�s�x</param>
	void DrawRectGauge(float progress);
};

