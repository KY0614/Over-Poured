#pragma once
#include <string>
#include <functional>
#include "../ActorBase.h"
#include "StageObjectLibrary.h"

class Cube;
class Sphere;
class Player;

class StageObject : public ActorBase
{
public:

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

	virtual void Init(void)override;
	virtual void Update(void)override;
	virtual void Draw(void)override;

	//�ݒ�p�A�ύX�p�֐�-------------------------------------------------------------

	/// <summary>
	/// ���W��ݒ肷��
	/// </summary>
	/// <param name="pos">�ݒ���W</param>
	void SetPos(VECTOR pos);
	void SetFollowPos(void) { transform_.pos = GetRotPos(follow_.localPos); }

	void SetFollowPos(VECTOR pos) { follow_.pos = pos; }
	void SetFollowLocalPos(VECTOR localPos) { follow_.localPos = localPos; }

	void SetFollow(const Transform& parent) { follow_ = parent; }

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
	/// �����^�щ\�̃I�u�W�F�N�g���ǂ���
	/// </summary>
	/// <param name=""></param>
	/// <returns>true:�����^�щ\�Afalse:�s�\</returns>
	bool GetIsCarryable(void)const { return param_.carryable_; }

	/// <summary>
	/// �ݒu�\�I�u�W�F�N�g���ǂ���
	/// </summary>
	/// <param name=""></param>
	/// <returns>true:�ݒu�\�Afalse:�s�\</returns>
	bool GetIsPlaceable(void)const { return param_.placeable_; }

	/// <summary>
	/// �v���C���[���A�N�V�����\�ȃI�u�W�F�N�g���ǂ���
	/// </summary>
	/// <param name=""></param>
	/// <returns>true:�A�N�V�����\�Afalse:�s�\</returns>
	bool GetIsInteractable(void)const { return param_.interactable_; }

	/// <summary>
	/// �C���^���N�g�^�C�v�i����^�C�v�j���擾����
	/// </summary>
	/// <param name="">�I�u�W�F�N�g�̃p�����[�^</param>
	/// <returns>�C���^���N�g�^�C�v</returns>
	std::string GetInteractType(void)const { return param_.interactType_; }

	/// <summary>
	/// �C���^���N�g���Ԃ��擾����
	/// </summary>
	/// <param name="">�I�u�W�F�N�g�̃p�����[�^</param>
	/// <returns>�C���^���N�g����</returns>
	float GetInteractTime(void)const { return param_.interactTime; }

	/// <summary>
	/// �I�u�W�F�N�g��ID���擾����
	/// </summary>
	/// <param name="">�I�u�W�F�N�g�̃p�����[�^</param>
	/// <returns>ID</returns>
	std::string GetObjectId(void)const { return param_.id_; }

	std::string GetObjCategory(void)const { return param_.category_; }

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
	virtual void PickUp(std::vector<std::unique_ptr<StageObject>>& object);

protected:

	//�I�u�W�F�N�g�̃p�����[�^
	StageObjectLibrary::ObjectParams param_;

	Player& player_;

	Transform follow_;

	//�����f���̗�����
	std::unique_ptr<Cube> cube_;
	float width_;	//����
	float height_;	//����
	float depth_;	//���s

	//�����蔻��p����
	std::unique_ptr<Sphere> sphere_;

	float rad_;		//���a

	//
	bool isActioned_;	

	virtual void UpdatePlaced(void);
	virtual void UpdateHold(void);
	
	virtual void UpdateInActive(void);
	virtual void UpdateActive(void);

private:
	std::string objId_;

	std::pair<std::string, StageObjectLibrary::ObjectParams> object_;

	ITEM_STATE itemState_;
	MACHINE_STATE machineState_;

	//���΍��W����]�����ă��[���h���W�Ŏ擾����
	VECTOR GetRotPos(const VECTOR& localPos) const;

	/// <summary>
	/// ���W��g�嗦�𒲐�����p��GUI
	/// </summary>
	/// <param name=""></param>
	void UpdateDebugImGui(void);
};

