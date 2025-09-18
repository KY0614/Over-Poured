#include "../Libs/ImGui/imgui.h"
#include "../../Common/DebugDrawFormat.h"
#include "../Manager/Generic/ResourceManager.h"
#include "../Common/Sphere.h"
#include "../Common/Cube.h"
#include "../../Utility/AsoUtility.h"
#include "../../Utility/StringUtility.h"
#include "StageObjectLibrary.h"
#include "StageObject.h"

StageObject::StageObject(const std::string objId,
	const float height, Player& player):
	objId_(objId),height_(height),player_(player)
{
	isActioned_ = false;
	itemState_ = ITEM_STATE::NONE;
	machineState_ = MACHINE_STATE::NONE;
	param_ = StageObjectLibrary::ObjectParams();
	isLid_ = false;
	hasStock_ = false;
}

StageObject::~StageObject(void)
{
}

void StageObject::Init(VECTOR pos,float rotY, VECTOR scale)
{
	//作成するオブジェクトのパラメータをjsonファイルから読み込む
	param_ = StageObjectLibrary::LoadData(objId_).second;

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

	//当たり判定用の球を作成
	sphere_ = std::make_unique<Sphere>(transform_);
	//オブジェクトからの相対座標を設定
	sphere_->SetLocalPos(AsoUtility::VECTOR_ZERO);
	//半径を設定
	sphere_->SetRadius(param_.collisionRadius_);

	//テーブルとカウンターの当たり判定用の球体の高さを高めにしておく（テーブルから半分球体が出るくらい）
	if (objId_ == TABLE || objId_ == COUNTER)sphere_->SetLocalPos({ 0.0f, height_, 0.0f });
	//ゴミ箱も同様
	if (objId_ == DUST_BOX)sphere_->SetLocalPos({ 0.0f, height_ + sphere_->GetRadius(), 0.0f });

	//アイテムの初期状態は設置状態
	ChangeItemState(ITEM_STATE::PLACED);

	//マシンの初期状態は非稼働状態
	ChangeMachineState(MACHINE_STATE::INACTIVE);

	//テーブルじゃなかったらモデルを読み込まない
	if (objId_ != TABLE)return;
	InitTableColliderModel();
}

void StageObject::Update(void)
{
	isActioned_ = false;

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
	tableColliderTran_.Update();
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

VECTOR StageObject::GetSpherePos(void) const
{
	return sphere_->GetPos();
}

VECTOR StageObject::GetTopCenter(void) const
{
	VECTOR center = transform_.pos;
	center.y = height_;

	return sphere_->GetPos();
	//return center;
}

float StageObject::GetSphereRad(void) const
{
	return sphere_->GetRadius();
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

void StageObject::AddStock(void)
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

void StageObject::InitTableColliderModel(void)
{
	//モデルの基本設定（テーブルのコライダー用モデル)
	tableColliderTran_.SetModel(ResourceManager::GetInstance().LoadModelDuplicate(
		ResourceManager::SRC::TABLE_COL));
	tableColliderTran_.scl = transform_.scl;
	tableColliderTran_.pos = transform_.pos;
	tableColliderTran_.quaRot = transform_.quaRot;
	tableColliderTran_.quaRotLocal = transform_.quaRotLocal;
	tableColliderTran_.MakeCollider(Collider::TYPE::STAGE);
	tableColliderTran_.Update();
}