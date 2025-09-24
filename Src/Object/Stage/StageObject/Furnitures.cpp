#include "../../../Manager/Generic/ResourceManager.h"
#include "../../../Utility/CommonUtility.h"
#include "../../../Renderer/ModelMaterial.h"
#include "../../../Renderer/ModelRenderer.h"
#include "Furnitures.h"

Furnitures::Furnitures(void)
{
	material_ = nullptr;
	renderer_ = nullptr;
}

Furnitures::~Furnitures(void)
{
}

void Furnitures::Init(void)
{
	//���̊�{�ݒ�
	SetupFurniture(floor_, ResourceManager::SRC::FLOOR, CommonUtility::VECTOR_ZERO);
	floor_.MakeCollider(Collider::TYPE::STAGE);

	//���p�̃}�e���A��������
	InitMaterial();
	
	//���̑��Ƌ�̊�{�ݒ�
	SetupFurniture(deskL_, ResourceManager::SRC::DESK, DESK_L_POS);	//���̊�
	SetupFurniture(deskR_, ResourceManager::SRC::DESK, DESK_R_POS);	//�E�̊�
	SetupFurniture(sidePlant_, ResourceManager::SRC::PLANT, SIDE_PLANT_POS);	//���̊ϗt�A��
	SetupFurniture(frontPlant_, ResourceManager::SRC::PLANT, FRONT_PLANT_POS);	//�O�̊ϗt�A��
	SetupFurniture(sideShelves_, ResourceManager::SRC::SHELVES, SIDE_SHELVES_POS);	//���̒I
	SetupFurniture(frontLShelves_, ResourceManager::SRC::SHELVES_FRONT,			//�O�̍��̒I
		{ FRONT_SHELVES_R_POS_X,FRONT_SHELVES_POS.y,FRONT_SHELVES_POS.z });
	SetupFurniture(frontRShelves_, ResourceManager::SRC::SHELVES,		//�O�̉E�̒I
		FRONT_SHELVES_POS, Quaternion::Euler({ 0.0f, CommonUtility::Deg2RadF(90.0f), 0.0f }));
}

void Furnitures::Draw(void)
{
	//���̕`��
	renderer_->Draw();
	//���̑��Ƌ�̕`��
	MV1DrawModel(deskL_.modelId);			//���̊�
	MV1DrawModel(deskR_.modelId);			//�E�̊�
	MV1DrawModel(sidePlant_.modelId);		//���̊ϗt�A��
	MV1DrawModel(frontPlant_.modelId);		//�O�̊ϗt�A��
	MV1DrawModel(sideShelves_.modelId);		//���̒I
	MV1DrawModel(frontLShelves_.modelId);	//�O�̍��̒I
	MV1DrawModel(frontRShelves_.modelId);	//�O�̉E�̒I
}

void Furnitures::SetupFurniture(Transform& transform,
	ResourceManager::SRC modelSrc, const VECTOR& pos, const Quaternion& quaRotLocal)
{
	//���f���̓ǂݍ��݂Ɗ�{�ݒ�
	transform.SetModel(ResourceManager::GetInstance().LoadModelDuplicate(modelSrc));
	transform.scl = CommonUtility::VECTOR_ONE;
	transform.pos = pos;
	transform.quaRot = Quaternion();
	transform.quaRotLocal = quaRotLocal;
	transform.Update();
}

void Furnitures::InitMaterial(void)
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

	//uv�ɓn���X�P�[���l
	material_->AddConstBufVS({ TILLING_SIZE ,TILLING_SIZE,TILLING_SIZE,TILLING_SIZE });

	//�F�̉e���x
	const float colorPow = 1.0f;
	material_->AddConstBufPS({ colorPow,colorPow,colorPow,colorPow });

	//���C�g�̕���
	VECTOR light = GetLightDirection();
	material_->AddConstBufPS({ light.x,light.y,light.z,0.0f });	//4�ڂ̓_�~�[

	//����
	material_->AddConstBufPS({ AMBIENT_COLOR,AMBIENT_COLOR,AMBIENT_COLOR,AMBIENT_COLOR });

	//�����_���[�̐���
	renderer_ = std::make_unique<ModelRenderer>(floor_.modelId, *material_);
}