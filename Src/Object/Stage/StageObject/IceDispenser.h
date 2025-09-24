#pragma once
#include "../StageObject.h"

class GaugeUI;
class IconUI;

class IceDispenser : public StageObject
{
public:

	//�X�𐶐�����܂ł̎���
	static constexpr float ICE_PRODUCES_TIME = 3.0f;

	//�R���X�g���N�^
	IceDispenser(const std::string objId,Player& player,
		std::vector<std::unique_ptr<StageObject>>& object);
	//�f�X�g���N�^
	~IceDispenser(void) = default;

	/// <summary>
	/// ����������
	/// </summary>
	/// <param name="pos">����������W</param>
	/// <param name="rotY">��]�p�iY���j</param>
	/// <param name="scale">�傫��</param>
	void Init(VECTOR pos, float rotY = 0.0f, VECTOR scale = { 1.0f,1.0f,1.0f })override;

	/// <summary>
	/// �`�揈��
	/// </summary>
	void Draw(void) override;

	/// <summary>
	/// �C���^���N�g����
	/// </summary>
	/// <param name="objId">�C���^���N�g����I�u�W�F�N�gid</param>
	void Interact(const std::string& objId) override;

private:

	//�I�u�W�F�N�g�̎Q��
	std::vector<std::unique_ptr<StageObject>>& objects_;

	//�Q�[�W��UI
	std::unique_ptr<GaugeUI> gaugeUI_;
	//�A�C�R����UI
	std::unique_ptr<IconUI> iconUI_;


	/// <summary>
	/// ��ғ����̍X�V����
	/// </summary>
	void UpdateInActive(void)override;

	/// <summary>
	/// �ғ����̍X�V����
	/// </summary>
	void UpdateActive(void)override;

};

