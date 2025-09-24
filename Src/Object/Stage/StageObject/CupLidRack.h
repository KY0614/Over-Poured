#pragma once
#include "../StageObject.h"

class GaugeUI;

class CupLidRack : public StageObject
{
public:
	//�W����܂ł̎���
	static constexpr float LID_PRODUCES_TIME = 3.0f;

	//�R���X�g���N�^
	CupLidRack(const std::string objId, Player& player,
		std::vector<std::unique_ptr<StageObject>>& object);
	//�f�X�g���N�^
	~CupLidRack(void) = default;

	/// <summary>
	/// �C���^���N�g����
	/// </summary>
	/// <param name="objId">�̂Ă�I�u�W�F�N�gid</param>
	void Interact(const std::string& objId) override;

	/// <summary>
	/// ����������
	/// </summary>
	/// <param name="pos">����������W</param>
	/// <param name="rotY">��]�p�iY���j</param>
	/// <param name="scale">�傫��</param>
	void Init(VECTOR pos, float rotY = 0.0f, VECTOR scale = { 1.0f,1.0f,1.0f })override;

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update(void)override;

	/// <summary>
	/// �`�揈��
	/// </summary>
	void Draw(void)override;

private:
	//�I�u�W�F�N�g�̎Q��
	std::vector<std::unique_ptr<StageObject>>& objects_;

	//�Q�[�W��UI
	std::unique_ptr<GaugeUI> gaugeUI_;
};

