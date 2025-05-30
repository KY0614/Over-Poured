#pragma once
#include <memory>
#include <map>
#include "../ActorBase.h"
#include "../../Common/Vector2.h"

class ResourceManager;
class StageObject;
class Player;

class StageManager : public ActorBase
{

public:
	//�I�u�W�F�N�g�֘A

	//�e�[�u���֘A
	static constexpr int TABLE_NUM = 4;			//�e�[�u���̐�

	static constexpr float TABLE_WIDTH = 95.0f;	//�e�[�u���̉���

	//���W

	static constexpr VECTOR TABLE_POS = { -140.0f, 0.0f, -175.0f };	//�e�[�u���̍��W
	static constexpr VECTOR COLUMN_TABLE_POS = { -222.0f, 0.0f, -98.0f };	//��e�[�u���̍��W

	static constexpr VECTOR MACHINE_POS = { -128.0f, 76.0f, -175.0f };	//�R�[�q�[�}�V���̍��W
	static constexpr VECTOR CUPHOT_POS = { -45.0f, 76.0f, -175.0f };	//�z�b�g�p�J�b�v�̍��W
	static constexpr VECTOR CUPICE_POS = { 45.0f, 76.0f, -175.0f };		//�A�C�X�p�J�b�v�̍��W
	static constexpr VECTOR ICEDIS_POS = { 133.0f, 76.0f, -175.0f };	//�A�C�X�f�B�X�y���T�[�̍��W
	static constexpr VECTOR LIBS_POS = { 74.0f, 76.0f, -175.0f };		//�J�b�v�̊W�̍��W
	static constexpr VECTOR DUSTBOX_POS = { 227.0f, 0.0f, -175.0f };	//�S�~���̍��W
	
	//�R���X�g���N�^
	StageManager(Player& player);

	//�f�X�g���N�^
	~StageManager(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;

	/// <summary>
	/// �z�b�g�R�[�q�[�����
	/// </summary>
	/// <param name="">�}�V���ƃz�b�g�p�J�b�v�����������A�J�b�v�̏ꏊ�ɃR�[�q�[����</param>
	void MakeHotCoffee(void);

	/// <summary>
	/// �R�[�q�[�ɊW������
	/// </summary>
	/// <param name=""></param>
	void LidFollowCup(void);

	void SurveItem(void);

private:

	//�v���C���[�̎Q��
	Player& player_;

	//�X�e�[�W�ɔz�u����I�u�W�F�N�g�B
	std::vector<std::unique_ptr<StageObject>> objects_;

	//�X�e�[�W�ɔz�u�����
	std::vector<std::unique_ptr<StageObject>> tables_;

	std::vector<std::vector<std::unique_ptr<StageObject>>> grid_;

	bool IsInBounds(int x, int y) const;

	void UpdateDebugImGui(void);
};
