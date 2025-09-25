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

	//�R���X�g���N�^
	TitleScene(void);

	//�f�X�g���N�^
	~TitleScene(void);

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
	//�}�e���A��
	std::unique_ptr<ModelMaterial> material_;
	//�����_��
	std::unique_ptr<ModelRenderer> renderer_;

	//�摜
	int pushImg_;	//�����摜
	int titleImg_;	//���S�摜

	//�r���[�؂�ւ�
	bool isView_;	//true:�\��,false:��\��
	//�n�C���C�g�o�ߎ���
	float highlightTime_;

	//���X�p���f�����
	Transform cafeTran_;

	//�n�ʗp���f�����
	Transform groundTran_;

	//�L�����N�^�[���f�����
	Transform character_;

	//�A�j���[�V����
	std::unique_ptr<AnimationController> animationController_;

	/// <summary>
	/// ���f��������
	/// </summary>
	void Init3DModel(void);

	/// <summary>
	/// �}�e���A����񏉊���
	/// </summary>
	/// <param name="">�}�e���A���̒萔�o�b�t�@�ݒ�</param>
	void InitMaterial(void);

	/// <summary>
	/// �摜�ǂݍ���
	/// </summary>
	void LoadImages(void);

	/// <summary>
	/// �T�E���h������
	/// </summary>
	void InitSound(void);
};
