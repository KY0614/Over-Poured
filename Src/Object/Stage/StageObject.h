#pragma once
#include <string>
#include <functional>
#include "../ActorBase.h"
#include "StageObjectLibrary.h"

class Cube;
class Player;

class StageObject : public ActorBase
{
public:

	enum class ITEM_STATE {
		NONE,
		PLACED,
		HOLD,
	};

	StageObject(const std::string objId,const float width,
		const float height, const float depth,Player& player);

	~StageObject(void);

	virtual void Init(void)override;
	virtual void Update(void)override;
	virtual void Draw(void)override;

	void SetPos(VECTOR pos);

	void SetFollowPos(VECTOR followPos) { followPos_ = followPos; }

	void ChangeItemState(ITEM_STATE state) { state_ = state; }

	VECTOR GetPos(void)const { return transform_.pos; };
	VECTOR GetSpherePos(void)const { return sphereTran_.pos; };

	/// <summary>
	/// �����^�щ\�̃I�u�W�F�N�g���ǂ���
	/// </summary>
	/// <param name=""></param>
	/// <returns>true:�����^�щ\�Afalse:�s�\</returns>
	bool IsCarryable(void)const { return param_.carryable_; }

	std::string GetObjectId(void)const { return objId_; }

	ITEM_STATE GetItemState(void)const { return state_; }

	virtual void Interact(void) = 0;

protected:

	Player& player_;

	VECTOR followPos_;

	virtual void UpdatePlaced(void);
	virtual void UpdateHold(void);

private:
	std::string objId_;
	StageObjectLibrary::ObjectParams param_;
	std::pair<std::string, StageObjectLibrary::ObjectParams> object_;

	ITEM_STATE state_;

	//�����f���̗�����
	std::unique_ptr<Cube> cube_;
	float width_;
	float height_;
	float depth_;

	float fillProgress_ = 0.0f;

	/// <summary>
	/// ���W��g�嗦�𒲐�����p��GUI
	/// </summary>
	/// <param name=""></param>
	void UpdateDebugImGui(void);
};

