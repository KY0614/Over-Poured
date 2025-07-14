#pragma once
#include <memory>
#include "../../ActorBase.h"

class ModelRenderer;
class ModelMaterial;

class Furnitures : public ActorBase
{
public:
	//���W�֘A
	static constexpr VECTOR SIDE_PLANT_POS = { -255.0f,0.0f,-280.0f };
	static constexpr VECTOR FRONT_PLANT_POS = { 170.0f,0.0f,465.0f };
	static constexpr VECTOR DESK_R_POS = { -190.0f,0.0f,635.0f };
	static constexpr VECTOR DESK_L_POS = { 405.0f,0.0f,635.0f };
	static constexpr VECTOR SIDE_SHELVES_POS = { -380.0f,0.0f,-35.0f };
	static constexpr VECTOR FRONT_SHELVES_POS = { -25.0f,0.0f,465.0f };
	static constexpr float FRONT_SHELVES_R_POS_X = 350.0f;

	//UV�X�P�[��
	static constexpr float TILLING_SIZE = 35.0f;

	static constexpr float AMBIENT_COLOR = 0.2f;

	//�R���X�g���N�^
	Furnitures(void);

	//�f�X�g���N�^
	~Furnitures(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;

	const Transform GetFloorTran(void)const { return floor_; }

private:

	std::unique_ptr<ModelMaterial> material_;
	std::unique_ptr<ModelRenderer> renderer_;

	Transform floor_;
	Transform deskL_;
	Transform deskR_;
	Transform sidePlant_;
	Transform frontPlant_;
	Transform sideShelves_;
	Transform frontLShelves_;
	Transform frontRShelves_;

	/// <summary>
	/// ���f���Z�b�g�A�b�v�p�֐�
	/// </summary>
	/// <param name="transform">���f����Transform</param>
	/// <param name="modelSrc">���f���̃\�[�X�^�C�v</param>
	/// <param name="pos">�������W</param>
	/// <param name="quaRotLocal">��]</param>
	void SetupFurniture(Transform& transform,
		ResourceManager::SRC modelSrc, const VECTOR& pos,
		const Quaternion& quaRotLocal = Quaternion());

	void UpdateDebugImGui(void);
};

