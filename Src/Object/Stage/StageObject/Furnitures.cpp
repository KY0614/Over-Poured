#include "../../../Libs/ImGui/imgui.h"
#include "../../../Manager/Generic/ResourceManager.h"
#include "../../../Utility/AsoUtility.h"
#include "../../../Renderer/ModelMaterial.h"
#include "../../../Renderer/ModelRenderer.h"
#include "Furnitures.h"

Furnitures::Furnitures(void)
{
}

Furnitures::~Furnitures(void)
{
}

void Furnitures::Init(void)
{
	//���f���̊�{�ݒ�
	SetupFurniture(floor_, ResourceManager::SRC::FLOOR, AsoUtility::VECTOR_ZERO);

	//���f���`��p
	material_ = std::make_unique<ModelMaterial>(
		"StdModelVS.cso", 1,
		"StdModelPS.cso", 3
	);
	//�^�C�����O����̂Ńe�N�X�`���A�h���X��WRAP��
	material_->SetTextureAddress(ModelMaterial::TEXADDRESS::WRAP);

	//uv�ɓn���X�P�[���l
	material_->AddConstBufVS({ TILLING_SIZE ,TILLING_SIZE,TILLING_SIZE,TILLING_SIZE });

	//�F�̉e���x
	material_->AddConstBufPS({ AsoUtility::VECTOR_ONE.x,AsoUtility::VECTOR_ONE.y,AsoUtility::VECTOR_ONE.z,1.0f });

	//���C�g�̕���
	VECTOR light = GetLightDirection();
	material_->AddConstBufPS({ light.x,light.y,light.z,1.0f });

	//����
	material_->AddConstBufPS({ AMBIENT_COLOR,AMBIENT_COLOR,AMBIENT_COLOR,AMBIENT_COLOR });

	renderer_ = std::make_unique<ModelRenderer>(floor_.modelId, *material_);

	SetupFurniture(deskL_, ResourceManager::SRC::DESK, DESK_L_POS);
	SetupFurniture(deskR_, ResourceManager::SRC::DESK, DESK_R_POS);
	SetupFurniture(sidePlant_, ResourceManager::SRC::PLANT, SIDE_PLANT_POS);
	SetupFurniture(frontPlant_, ResourceManager::SRC::PLANT, FRONT_PLANT_POS);
	SetupFurniture(sideShelves_, ResourceManager::SRC::SHELVES, SIDE_SHELVES_POS);
	SetupFurniture(frontLShelves_, ResourceManager::SRC::SHELVES_FRONT,
		{ FRONT_SHELVES_R_POS_X,FRONT_SHELVES_POS.y,FRONT_SHELVES_POS.z });
	SetupFurniture(frontRShelves_, ResourceManager::SRC::SHELVES,
		FRONT_SHELVES_POS, Quaternion::Euler({ 0.0f, AsoUtility::Deg2RadF(90.0f), 0.0f }));
}

void Furnitures::Update(void)
{
	//�������\����ɂȂ��̂ŉ����������Ȃ�
}

void Furnitures::Draw(void)
{
	renderer_->Draw();
	MV1DrawModel(deskL_.modelId);
	MV1DrawModel(deskR_.modelId);
	MV1DrawModel(sidePlant_.modelId);
	MV1DrawModel(frontPlant_.modelId);
	MV1DrawModel(sideShelves_.modelId);
	MV1DrawModel(frontLShelves_.modelId);
	MV1DrawModel(frontRShelves_.modelId);
}

void Furnitures::SetupFurniture(Transform& transform,
	ResourceManager::SRC modelSrc, const VECTOR& pos, const Quaternion& quaRotLocal)
{
	transform.SetModel(ResourceManager::GetInstance().LoadModelDuplicate(modelSrc));
	transform.scl = AsoUtility::VECTOR_ONE;
	transform.pos = pos;
	transform.quaRot = Quaternion();
	transform.quaRotLocal = quaRotLocal;
	transform.Update();
}

void Furnitures::UpdateDebugImGui(void)
{
	//�E�B���h�E�^�C�g��&�J�n����
	ImGui::Begin("case");

	//�ʒu
	ImGui::Text("plant");
	//�\���̂̐擪�|�C���^��n���Axyz�ƘA�������������z�u�փA�N�Z�X
	ImGui::InputFloat3("Pos", &deskR_.pos.x);
	ImGui::SliderFloat("PosX", &deskR_.pos.x, -1000.0f, 1000.0f);
	ImGui::SliderFloat("PosY", &deskR_.pos.y, 0.0f, 500.0f);
	ImGui::SliderFloat("PosZ", &deskR_.pos.z, -1000.0f, 1000.0f);

	//�I������
	ImGui::End();
}