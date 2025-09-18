#pragma once
#include <string>
#include <functional>
#include "../ActorBase.h"
#include "../Manager/GameSystem/OrderCustomerManager.h"
#include "StageObjectLibrary.h"

class Sphere;
class Player;

class StageObject : public ActorBase
{
public:
	//�}�V����Y���W�I�t�Z�b�g(�}�V���̏�ɒu���悤�ɂ���������)
	static constexpr float MACHINE_OFSET_Z = 15.0f;	
	static constexpr float DISPENSER_OFSET_Z = 17.0f;	
	static constexpr float MACHINE_OFSET_Y = 16.0f;	

	//���b�N�̍ő�݌ɐ�
	static constexpr int SWEETS_STOCK_MAX = 4;	//�X�C�[�c�̃��b�N�̍ő�݌ɐ�
	static constexpr int CUP_STOCK_MAX = 5;		//�J�b�v�̃��b�N�̍ő�݌ɐ�

	//�P���ǉ��i��[�j����Ԋu
	static constexpr float ADD_INTERVAL = 0.7f;		

	//�I�u�W�F�N�g�̓����蔻��p���̂̔��a
	static constexpr float SWEETS_HALF_WIDTH = 18.0f;
	static constexpr float SWEETS_HEIGHT_OFFSET = 12.0f;
	static constexpr float SWEETS_Z_FRONT_OFFSET = 2.0f;
	static constexpr float SWEETS_Z_BACK_OFFSET = -19.0f;
	static constexpr float SWEETS_ROT_X = 40.0f;

	static constexpr float CUPS_HALF_WIDTH = 15.0f;
	static constexpr float CUPS_HEIGHT_OFFSET = 35.0f;
	static constexpr float CUPS_Z_OFFSET = 0.0f;

	static constexpr float LID_RACK_INTERACT_TIME = 3.0f;

	//�I�u�W�F�N�gID
	static constexpr const char* HOT_COFFEE = "Hot_Coffee";					//�z�b�g�R�[�q�[
	static constexpr const char* ICE_COFFEE = "Ice_Coffee";					//�A�C�X�R�[�q�[
	static constexpr const char* HOT_CUP = "Hot_Cup";						//�z�b�g�p�J�b�v
	static constexpr const char* ICE_CUP = "Ice_Cup";						//�A�C�X�p�J�b�v
	static constexpr const char* CUP_WITH_ICE = "Cup_With_Ice";				//�X����̃A�C�X�p�J�b�v
	static constexpr const char* TABLE = "Table";							//�e�[�u��
	static constexpr const char* COUNTER = "Counter";						//�J�E���^�[
	static constexpr const char* HOT_CUP_RACK = "Cup_Hot_Rack";				//�z�b�g�J�b�v�p���b�N
	static constexpr const char* ICE_CUP_RACK = "Cup_Ice_Rack";				//�A�C�X�J�b�v�p���b�N
    static constexpr const char* CHOCO_SWEETS_RACK = "Sweets_Choco_Rack";	//�`���R�X�C�[�c�p���b�N
	static constexpr const char* BERRY_SWEETS_RACK = "Sweets_Strawberry_Rack";//�x���[�X�C�[�c�p���b�N
	static constexpr const char* CHOCO_SWEETS = "Sweets_Choco";				//�`���R�X�C�[�c
	static constexpr const char* BERRY_SWEETS = "Sweets_Strawberry";		//�x���[�X�C�[�c
	static constexpr const char* COFFEE_MACHINE = "Coffee_Machine";			//�R�[�q�[�}�V��
	static constexpr const char* CUP_LID_RACK = "Cup_Lid_Rack";				//�W�̃��b�N
	static constexpr const char* HOTCUP_LID = "Hot_Cup_Lid";				//�W�i�z�b�g�j
	static constexpr const char* ICECUP_LID = "Ice_Cup_Lid";				//�W
	static constexpr const char* ICE_DISPENSER = "Ice_Dispenser";			//���X�@
	static constexpr const char* DUST_BOX = "Dust_Box";						//�S�~��

	//�A�C�e���I�u�W�F�N�g�̏��
	enum class ITEM_STATE 
	{
		NONE,
		PLACED,
		HOLD,
	};

	//�}�V���̉ғ����
	enum class MACHINE_STATE 
	{
		NONE,
		INACTIVE,
		ACTIVE,
	};

	//�R���X�g���N�^
	StageObject(const std::string objId,const float height,Player& player);

	//�f�X�g���N�^
	~StageObject(void);

	/// <summary>
	/// ����������
	/// </summary>
	/// <param name="pos">�I�u�W�F�N�g�𐶐����鏉�����W</param>
	/// <param name="rotY">�I�u�W�F�N�g��Y���p�x�i���͂��Ȃ��ꍇ��0�j</param>
	/// <param name="scale">�I�u�W�F�N�g�̑傫���i���͂��Ȃ��ꍇ�͂P�j</param>
	virtual void Init(VECTOR pos, float rotY = 0.0f, VECTOR scale = {1.0f,1.0f,1.0f});

	/// <summary>
	/// �X�V����
	/// </summary>
	virtual void Update(void)override;

	/// <summary>
	/// �`�揈��
	/// </summary>
	virtual void Draw(void)override;

	//�ݒ�p�A�ύX�p�֐�-------------------------------------------------------------

	/// <summary>
	/// ���W��ݒ肷��
	/// </summary>
	/// <param name="pos">�ݒ���W</param>
	void SetPos(VECTOR pos);

	/// <summary>
	/// �傫����ݒ肷��
	/// </summary>
	/// <param name="scale">�I�u�W�F�N�g�̑傫��</param>
	void SetScale(VECTOR scale);

	/// <summary>
	/// �I�u�W�F�N�g�̃C���^���N�g���Ԃ�ݒ肷��
	/// </summary>
	/// <param name="time">�ݒ肷�鎞��</param>
	void SetInteractTime(const float time) { param_.interactTime_ = time; }

	/// <summary>
	/// �I�u�W�F�N�g�ɑ΂��ăv���C���[���A�N�V�������s���Ă��Ȃ���Ԃɂ���
	/// </summary>
	void IsNotActioned(void) { isActioned_ = false; }

	/// <summary>
	/// �W���悹�Ă����Ԃɂ���
	/// </summary>
	void PutOnTheLid(void) { isLid_ = true; }

	/// <summary>
	/// �A�C�e���I�u�W�F�N�g�̏�Ԃ�ύX����
	/// </summary>
	/// <param name="state">�ύX������</param>
	void ChangeItemState(ITEM_STATE state) { itemState_ = state; }

	/// <summary>
	/// �}�V���̏�Ԃ�ύX����
	/// </summary>
	/// <param name="state">�ύX������</param>
	void ChangeMachineState(MACHINE_STATE state) { machineState_ = state; }

	//�擾�p�֐�--------------------------------------------------------------------

	/// <summary>
	/// �I�u�W�F�N�g�̍��W���擾����
	/// </summary>
	/// <param name="">transform�̍��W��Ԃ�</param>
	/// <returns>�I�u�W�F�N�g�̍��W</returns>
	VECTOR GetPos(void)const { return transform_.pos; };

	/// <summary>
	/// �����蔻��p���̂̍��W���擾����
	/// </summary>
	/// <returns>���̂̍��W</returns>
	VECTOR GetSpherePos(void)const;

	/// <summary>
	/// �I�u�W�F�N�g�̃p�����[�^���擾����
	/// </summary>
	/// <returns>�I�u�W�F�N�g�̃p�����[�^</returns>
	StageObjectLibrary::ObjectParams GetParam(void)const { return param_; }

	/// <summary>
	/// �A�C�e���I�u�W�F�N�g�̏�Ԃ��擾
	/// </summary>
	/// <param name="">itemState_��Ԃ�</param>
	/// <returns>�A�C�e���I�u�W�F�N�g�̏��</returns>
	ITEM_STATE GetItemState(void)const { return itemState_; }

	/// <summary>
	/// �}�V���I�u�W�F�N�g�̏�Ԃ��擾
	/// </summary>
	/// <param name="">machineState_��Ԃ�</param>
	/// <returns>�}�V���I�u�W�F�N�g�̏��</returns>
	MACHINE_STATE GetMachineState(void)const { return machineState_; }

	/// <summary>
	/// ���f���̏㕔�������W���擾����(�e�[�u���p)
	/// </summary>
	/// <param name="">transform��pos.y��height_�𑫂��ĕԂ�</param>
	/// <returns>�㕔�������W</returns>
	VECTOR GetTopCenter(void)const;

	/// <summary>
	/// �����蔻��p���̂̔��a���擾����
	/// </summary>
	/// <param name="">Sphere��GetRadius�֐���Ԃ�</param>
	/// <returns>���̂̔��a</returns>
	float GetSphereRad(void)const;

	/// <summary>
	/// �I�u�W�F�N�g���v���C���[����A�N�V�������s���Ă��邩�ǂ������擾
	/// </summary>
	/// <param name=""></param>
	/// <returns>�A�N�V�������Ă��邩�ǂ���</returns>
	bool IsActioned(void) const{ return isActioned_; }

	/// <summary>
	/// �W������Ă��邩�ǂ���
	/// </summary>
	/// <param name=""></param>
	/// <returns>true:����Ă���Afalse:����Ă��Ȃ�</returns>
	bool IsLidOn(void) const { return isLid_; }

	/// <summary>
	/// ���b�N�ɍ݌ɂ����邩�ǂ���
	/// </summary>
	/// <param name="">�݌ɂ��Ȃ��Ƃ���false��Ԃ�</param>
	/// <returns>ture:����@false:�Ȃ�</returns>
	bool GetHasStock(void) const { return hasStock_; }

	/// <summary>
	/// �e�[�u���R���C�_�[�p�̏����擾
	/// </summary>
	const Transform& GetTalbeColTran(void) const{ return tableColliderTran_; }

	//-------------------------------------------------------------------------

	/// <summary>
	/// �I�u�W�F�N�g�������^�ԏ���
	/// </summary>
	/// <param name="">�X�y�[�X�L�[�����Ŏ���</param>
	virtual void ItemCarry(void);

	/// <summary>
	/// �I�u�W�F�N�g��ݒu���鏈��
	/// </summary>
	/// <param name="pos">�ݒu������W</param>
	virtual void ItemPlaced(VECTOR pos);

	/// <summary>
	/// �I�u�W�F�N�g�ɑ΂��ăC���^���N�g���鏈��
	/// </summary>
	/// <param name="objId">�C���^���N�g���������I�u�W�F�N�g��ID</param>
	virtual void Interact(const std::string& objId);

	/// <summary>
	/// �I�u�W�F�N�g�����o������
	/// </summary>
	/// <param name="object"></param>
	virtual void PickUp(std::string rackName,std::vector<std::unique_ptr<StageObject>>& object);

	/// <summary>
	/// �݌ɂ��[���鏈��
	/// </summary>
	virtual void AddStock(void);

protected:
	//�I�u�W�F�N�g��ID
	std::string objId_;

	//�I�u�W�F�N�g�̃p�����[�^
	StageObjectLibrary::ObjectParams param_;

	//�e�[�u���̃R���C�_�[�p�̃��f�����
	Transform tableColliderTran_;

	//�v���C���[�̎Q��
	Player& player_;

	//�����f���̗�����
	float height_;	//����

	//�����蔻��p����
	std::unique_ptr<Sphere> sphere_;

	//�v���C���[���A�N�V�������s�������ǂ���
	bool isActioned_;	

	//�R�[�q�[�p---------------------------------------------------

	//�W�����Ă��邩�ǂ���(�R�[�q�[�ȊO�̃I�u�W�F�N�g�͊�{false�j
	bool isLid_;

	//���b�N�p-----------------------------------------------------

	//���b�N�ɍ݌ɂ����邩�ǂ���
	bool hasStock_;

	//�A�C�e���I�u�W�F�N�g�p�֐�-----------------------------------

	/// <summary>
	///�@�A�C�e�����ݒu��Ԃ̎��̍X�V����
	/// </summary>
	virtual void UpdatePlaced(void);

	/// <summary>
	/// �A�C�e����������Ԃ̎��̍X�V����
	/// </summary>
	/// <param name=""></param>
	virtual void UpdateHold(void);

	//�}�V���I�u�W�F�N�g�p�֐�-------------------------------------
	
	/// <summary>
	/// ��ғ���Ԃ̎��̍X�V����
	/// </summary>
	/// <param name=""></param>
	virtual void UpdateInActive(void);

	/// <summary>
	///�@�ғ���Ԃ̎��̍X�V����
	/// </summary>
	/// <param name=""></param>
	virtual void UpdateActive(void);

private:
	//�A�C�e���I�u�W�F�N�g�̏��
	ITEM_STATE itemState_;

	//�}�V���̉ғ����
	MACHINE_STATE machineState_;

	void InitTableColliderModel(void);
};