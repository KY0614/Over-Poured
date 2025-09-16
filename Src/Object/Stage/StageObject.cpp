#include "../Libs/ImGui/imgui.h"
#include "../../Common/DebugDrawFormat.h"
#include "../Manager/Generic/ResourceManager.h"
#include "../Common/Sphere.h"
#include "../Common/Cube.h"
#include "../../Utility/AsoUtility.h"
#include "../../Utility/StringUtility.h"
#include "StageObjectLibrary.h"
#include "StageObject.h"

StageObject::StageObject(const std::string objId, const float width,
	const float height, const float depth,Player& player):
	objId_(objId),width_(width),height_(height),
	depth_(depth),player_(player)
{
	isActioned_ = false;
	itemState_ = ITEM_STATE::NONE;
	machineState_ = MACHINE_STATE::NONE;
	param_ = StageObjectLibrary::ObjectParams();
	collisionRad_ = 0.0f;
	isLid_ = false;
	drink_ = Order::DRINK::NONE;
	sweets_ = Order::SWEETS::NONE;

	hasStock_ = false;
	interactImg_ = -1;
	isInteract_ = false;
}

StageObject::~StageObject(void)
{
}

void StageObject::Init(VECTOR pos,float rotY, VECTOR scale)
{
	//作成するオブジェクトのパラメータをjsonファイルから読み込む
	object_ = StageObjectLibrary::LoadData(objId_);
	param_ = object_.second;

	//文字列をSRCに変換してモデル設定
	ResourceManager::SRC srcType = ResourceManager::GetInstance().StringToSRC(param_.id_);

	//モデルの基本設定
	transform_.SetModel(ResourceManager::GetInstance().LoadModelDuplicate(srcType));
	transform_.scl = scale;
	transform_.pos = pos;
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal =
		Quaternion::Euler({ 0.0f, AsoUtility::Deg2RadF(rotY), 0.0f });
	transform_.MakeCollider(Collider::TYPE::STAGE);
	transform_.Update();

	//当たり判定の半径を設定
	collisionRad_ = param_.collisionRadius_;

	if (objId_ == HOT_COFFEE_ID)drink_ = Order::DRINK::HOT;
	if (objId_ == ICE_COFFEE_ID)drink_ = Order::DRINK::ICE;

	sphere_ = std::make_unique<Sphere>(transform_);
	sphere_->SetLocalPos(AsoUtility::VECTOR_ZERO);
	sphere_->SetRadius(collisionRad_);

	if (objId_ == "Table" || objId_ == "Counter")sphere_->SetLocalPos({ 0.0f, height_, 0.0f });
	if (objId_ == "Dust_Box")sphere_->SetLocalPos({ 0.0f, height_ + sphere_->GetRadius(), 0.0f });

	itemState_ = ITEM_STATE::PLACED;
	machineState_ = MACHINE_STATE::INACTIVE;

	if (objId_ != "Table")return;

	//モデルの基本設定（テーブルのコライダー用モデル)
	colTran_.SetModel(ResourceManager::GetInstance().LoadModelDuplicate(
	ResourceManager::SRC::TABLE_COL));
	colTran_.scl = transform_.scl;
	colTran_.pos = transform_.pos;
	colTran_.quaRot = transform_.quaRot;
	colTran_.quaRotLocal = transform_.quaRotLocal;
	colTran_.MakeCollider(Collider::TYPE::STAGE);
	colTran_.Update();
}

void StageObject::Update(void)
{
	isActioned_ = false;
	isInteract_ = false;

	switch (itemState_)
	{
	case StageObject::ITEM_STATE::NONE:
		break;

	case StageObject::ITEM_STATE::PLACED:
		UpdatePlaced();
		break;

	case StageObject::ITEM_STATE::HOLD:
		UpdateHold();
		break;

	default:
		break;
	}

	switch (machineState_)
	{
	case StageObject::MACHINE_STATE::NONE:
		break;

	case StageObject::MACHINE_STATE::INACTIVE:
		UpdateInActive();
		break;

	case StageObject::MACHINE_STATE::ACTIVE:
		UpdateActive();
		break;

	default:
		break;
	}
	colTran_.Update();
	transform_.Update();

}

void StageObject::Draw(void)
{
	//モデルの描画
	MV1DrawModel(transform_.modelId);
}

void StageObject::SetPos(VECTOR pos)
{
	transform_.pos = pos;
}

void StageObject::SetScale(VECTOR scale)
{
	transform_.scl = scale;
}

void StageObject::SetQuaRotY(const float localRotY)
{
	transform_.quaRot = Quaternion::Euler({ 0.0f, AsoUtility::Deg2RadF(localRotY), 0.0f });
}

VECTOR StageObject::GetSpherePos(void) const
{
	return sphere_->GetPos();
}

VECTOR StageObject::GetTopCenter(void) const
{
	VECTOR center = transform_.pos;
	center.y = height_;
	return center;
}

float StageObject::GetSphereRad(void) const
{
	return sphere_->GetRadius();
}

bool StageObject::IsActioned(void) const
{
	return isActioned_;
}

void StageObject::ItemCarry(void)
{
}

void StageObject::ItemPlaced(VECTOR pos)
{
}

void StageObject::Interact(const std::string& objId)
{
}

void StageObject::PickUp(std::string rackName,std::vector<std::unique_ptr<StageObject>>& object)
{
}

void StageObject::AddStock(int addStockNum)
{
}

void StageObject::UpdatePlaced(void)
{
}

void StageObject::UpdateHold(void)
{
}

void StageObject::UpdateInActive(void)
{
}

void StageObject::UpdateActive(void)
{
}