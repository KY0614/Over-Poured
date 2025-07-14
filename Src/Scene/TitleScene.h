#pragma once
#include <memory>
#include "SceneBase.h"
#include "../Object/Common/Transform.h"
class SceneManager;
class SkyDome;
class AnimationController;

class TitleScene : public SceneBase
{

public:

	static constexpr int LOGO_OFFSET_Y = 80;

	//�R���X�g���N�^
	TitleScene(void);

	//�f�X�g���N�^
	~TitleScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;

private:

	//�摜
	int pushImg_;
	int titleImg_;

	//�X�J�C�h�[���p
	Transform spaceDomeTran_;

	//�X�J�C�h�[��(�w�i)
	std::unique_ptr<SkyDome> skyDome_;

	//�f��
	Transform planet_;
	Transform movePlanet_;

	//�L�����N�^�[
	Transform charactor_;

	//�A�j���[�V����
	std::unique_ptr<AnimationController> animationController_;

};
