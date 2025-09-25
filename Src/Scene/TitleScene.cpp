#include <string>
#include <DxLib.h>
#include "../Application.h"
#include "../Libs/ImGui/imgui.h"
#include "../Utility/CommonUtility.h"
#include "../Manager/GameSystem/SoundManager.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/ResourceManager.h"
#include "../Manager/Generic/InputManager.h"
#include "../Manager/Generic/Camera.h"
#include "../Object/Common/AnimationController.h"
#include "../Renderer/ModelMaterial.h"
#include "../Renderer/ModelRenderer.h"
#include "TitleScene.h"

namespace
{
	//�n�C���C�g�̓_�ŊԊu
	const float HIGH_LIGHT_INTERVAL = 1.2f;

	//UV�X�P�[��
	const float TILLING_SIZE = 35.0f;

	//�����̋���
	const float AMBIENT_COLOR = 0.2f;

	//�摜�̈ʒu
	const int LOGO_OFFSET_Y = 80;	//���S��Y�ʒu
	const int LOGO_HEIGHT = 1024;	//���S�̍���
	const int PUSHIMG_OFFSET_Y = 100;	//�����摜��Y�ʒu
}

TitleScene::TitleScene(void)
{
	pushImg_ = -1;
	titleImg_ = -1;
	animationController_ = nullptr;
	isView_ = false;
	highlightTime_ = 0.0f;
}

TitleScene::~TitleScene(void)
{

}

void TitleScene::Init(void)
{
	//�T�E���h������
	InitSound();

	//�摜�ǂݍ���
	LoadImages();

	//���f��������
	Init3DModel();

	const float animSpeed = 20.0f;
	//�A�j���[�V�����̐ݒ�
	std::string path = Application::PATH_MODEL + "Player/";
	animationController_ = std::make_unique<AnimationController>(character_.modelId);
	animationController_->Add(0, path + "Idle.mv1", animSpeed);
	animationController_->Play(0);

	//��_�J����
	mainCamera->ChangeMode(Camera::MODE::FIXED_POINT);
	isView_ = true;
}

void TitleScene::Update(void)
{
	//�V�[���J��
	InputManager& ins = InputManager::GetInstance();
	//�_�Ŏ��ԍX�V
	highlightTime_ += SceneManager::GetInstance().GetDeltaTime();
	//�X�y�[�X�L�[�Ń`���[�g���A����
	if (ins.IsInputTriggered("Interact"))
	{
		//SE�Đ�
		SoundManager::GetInstance().Play(SoundManager::SOUND::PUSH_SPACE);
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TUTORIAL);
	}
	//�_�ŏ���
	if (highlightTime_ >= HIGH_LIGHT_INTERVAL)
	{
		isView_ = !isView_;	//�\����\���؂�ւ�
		highlightTime_ = 0.0f;
	}
	//�L�����A�j���[�V����
	animationController_->Update();
	//���f���̍X�V
	cafeTran_.Update();
	character_.Update();
	groundTran_.Update();
}

void TitleScene::Draw(void)
{
	//���S�������߂ɏk�����Ă���̂ŃW���M�[���ڗ����Ȃ��悤�Ƀo�C���j�A�@�ŕ`��
	SetDrawMode(DX_DRAWMODE_BILINEAR);
	//���̕`��(�V�F�[�_�[�g�p����)
	renderer_->Draw();
	//���f���̕`��
	MV1DrawModel(cafeTran_.modelId);
	MV1DrawModel(character_.modelId);

	//��ʂ̑傫���ɍ��킹�Ċg�嗦��ς���
	float screenScl =
		static_cast<float>(Application::SCREEN_SIZE_Y) /
		static_cast<float>(Application::SCREEN_MAX_SIZE_Y);
	//���S�̊g�嗦
	const float logoScale = 0.8f;
	//���S�͏��������߂ɒ���
	screenScl *= logoScale;

	//pushspace�摜��z�u���邽�߂Ƀ��S�̑傫�����m��
	const int drawScl = (int)((float)(LOGO_HEIGHT / 2) * screenScl);
	if (isView_)
	{
		const float scale = 2.0f;
		//pushspace�̉摜
		DrawRotaGraph(
			Application::SCREEN_SIZE_X / 2,
			Application::SCREEN_SIZE_Y / 2 + drawScl,
			scale, 0.0, pushImg_, true);
	}

	//���S�摜
	DrawRotaGraph(
		Application::SCREEN_SIZE_X / 2,
		Application::SCREEN_SIZE_Y/2 - LOGO_OFFSET_Y,
		screenScl, 0.0, titleImg_, true);
	
}

void TitleScene::Init3DModel(void)
{
	//���X�̍��W�Ɖ�]�p
	const VECTOR cafePos = { 250.0f,0.0f,300.0f };
	const float cafeRot = 90.0f;
	//���X
	cafeTran_.SetModel(
		ResourceManager::GetInstance().
		LoadModelDuplicate(ResourceManager::SRC::CAFE));
	cafeTran_.pos = cafePos;
	cafeTran_.scl = CommonUtility::VECTOR_ONE;
	cafeTran_.quaRot = Quaternion::Euler(
		0.0f, CommonUtility::Deg2RadF(cafeRot), 0.0f);
	cafeTran_.Update();
	//�n�ʂ̍��W�Ɗg�嗦
	const VECTOR graoundPos = { -4500.0f, 0.0f, 6100.0f };
	const float cafeScl = 2.5f;
	//�n��
	groundTran_.SetModel(
		ResourceManager::GetInstance().
		LoadModelDuplicate(ResourceManager::SRC::FLOOR));
	groundTran_.pos = graoundPos;
	groundTran_.scl = { cafeScl,cafeScl,cafeScl };
	groundTran_.quaRot = CommonUtility::VECTOR_ZERO;
	groundTran_.MakeCollider(Collider::TYPE::STAGE);
	groundTran_.Update();

	//�}�e���A��������
	InitMaterial();

	//�L�����̍��W
	const VECTOR charaPos = { -390.0f,0.0f, -265.0f };
	//�L����
	character_.SetModel(
		ResourceManager::GetInstance().
		LoadModelDuplicate(ResourceManager::SRC::PLAYER));
	character_.pos = charaPos;
	character_.scl = CommonUtility::VECTOR_ONE;
	character_.quaRot = Quaternion();
	character_.Update();

}

void TitleScene::InitMaterial(void)
{
	//�萔�o�b�t�@�p�̒萔
	const int VSbufferSize = 1;
	const int PSbufferSize = 3;
	//���f���`��p
	material_ = std::make_unique<ModelMaterial>(
		"StdModelVS.cso", VSbufferSize,
		"StdModelPS.cso", PSbufferSize
	);
	//�^�C�����O����̂Ńe�N�X�`���A�h���X��WRAP��
	material_->SetTextureAddress(ModelMaterial::TEXADDRESS::WRAP);
	material_->SetTextureBuf(0, ResourceManager::GetInstance().
		Load(ResourceManager::SRC::GROUND).handleId_);

	//uv�ɓn���X�P�[���l
	material_->AddConstBufVS({ TILLING_SIZE ,TILLING_SIZE,TILLING_SIZE,TILLING_SIZE });

	//�F�̉e���x
	material_->AddConstBufPS({ CommonUtility::VECTOR_ONE.x,CommonUtility::VECTOR_ONE.y,CommonUtility::VECTOR_ONE.z,1.0f });

	//���C�g�̕���
	VECTOR light = GetLightDirection();
	//4�ڂ̒l�̓_�~�[
	material_->AddConstBufPS({ light.x,light.y,light.z,0.0f });

	//����
	material_->AddConstBufPS({ AMBIENT_COLOR,AMBIENT_COLOR,AMBIENT_COLOR,AMBIENT_COLOR });

	//���f�������_���[����
	renderer_ = std::make_unique<ModelRenderer>(groundTran_.modelId, *material_);
}

void TitleScene::LoadImages(void)
{
	//�摜�ǂݍ���
	//pushspace�摜
	pushImg_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::PUSH_SPACE).handleId_;
	//���S�摜
	titleImg_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::TITLE_LOGO).handleId_;
}

void TitleScene::InitSound(void)
{
	auto& sound = SoundManager::GetInstance();

	//BGM�o�^���ĉ��ʒ��ߌ�ɍĐ�
	sound.Add(SoundManager::TYPE::BGM, SoundManager::SOUND::TITLE,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::TITLE_BGM).handleId_);
	sound.AdjustVolume(SoundManager::SOUND::TITLE, VOLUME_MAX / 2);
	sound.Play(SoundManager::SOUND::TITLE);

	//SE�o�^���ĉ��ʒ���
	sound.Add(SoundManager::TYPE::SE, SoundManager::SOUND::PUSH_SPACE,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::PUSH_SPACE_SE).handleId_);
	sound.AdjustVolume(SoundManager::SOUND::PUSH_SPACE, VOLUME_MAX / 3);

}