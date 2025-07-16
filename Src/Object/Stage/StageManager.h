#pragma once
#include <memory>
#include <vector>
#include <map>
#include <functional>
#include "../Order/Order.h"
#include "../ActorBase.h"

class StageObject;
class Furnitures;
class Player;
class AnimationController;

class StageManager : public ActorBase
{

public:
	//�I�u�W�F�N�g�֘A

	//�e�[�u���֘A
	static constexpr int TABLE_COLUMN_NUM = 4;		//�����̃e�[�u���̐��i�P��j
	static constexpr int TABLE_ROW_BACK_NUM = 4;	//��O���̃e�[�u���̐�
	static constexpr int TABLE_ROW_FRONT_NUM = 2;	//�����̃e�[�u���̐�
	static constexpr int TABLE_CENTER_NUM = 4;	//�e�[�u���̐�
	static constexpr int MAX_TABLE_NUM = 16;	//�e�[�u���̐�

	static constexpr float TABLE_WIDTH = 92.5f;	//�e�[�u���̉���

	//���W

	static constexpr VECTOR TABLE_POS_BACK = { -150.0f, 0.0f, -270.0f };	//�e�[�u���̍��W(��O���j
	static constexpr VECTOR TABLE_POS_FRONT = { -115.0f, 0.0f, 180.0f };	//�e�[�u���̍��W�i�����j
	static constexpr VECTOR COLUMN_TABLE_LEFT_POS = { -250.0f, 0.0f, -192.0f };		//��i���j�e�[�u���̍��W
	static constexpr VECTOR COLUMN_TABLE_RIGHT_POS = { 320.0f, 0.0f, -192.0f };	//��(�E�j�e�[�u���̍��W
	static constexpr VECTOR CENTER_TABLE_POS = { -20.0f,0.0f,-100.0f };	//��(�E�j�e�[�u���̍��W
	static constexpr VECTOR COUNTER_POS = { 225.0f, 0.0f, 190.0f };			//�J�E���^�[�̍��W
	static constexpr VECTOR CASE_POS = { -57.0f, 0.0f, 190.0f };			//�V���[�P�[�X�̍��W

	static constexpr VECTOR MACHINE_POS = { -128.0f, 76.0f, -175.0f };	//�R�[�q�[�}�V���̍��W
	static constexpr VECTOR CUPHOT_POS = { -45.0f, 76.0f, -175.0f };	//�z�b�g�p�J�b�v�̍��W
	static constexpr VECTOR CUPICE_POS = { 45.0f, 76.0f, -175.0f };		//�A�C�X�p�J�b�v�̍��W
	static constexpr VECTOR ICEDIS_POS = { 133.0f, 76.0f, -175.0f };	//�A�C�X�f�B�X�y���T�[�̍��W
	static constexpr VECTOR DUSTBOX_POS = { 240.0f, 0.0f, -270.0f };	//�S�~���̍��W

	//�A�j���[�V�������
	enum class ANIM_TYPE
	{
		CREATE,
		IDLE,
		PAYING,
		
	};
	
	//�R���X�g���N�^
	StageManager(Player& player);

	//�f�X�g���N�^
	~StageManager(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;

	/// <summary>
	/// ���݂̐擪�̂��q�̒�����ݒ肷��
	/// </summary>
	/// <param name="order">�擪�̂��q�̒���</param>
	void SetCurrentOrder(const Order::OrderData& order);

	/// <summary>
	/// �񋟂���A�C�e�����擾����
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	Order::OrderData GetServeItems(void)const { return servedItems_; }

	/// <summary>
	/// �񋟂������ǂ������擾����
	/// </summary>
	/// <param name=""></param>
	/// <returns>true:�񋟂����@flase:����</returns>
	bool IsServed(void) const { return isServed_; }

	/// <summary>
	/// �񋟃f�[�^�����Z�b�g����
	/// </summary>
	/// <param name=""></param>
	void ResetServeData(void);

	int GetTableNum(void) const { return static_cast<int>(tables_.size()); }

	Transform GetCounterTran(void) const;
	Transform GetTableTran(int index) const;
	Transform GetShowCase(void) const;
	Transform GetDustBox(void) const;
	Transform GetFloorTran(void) const;

private:

	//�A�j���[�V����
	std::unique_ptr<AnimationController> animationController_;

	//�X�C�[�c�P�[�X�p�g�����X�t�H�[��
	Transform caseTran_;
	//�S�~���p�g�����X�t�H�[��
	Transform dustBoxTran_;

	//�v���C���[�̎Q��
	Player& player_;

	//�X�e�[�W�ɔz�u����I�u�W�F�N�g�B
	std::vector<std::unique_ptr<StageObject>> objects_;

	//�X�e�[�W�ɔz�u�����
	std::vector<std::unique_ptr<StageObject>> tables_;

	//�J�E���^�[�p
	std::unique_ptr<StageObject> counter_;
	std::unique_ptr<StageObject> case_;

	std::unique_ptr<Furnitures> furnitures_;

	bool isServed_;

	//�񋟍ςݏ��i
	Order::OrderData servedItems_;

	//���݂̂��q�̒������e
	Order::OrderData currentOrder_;

	//�e�������񋟂��ꂽ���ǂ����̃t���O
	std::vector<bool> isServedItems_; 

	void Init3DModel(void);

	void InitAnimation(void);

	/// <summary>
	/// �񋟂���A�C�e����ݒ肷��
	/// </summary>
	/// <param name="obj"></param>
	void SurveItem(StageObject& obj);

	/// <summary>
	/// �񋟂����A�C�e�����폜����
	/// </summary>
	/// <param name=""></param>
	void DeleteSurvedItem(void);

	/// <summary>
	/// �����^�щ\�ȃI�u�W�F�N�g�̃C���^���N�g����
	/// </summary>
	/// <param name=""></param>
	void CarryableObjInteract(void);

	/// <summary>
	/// �}�V���̃C���^���N�g����
	/// </summary>
	/// <param name="">�R�[�q�[�}�V���ƃA�C�X�f�B�X�y���T�[�̃C���^���N�g����</param>
	void MachineInteract(void);

	/// <summary>
	/// �W�̃��b�N�̃C���^���N�g����
	/// </summary>
	/// <param name="">�C���^���N�g���̓v���C���[�͑���ł��Ȃ�</param>
	void LidRackInteract(void);

	/// <summary>
	/// �R�[�q�[�𐶐����鏈��
	/// </summary>
	/// <param name="">�}�V���ɐݒu����Ă���J�b�v�ɑ΂��čs������/param>
	void ProduceCoffee(int index);

	/// <summary>
	/// �R�[�q�[����鏈��
	/// </summary>
	/// <param name="index">�z��ԍ��i�J�b�v���R�[�q�[�ɏ㏑������̂Łj</param>
	/// <param name="pos">����������W</param>
	/// <param name="objName">���R�[�q�[�̃p�����[�^���i�z�b�g���A�C�X)</param>
	void MakeCoffee(int index,StageObject& obj,std::string objName);

	/// <summary>
	/// �J�b�v�ɃA�C�X�f�B�X�y���T�[�ŕX�����鏈��
	/// </summary>
	/// <param name="index">�w�肷��A�C�X�f�B�X�y���T�[�̗v�f�ԍ�</param>
	void DispenseIce2Cup(int index);

	/// <summary>
	/// �R�[�q�[�ɊW������
	/// </summary>
	/// <param name=""></param>
	void LidFollowCup(void);

	/// <summary>
	/// �S�~���̃C���^���N�g����
	/// </summary>
	/// <param name=""></param>
	void DustBoxInteract(void);

	/// <summary>
	/// �I�[�_�[���ɉ��������̃A�C�e����񋟂��Ă��邩�ǂ���
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	bool IsOrderCompleted(void);
};
