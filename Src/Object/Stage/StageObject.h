#pragma once
#include <string>
#include <functional>
#include "../ActorBase.h"
#include "../Manager/GameSystem/OrderCustomerManager.h"
#include "StageObjectLibrary.h"

class Cube;
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

	//�ǉ��Ԋu
	static constexpr float ADD_INTERVAL = 0.7f;	

	static constexpr float SWEETS_HALF_WIDTH = 18.0f;
	static constexpr float SWEETS_HEIGHT_OFFSET = 12.0f;
	static constexpr float SWEETS_Z_FRONT_OFFSET = 2.0f;
	static constexpr float SWEETS_Z_BACK_OFFSET = -19.0f;
	static constexpr float SWEETS_ROT_X = 40.0f;

	static constexpr float CUPS_HALF_WIDTH = 15.0f;
	static constexpr float CUPS_HEIGHT_OFFSET = 35.0f;
	static constexpr float CUPS_Z_OFFSET = 0.0f;

	//�A�C�e���I�u�W�F�N�g�̏��
	enum class ITEM_STATE {
		NONE,
		PLACED,
		HOLD,
	};

	//�}�V���̉ғ����
	enum class MACHINE_STATE {
		NONE,
		INACTIVE,
		ACTIVE,
	};

	StageObject(const std::string objId,const float width,
		const float height, const float depth,Player& player);

	~StageObject(void);

	virtual void Init(VECTOR pos,float rotY = 0.0f);
	virtual void Update(void)override;
	virtual void Draw(void)override;

	//�ݒ�p�A�ύX�p�֐�-------------------------------------------------------------

	/// <summary>
	/// ���W��ݒ肷��
	/// </summary>
	/// <param name="pos">�ݒ���W</param>
	void SetPos(VECTOR pos);

	void SetScale(VECTOR scale);

	/// <summary>
	/// �����̉�]��ݒ肷��(�����Ƃ���Euler�p�Őݒ肷��)
	/// </summary>
	/// <param name="pos">��]</param>
	void SetQuaRotY(const float localRotY);

	void SetInteractTime(const float time) { param_.interactTime_ = time; }

	void IsNotActioned(void) { isActioned_ = false; }

	void PutOnTheLid(void) { isLid_ = true; }

	void SetLidOn(const bool isLid) { isLid_ = isLid; }

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

	float GetObjHeight(void)const { return height_; }

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
	/// 
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	bool IsActioned(void) const;

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

	Order::DRINK GetDrinkType(void) const { return drink_; }
	Order::SWEETS GetSweetsType(void) const { return sweets_; }

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

	virtual void AddStock(int addStockNum);

	/// <summary>
	/// ���W��g�嗦�𒲐�����p��GUI
	/// </summary>
	/// <param name=""></param>
	void UpdateDebugImGui(void);

	void UpdateDebugImGui2(void);

protected:

	//�I�u�W�F�N�g�̃p�����[�^
	StageObjectLibrary::ObjectParams param_;

	Player& player_;

	//�����f���̗�����
	std::unique_ptr<Cube> cube_;
	float width_;	//����
	float height_;	//����
	float depth_;	//���s

	//�����蔻��p����
	std::unique_ptr<Sphere> sphere_;

	//�����蔻��p���̂̔��a
	float rad_;

	//�v���C���[���A�N�V�������s�������ǂ���
	bool isActioned_;	

	//�W�����Ă��邩�ǂ���(�R�[�q�[�ȊO�̃I�u�W�F�N�g��false�j
	bool isLid_;

	//���b�N�p------------------------

	//���b�N�ɍ݌ɂ����邩�ǂ���
	bool hasStock_;

	//--------------------------------

	Order::DRINK drink_;
	Order::SWEETS sweets_;

	virtual void UpdatePlaced(void);
	virtual void UpdateHold(void);
	
	virtual void UpdateInActive(void);
	virtual void UpdateActive(void);

	std::string objId_;
	std::pair<std::string, StageObjectLibrary::ObjectParams> object_;

private:

	ITEM_STATE itemState_;
	MACHINE_STATE machineState_;

	void DrawDebug(void);
};