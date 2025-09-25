#pragma once
#include <memory>
#include "SceneBase.h"

class Score;

class ResultScene : public SceneBase
{
public:

	// �R���X�g���N�^
	ResultScene(void);

	// �f�X�g���N�^
	~ResultScene(void);

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

private:
	//�X�R�A
	std::unique_ptr<Score> score_;

};

