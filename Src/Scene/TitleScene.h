#pragma once
#include <memory>
#include "SceneBase.h"
#include "../Object/Common/Transform.h"

class SceneManager;
class AnimationController;
class ModelRenderer;
class ModelMaterial;


class TitleScene : public SceneBase
{

public:
	//UV�X�P�[��
	static constexpr float TILLING_SIZE = 35.0f;

	static constexpr float AMBIENT_COLOR = 0.2f;

	static constexpr int LOGO_OFFSET_Y = 80;
	static constexpr int LOGO_HEIGHT = 1024;
	static constexpr int PUSHIMG_OFFSET_Y = 100;

	//�R���X�g���N�^
	TitleScene(void);

	//�f�X�g���N�^
	~TitleScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;

private:
	//�}�e���A��
	std::unique_ptr<ModelMaterial> material_;
	//�����_��
	std::unique_ptr<ModelRenderer> renderer_;

	//�摜
	int pushImg_;
	int titleImg_;

	//���X�p
	Transform cafeTran_;

	//�n�ʗp
	Transform graoundTran_;

	//�L�����N�^�[
	Transform charactor_;

	//�A�j���[�V����
	std::unique_ptr<AnimationController> animationController_;

	/// <summary>
	/// �}�e���A����񏉊���
	/// </summary>
	/// <param name="">�}�e���A���̒萔�o�b�t�@�ݒ�</param>
	void InitMaterial(void);

	void UpdateDebugImGui(void);
};
