#pragma once
#include <memory>
#include "SceneBase.h"

class StageManager;
class Player;
class OrderCustomerManager;
class Timer;

class TutorialScene : public SceneBase
{
public:
	//�w�i�摜�֘A
	static constexpr int BACK_IMG_MARGINE = 250;	//�������W���班�������Ԋu��������
	static constexpr int BACK_IMG_SCALE = 500;		//�w�i�摜�̑傫��

	static constexpr int CURSOR_IMG_MARGINE = 300;	//�������W���班�������Ԋu��������

	static constexpr int TUTORIAL_IMG_MAX_NUM = 3;				//�����p�摜�̍ő喇��
	static constexpr int INDEX_MAX = TUTORIAL_IMG_MAX_NUM - 1;	//�O����Ȃ̂łP�����Ă���

	static constexpr float HIGH_LIGHT_INTERVAL = 1.2f;

	static constexpr int LOGO_HEIGHT = 1024;

	enum class STEP
	{
		OPERATION,	//�������
		ORDER,		//��������
		CUSTOMER,	//�q�̐���
		FINISH,		//�I��
	};

	//�R���X�g���N�^
	TutorialScene(void);

	//�f�X�g���N�^
	~TutorialScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;

private:
	//�֐��|�C���^
	using UpdateFunc_t = void(TutorialScene::*)();
	using DrawFunc_t = void(TutorialScene::*)();

	UpdateFunc_t update_;
	DrawFunc_t draw_;

	//�X�e�[�W
	std::unique_ptr<StageManager> stage_;

	//�v���C���[
	std::unique_ptr<Player> player_;

	//�q
	std::unique_ptr<OrderCustomerManager> customer_;

	void UpdateOperation(void);
	
	void DrawOperation(void);

	//�摜�Ő����p�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[

	//�����p�摜
	int* tutorialImgs_;
	int tutorialBackImg_;
	int* cursorImg_;

	int decoImg_;

	int pushImg_;	//�����摜

	//�����p�摜�̃C���f�b�N�X
	int imgIdx_;

	int blinkTime_;
	int blinkIdx_;

	bool isBlinkR_;
	bool isBlinkL_;

	bool isView_;
	float highlightTime_;

	void ImageInit(void);
	void ImageUpdate(void);
	void ImageDraw(void);
};

