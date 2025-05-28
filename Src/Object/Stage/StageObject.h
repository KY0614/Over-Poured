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

	enum class ITEM_STATE {
		NONE,
		PLACED,
		HOLD,
	};

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

	void SetPos(VECTOR pos);

	void SetFollowPos(VECTOR followPos) { followPos_ = followPos; }

	void ChangeItemState(ITEM_STATE state) { itemState_ = state; }
	void ChangeMachineState(MACHINE_STATE state) { machineState_ = state; }

	VECTOR GetPos(void)const { return transform_.pos; };
	VECTOR GetSpherePos(void)const { return sphereTran_.pos; };

	/// <summary>
	/// �����^�щ\�̃I�u�W�F�N�g���ǂ���
	/// </summary>
	/// <param name=""></param>
	/// <returns>true:�����^�щ\�Afalse:�s�\</returns>
	bool IsCarryable(void)const { return param_.carryable_; }

	/// <summary>
	/// �ݒu�\�I�u�W�F�N�g���ǂ���
	/// </summary>
	/// <param name=""></param>
	/// <returns>true:�ݒu�\�Afalse:�s�\</returns>
	bool IsPlaceable(void)const { return param_.placeable_; }

	/// <summary>
	/// �v���C���[���A�N�V�����\�ȃI�u�W�F�N�g���ǂ���
	/// </summary>
	/// <param name=""></param>
	/// <returns>true:�A�N�V�����\�Afalse:�s�\</returns>
	bool IsInteractable(void)const { return param_.interactable_; }

	std::string GetInteractType(void)const { return param_.interactType_; }

	float GetInteractTime(void)const { return param_.interactTime; }

	std::string GetObjectId(void)const { return param_.id_; }

	ITEM_STATE GetItemState(void)const { return itemState_; }
	MACHINE_STATE GetMachineState(void)const { return machineState_; }

	virtual void ItemCarry(void);

	virtual void ItemPlaced(VECTOR pos);

	virtual void Interact(const std::string& objId, std::vector<std::unique_ptr<StageObject>>& object);

	virtual void PickUp(std::vector<std::unique_ptr<StageObject>>& object);

	VECTOR GetTopCenter(void)const;

	float GetSphereRad(void)const;

	bool IsActioned(void) const;

protected:

	//�I�u�W�F�N�g�̃p�����[�^
	StageObjectLibrary::ObjectParams param_;

	Player& player_;

	VECTOR followPos_;

	//�����f���̗�����
	std::unique_ptr<Cube> cube_;
	float width_;
	float height_;
	float depth_;

	//�����蔻��p����
	std::unique_ptr<Sphere> sphere_;

	float rad_;

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

	float fillProgress_ = 0.0f;

	/// <summary>
	/// ���W��g�嗦�𒲐�����p��GUI
	/// </summary>
	/// <param name=""></param>
	void UpdateDebugImGui(void);
};

