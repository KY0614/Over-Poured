#include <DxLib.h>
#include "../../Application.h"
#include "../Utility/CommonUtility.h"
#include "../../Manager/GameSystem/SoundManager.h"
#include "../../Manager/Generic/ResourceManager.h"
#include "../../Manager/Generic/InputManager.h"
#include "../Player.h"
#include "../Common/Sphere.h"
#include "../Common/AnimationController.h"
#include "StageObject/Furnitures.h"
#include "StageObject.h"
#include "StageObject/ItemObject.h"
#include "StageObject/RackObject.h"
#include "StageObject/FollowingObject.h"
#include "StageObject/Table.h"
#include "StageObject/Machine.h"
#include "StageObject/CupLidRack.h"
#include "StageObject/DustBox.h"
#include "StageObject/IceDispenser.h"
#include "StageManager.h"

namespace
{
	//アニメーションの再生速度
	const float ANIM_SPEED = 30.0f; 

	//テーブル関連
	const int TABLE_COLUMN_NUM = 4;		//横側のテーブルの数（１列）
	const int TABLE_ROW_BACK_NUM = 4;	//手前側のテーブルの数（１列）
	const int TABLE_ROW_FRONT_NUM = 2;	//奥側のテーブルの数（ドーナツのラックを載せる用なので表示はしない）
	const int TABLE_CENTER_NUM = 4;		//中央テーブルの数
	const int MAX_TABLE_NUM = 16;		//テーブルの最大数
	const float TABLE_WIDTH = 92.5f;	//テーブルの横幅

	//座標
	const VECTOR TABLE_POS_BACK = { -150.0f, 0.0f, -270.0f };	//テーブルの座標(手前側）
	const VECTOR TABLE_POS_FRONT = { -115.0f, 0.0f, 180.0f };	//テーブルの座標（奥側）
	const VECTOR COLUMN_TABLE_LEFT_POS = { -250.0f, 0.0f, -192.0f };//列（左）テーブルの座標
	const VECTOR COLUMN_TABLE_RIGHT_POS = { 320.0f, 0.0f, -192.0f };//列(右）テーブルの座標
	const VECTOR CENTER_TABLE_POS = { -20.0f,0.0f,-100.0f };		//列(右）テーブルの座標
	const VECTOR COUNTER_POS = { 225.0f, 0.0f, 190.0f };			//カウンターの座標
	const VECTOR CASE_POS = { -57.0f, 0.0f, 190.0f };				//ショーケースの座標
	const VECTOR MACHINE_POS = { -128.0f, 76.0f, -175.0f };			//コーヒーマシンの座標
	const VECTOR CUPHOT_POS = { -45.0f, 76.0f, -175.0f };			//ホット用カップの座標
	const VECTOR CUPICE_POS = { 45.0f, 76.0f, -175.0f };			//アイス用カップの座標
	const VECTOR ICEDIS_POS = { 133.0f, 76.0f, -175.0f };			//アイスディスペンサーの座標
	const VECTOR DUSTBOX_POS = { 240.0f, 0.0f, -270.0f };			//ゴミ箱の座標

	const int SOUND_VOLUME = 256; // 効果音の音量(0~256)
}

StageManager::StageManager(Player& player):player_(player)
{
	servedItems_ = {};
	currentOrder_ = {};

	objects_.clear();
	tables_.clear();
	counter_ = nullptr;

	isServed_ = false;
	isServedItems_.clear();

	animationController_ = nullptr;
}

StageManager::~StageManager(void)
{
}

void StageManager::Init(void)
{
	//サウンドの初期化
	InitSound();	

	//3Dモデルの初期化
	Init3DModel();
}

void StageManager::Update(void)
{
	//アニメーションの更新
	animationController_->Update();

	if (animationController_->IsEnd())
	{
		//アニメーションが終わったらマシンモードに切り替え
		animationController_->Play((int)ANIM_TYPE::IDLE);
	}

	//ステージオブジェクトの更新
	for (const auto& obj : objects_)
	{
		obj->Update();
	}
	//テーブルの更新
	for (const auto& obj : tables_)
	{
		obj->Update();
	}
	//カウンターのモデル情報更新
	counter_->Update();
	//家具の更新
	furnitures_->Update();

	//ラックからカップを取り出す処理
	CupRackInteract();

	//持ち運び可能なオブジェクトのインタラクト処理
	CarryableObjInteract();

	//マシンとのインタラクト処理
	MachineInteract();

	//蓋ラックとのインタラクト処理
	LidRackInteract();

	//ゴミ箱とのインタラクト処理
	DustBoxInteract();

	//3Dモデルの更新
	registerTran_.Update();
	caseTran_.Update();
}

void StageManager::Draw(void)
{
	//3Dモデルの描画
	//レジとショーケースのモデル
	MV1DrawModel(registerTran_.modelId);
	MV1DrawModel(caseTran_.modelId);

	//家具の描画
	furnitures_->Draw();

	//テーブルの描画
	for (const auto& table : tables_)
	{
		table->Draw();
	}

	//カウンターの描画
	counter_->Draw();

	//ステージオブジェクトの描画
	for (const auto& obj : objects_)
	{
		obj->Draw();
	}
}

void StageManager::SetCurrentOrder(const Order::OrderData& order)
{
	//注文数文の配列を確保し、初期化＆お客の注文内容を受け取る
	size_t size = static_cast<size_t>(order.orderNum_);
	isServedItems_.resize(size);
	for(bool isSuved : isServedItems_)
	{
		isSuved = false; // 初期化
	}
	currentOrder_ = order;
}

void StageManager::ResetServeData(void)
{
	//提供データをリセット
	isServed_ = false;
	servedItems_.drink_ = Order::DRINK::NONE;
	servedItems_.sweets_ = Order::SWEETS::NONE;
	servedItems_.lid_ = false;
	isServedItems_.clear();
	isServedItems_.resize(0);
}

const Transform& StageManager::GetCounterTran(void) const
{
	return counter_->GetTransform(); 
}

const Transform& StageManager::GetTableTran(int index) const
{
	//範囲外のインデックスは無視
	if (index > static_cast<int>(tables_.size()))return Transform(); 
	return tables_[index]->GetTalbeColTran();
}

const Transform& StageManager::GetShowCase(void) const
{
	return caseTran_;
}

const Transform& StageManager::GetDustBox(void) const
{
	return dustBoxTran_;
}

const Transform& StageManager::GetFloorTran(void) const
{
	return furnitures_->GetFloorTran();
}

void StageManager::Init3DModel(void)
{
	//モデル制御の基本情報
	//ショーケースのモデル
	caseTran_.SetModel(
		ResourceManager::GetInstance().LoadModelDuplicate(
			ResourceManager::SRC::SWEETS_CASE));
	caseTran_.scl = CommonUtility::VECTOR_ONE;
	caseTran_.pos = CASE_POS;
	caseTran_.quaRot = Quaternion();
	caseTran_.quaRotLocal = Quaternion();
	caseTran_.MakeCollider(Collider::TYPE::STAGE);
	caseTran_.Update();

	//レジのモデル
	registerTran_.SetModel(
		ResourceManager::GetInstance().LoadModelDuplicate(
			ResourceManager::SRC::REGISTER));
	registerTran_.scl = CommonUtility::VECTOR_ONE;
	registerTran_.pos = CommonUtility::VECTOR_ZERO;
	registerTran_.quaRot = Quaternion::Euler(CommonUtility::VECTOR_ZERO);
	registerTran_.quaRotLocal = Quaternion();
	registerTran_.Update();

	//アニメーションの初期化
	InitAnimation();

	//家具の初期化
	furnitures_ = std::make_unique<Furnitures>();
	furnitures_->Init();

	//初期化でしか使わない定数
	//オブジェクトの向き(Y軸回転）
	const float rot_R = -90.0f;	//Y軸-90度回転
	const float rot_L = 90.0f;	//Y軸90度回転
	const float rot_B = 180.0f;	//Y軸180度回転

	VECTOR objectPos = {};
	float rotY = 0.0f;

	//横のテーブル群(手前)
	for (int x = 0; x < TABLE_ROW_BACK_NUM; x++)
	{
		objectPos = TABLE_POS_BACK;
		objectPos.x += (x * TABLE_WIDTH);
		tables_.emplace_back(std::make_unique<Table>(StageObject::TABLE, player_, objects_));
		tables_.back()->Init(objectPos, rot_B);
	}

	//縦のテーブル群(左側）
	for (int z = TABLE_ROW_BACK_NUM; z < TABLE_ROW_BACK_NUM + TABLE_COLUMN_NUM; z++)
	{
		objectPos = COLUMN_TABLE_LEFT_POS;
		objectPos.z += ((z - TABLE_ROW_BACK_NUM) * TABLE_WIDTH);
		tables_.emplace_back(std::make_unique<Table>(StageObject::TABLE, player_, objects_));
		tables_.back()->Init(objectPos, rot_R);
	}

	//縦のテーブル群(右側）
	for (int z = TABLE_ROW_BACK_NUM; z < TABLE_ROW_BACK_NUM + TABLE_COLUMN_NUM; z++)
	{
		objectPos = COLUMN_TABLE_RIGHT_POS;
		objectPos.z += ((z - TABLE_ROW_BACK_NUM) * TABLE_WIDTH);
		tables_.emplace_back(std::make_unique<Table>(StageObject::TABLE, player_, objects_));
		tables_.back()->Init(objectPos, rot_L);
	}
	//テーブルのX方向のオフセット値
	const float tableOffsetX = 90.0f;
	//真ん中のテーブル群（２個ずつ生成）
	for (int i = 0; i < TABLE_CENTER_NUM / 2; ++i)
	{
		objectPos = CENTER_TABLE_POS;
		objectPos.x += i * tableOffsetX;
		//奥側のテーブル２つ
		tables_.emplace_back(std::make_unique<Table>(StageObject::TABLE, player_, objects_));
		tables_.back()->Init(objectPos);
	}

	//テーブルのZ方向のオフセット値
	const float tableOffsetZ = 60.0f;
	for (int i = 0; i < TABLE_CENTER_NUM / 2; ++i)
	{
		//オブジェクトの位置を初期化し、オフセット値を加算
		objectPos = CENTER_TABLE_POS;
		objectPos.x += i * tableOffsetX;
		objectPos.z += tableOffsetZ;
		//手前側のテーブル２つ
		tables_.emplace_back(std::make_unique<Table>(StageObject::TABLE, player_, objects_));
		tables_.back()->Init(objectPos, rot_B);
	}
	const float caseOffsetX = 20.0f;
	//ケースのテーブル群(奥側)
	for (int x = 0; x < TABLE_ROW_FRONT_NUM; x++)
	{
		objectPos = TABLE_POS_FRONT;
		objectPos.x += (x * (TABLE_WIDTH + caseOffsetX));
		tables_.emplace_back(std::make_unique<Table>(StageObject::TABLE, player_, objects_));
		tables_.back()->Init(objectPos);
	}

	//カウンター用テーブル
	counter_ = std::make_unique<Table>(StageObject::COUNTER, player_, objects_);
	counter_->Init(COUNTER_POS);

	VECTOR pos = tables_[TABLE_ROW_BACK_NUM + TABLE_COLUMN_NUM - 1]->GetSpherePos();
	//ホット用カップのラック
	objects_.emplace_back(std::make_unique<RackObject>(StageObject::HOT_CUP_RACK, player_));
	objects_.back()->Init(pos, rot_R);

	//アイス用カップのラック
	pos = tables_[TABLE_ROW_BACK_NUM + TABLE_COLUMN_NUM + TABLE_COLUMN_NUM - 1]->GetSpherePos();
	objects_.emplace_back(std::make_unique<RackObject>(StageObject::ICE_CUP_RACK, player_));
	objects_.back()->Init(pos, rot_L);

	//チョコスイーツ用のラック
	pos = tables_[tables_.size() - 2]->GetSpherePos();
	objects_.emplace_back(std::make_unique<RackObject>(StageObject::CHOCO_SWEETS_RACK, player_));
	objects_.back()->Init(pos);

	//ベリースイーツ用のラック
	pos = tables_.back()->GetSpherePos();
	objects_.emplace_back(std::make_unique<RackObject>(StageObject::BERRY_SWEETS_RACK, player_));
	objects_.back()->Init(pos);

	//カップ用の蓋
	pos = tables_[2]->GetSpherePos();
	objects_.emplace_back(std::make_unique<CupLidRack>(StageObject::CUP_LID_RACK, player_, objects_));
	objects_.back()->Init(pos);

	//コーヒーマシン
	int machineTableIdx = MAX_TABLE_NUM - 2;	//コーヒーマシンを置くテーブルのインデックス
	pos = tables_[machineTableIdx]->GetSpherePos();
	objects_.emplace_back(std::make_unique<Machine>(StageObject::COFFEE_MACHINE, player_, objects_));
	objects_.back()->Init(pos, rot_L);

	//コーヒーマシン２個目
	machineTableIdx = MAX_TABLE_NUM - 4;	//コーヒーマシンを置くテーブルのインデックス
	pos = tables_[machineTableIdx]->GetSpherePos();
	objects_.emplace_back(std::make_unique<Machine>(StageObject::COFFEE_MACHINE,player_, objects_));
	objects_.back()->Init(pos, rot_L);

	//アイスディスペンサー
	machineTableIdx = MAX_TABLE_NUM - 1;	//コーヒーマシンを置くテーブルのインデックス
	pos = tables_[machineTableIdx]->GetSpherePos();
	objects_.emplace_back(std::make_unique<IceDispenser>(StageObject::ICE_DISPENSER,player_, objects_));
	objects_.back()->Init(pos, rot_R);

	//アイスディスペンサー２個目
	machineTableIdx = MAX_TABLE_NUM - 3;	//コーヒーマシンを置くテーブルのインデックス
	pos = tables_[machineTableIdx]->GetSpherePos();
	objects_.emplace_back(std::make_unique<IceDispenser>(StageObject::ICE_DISPENSER,player_, objects_));
	objects_.back()->Init(pos, rot_R);

	//ゴミ箱の大きさ（y軸方向を少し潰す）
	const VECTOR dustBoxScale = { 1.0f,0.8f,1.0f };
	//ゴミ箱
	objects_.emplace_back(std::make_unique<DustBox>(StageObject::DUST_BOX,player_, objects_));
	objects_.back()->Init(DUSTBOX_POS, rot_B, dustBoxScale);
	dustBoxTran_ = objects_.back()->GetTransform(); // ゴミ箱のTransformを保存
}

void StageManager::InitSound(void)
{
	auto& sound = SoundManager::GetInstance();

	//se追加
	//オブジェクトを持つSE
	sound.Add(SoundManager::TYPE::SE, SoundManager::SOUND::PICK_UP,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::PICK_UP).handleId_);
	sound.AdjustVolume(SoundManager::SOUND::PICK_UP, SOUND_VOLUME);
	//オブジェクトを置くSE
	sound.Add(SoundManager::TYPE::SE, SoundManager::SOUND::PUT_ON,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::PUT_ON).handleId_);
	sound.AdjustVolume(SoundManager::SOUND::PUT_ON, SOUND_VOLUME);
	//在庫を補充するSE
	sound.Add(SoundManager::TYPE::SE, SoundManager::SOUND::ADD_STOCK,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::ADD_STOCK).handleId_);
	sound.AdjustVolume(SoundManager::SOUND::ADD_STOCK, SOUND_VOLUME);
	//提供時のSE
	sound.Add(SoundManager::TYPE::SE, SoundManager::SOUND::PAYING,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::PAYING).handleId_);
	sound.AdjustVolume(SoundManager::SOUND::PAYING, SOUND_VOLUME);
}

void StageManager::InitAnimation(void)
{
	std::string path = Application::PATH_MODEL + "Stage/Animation/";
	animationController_ = std::make_unique<AnimationController>(registerTran_.modelId);
	//アニメーションの追加
	animationController_->Add((int)ANIM_TYPE::CREATE, path + "create_register.mv1", ANIM_SPEED);
	animationController_->Add((int)ANIM_TYPE::PAYING, path + "paying.mv1", ANIM_SPEED);	
	animationController_->Play((int)ANIM_TYPE::IDLE);
}

void StageManager::SurveItem(StageObject& obj)
{
	//objのIDや状態からOrderDataをセット
	if (obj.GetParam().id_ == StageObject::HOT_COFFEE)
	{
		servedItems_.drink_ = Order::DRINK::HOT;
		servedItems_.lid_ = obj.IsLidOn();
		isServedItems_.front() = true;
	}
	else if (obj.GetParam().id_ == StageObject::ICE_COFFEE)
	{
		servedItems_.drink_ = Order::DRINK::ICE;
		servedItems_.lid_ = obj.IsLidOn();
		isServedItems_.front() = true;
	}

	if (obj.GetParam().id_ == StageObject::CHOCO_SWEETS)
	{
		servedItems_.sweets_ = Order::SWEETS::CHOCO;
		isServedItems_.back() = true;
	}
	else if (obj.GetParam().id_ == StageObject::BERRY_SWEETS)
	{
		servedItems_.sweets_ = Order::SWEETS::STRAWBERRY;
		isServedItems_.back() = true;
	}
	//提供後はオブジェクトを削除
	DeleteSurvedItem(); 

	auto& sound = SoundManager::GetInstance();
	// 注文が揃ったか判定
	if (IsOrderCompleted())
	{
		isServed_ = true;
		animationController_->Play((int)ANIM_TYPE::PAYING, false);
		sound.Play(SoundManager::SOUND::PAYING);
	}
}

void StageManager::DeleteSurvedItem(void)
{
	//カウンターの球体と当たっているオブジェクトを削除
	for (auto it = objects_.begin(); it != objects_.end(); )
	{
		if (CommonUtility::IsHitSpheres(
			(*it)->GetSpherePos(), (*it)->GetSphereRad(),
			counter_->GetSpherePos(), counter_->GetSphereRad()))
		{
			StageObject* target = it->get(); //今から削除する親オブジェクトを記録
			if (target->IsLidOn())
			{
				//追従している蓋をすべて削除
				for (int i = 0; i < objects_.size(); )
				{
					FollowingObject* follower = dynamic_cast<FollowingObject*>(objects_[i].get());
					if (follower && &(follower->GetFollowedObj()) == target)
					{
						objects_.erase(objects_.begin() + i);
						continue;
					}
					++i;
				}
			}
			it = objects_.erase(it);

		}
		else
		{
			++it;
		}
	}
}

void StageManager::CupRackInteract(void)
{
	auto& pSphere = player_.GetSphere();

	//ラックからカップを取り出す処理
	for (const auto& obj : objects_)
	{
		//ラック以外のオブジェクトは判定しない
		if (obj->GetParam().id_ != StageObject::HOT_CUP_RACK &&
			obj->GetParam().id_ != StageObject::ICE_CUP_RACK &&
			obj->GetParam().id_ != StageObject::BERRY_SWEETS_RACK &&
			obj->GetParam().id_ != StageObject::CHOCO_SWEETS_RACK) continue;
		//ラックに在庫がないときの処理
		if (!player_.GetIsHolding() && obj->GetParam().interactable_ &&
			!obj->GetHasStock() &&
			CommonUtility::IsHitSpheres(pSphere.GetPos(), pSphere.GetRadius(),
				obj->GetSpherePos(), obj->GetSphereRad()))
		{
			//在庫を補充する処理
			obj->AddStock();
			break;
		}

		//プレイヤーが何も持っていないときの処理
		if (!player_.GetIsHolding() && obj->GetParam().interactable_ &&
			CommonUtility::IsHitSpheres(pSphere.GetPos(), pSphere.GetRadius(),
				obj->GetSpherePos(), obj->GetSphereRad()))
		{
			//在庫があるときはカップを取り出す処理
			obj->PickUp(obj->GetParam().id_, objects_);
			break;
		}
	}

}

void StageManager::CarryableObjInteract(void)
{
	auto& pSphere = player_.GetSphere();
	for (const auto& obj : objects_)
	{
		//カウンターで商品を提供する処理
		if (CommonUtility::IsHitSpheres(obj->GetSpherePos(), obj->GetSphereRad(),
			counter_->GetSpherePos(), counter_->GetSphereRad()
		))
		{
			//設置されているアイテムを提供する処理
			if (obj->GetItemState() == StageObject::ITEM_STATE::PLACED)
			{
				SurveItem(*obj);
				break;
			}
		}

		//プレイヤーが何も持っていないときの処理
		if (!player_.GetIsHolding() && obj->GetParam().carryable_ &&
			CommonUtility::IsHitSpheres(pSphere.GetPos(), pSphere.GetRadius(),
				obj->GetSpherePos(), obj->GetSphereRad()))
		{
			obj->ItemCarry();
			break;
		}

		//プレイヤーがアイテムを持っているときの処理
		if (player_.GetIsHolding())
		{
			//カウンターに設置
			if (CommonUtility::IsHitSpheres(obj->GetSpherePos(), obj->GetSphereRad(),
				counter_->GetSpherePos(), counter_->GetSphereRad()
			))
			{
				auto items = counter_->GetParam().acceptedItems_;
				//objIdがインタラクト対象物に存在するかどうか
				bool isAccepted = std::find(items.begin(), items.end(), obj->GetParam().id_) != items.end();
				if (!isAccepted)continue;	//存在しなかったら処理しない
				//アイテムを設置する処理
				obj->ItemPlaced(counter_->GetSpherePos());
			}

			for (const auto& table : tables_)
			{
				//設置可能なテーブルの上にアイテムを設置する処理
				if (table->GetParam().placeable_ &&
					CommonUtility::IsHitSpheres(pSphere.GetPos(), pSphere.GetRadius(),
						table->GetSpherePos(), table->GetSphereRad()
					))
				{
					//アイテムを設置する処理
					obj->ItemPlaced(table->GetSpherePos());
				}
				//既にアクションを行っていたらループを抜ける
				if (obj->IsActioned())
				{
					break;
				}
			}
		}
		//既にアクションを行っていたらループを抜ける
		if (obj->IsActioned())
		{
			break;
		}
	}
}

void StageManager::MachineInteract(void)
{
	auto& pSphere = player_.GetSphere();

	//マシンとカップの処理
	for (int i = 0; i < static_cast<int>(objects_.size()); ++i)
	{
		//コーヒーマシンの判定だけさせたい
		if (objects_[i]->GetParam().id_ != StageObject::COFFEE_MACHINE)continue;
		//持っているアイテムをマシンに設置する処理
		if (player_.GetIsHolding() &&
			CommonUtility::IsHitSpheres(pSphere.GetPos(), pSphere.GetRadius(),
				objects_[i]->GetSpherePos(), objects_[i]->GetSphereRad()))
		{
			objects_[i]->Interact(player_.GetHoldItem());
		}

		//設置して一定時間経ったら氷入りカップを出力する
		if (objects_[i]->GetParam().interactTime_ <= 0.0f)
		{
			//コーヒーを生成する処理
			ProduceCoffee(i);
			break;
		}
	}

	//ディスペンサーとカップの処理
	for (int i = 0; i < static_cast<int>(objects_.size()); ++i)
	{
		//ディスペンサーの判定だけさせたい
		if (objects_[i]->GetParam().id_ != StageObject::ICE_DISPENSER)continue;
		//持っているアイテムをマシンに設置する処理
		if (player_.GetIsHolding() &&
			CommonUtility::IsHitSpheres(pSphere.GetPos(), pSphere.GetRadius(),
				objects_[i]->GetSpherePos(), objects_[i]->GetSphereRad()))
		{
			objects_[i]->Interact(player_.GetHoldItem());
		}

		//設置して一定時間経ったら氷入りカップを出力する
		if (objects_[i]->GetParam().interactTime_ <= 0.0f)
		{
			//氷入りカップを生成する処理
			DispenseIce2Cup(i);
			break;
		}
	}
}

void StageManager::LidRackInteract(void)
{
	auto& pSphere = player_.GetSphere();
	//コーヒーと蓋のラックとの処理
	for (const auto& obj : objects_)
	{
		if (obj->GetParam().id_ != StageObject::CUP_LID_RACK)continue;

		//持っているコーヒーに蓋をつける処理
		//プレイヤーが何か持っている状態で蓋のラックに近づいたら処理する
		if (player_.GetIsHolding() &&
			CommonUtility::IsHitSpheres(pSphere.GetPos(), pSphere.GetRadius(),
				obj->GetSpherePos(), obj->GetSphereRad()))
		{
			//蓋のラックのインタラクト処理
			obj->Interact(player_.GetHoldItem());
		}
		else
		{
			//判定外の場合は初期値に戻す
			obj->IsNotActioned();
			obj->SetInteractTime(StageObject::LID_RACK_INTERACT_TIME);
		}

		//インタラクトし続けて一定時間経ったら蓋をする
		if (obj->GetParam().interactTime_ <= 0.0f)
		{
			//蓋をする処理
			LidFollowCup();
			break;
		}
	}
}

void StageManager::ProduceCoffee(int index)
{
	//コーヒーマシン以外のオブジェクトは判定しない
	if (objects_[index]->GetParam().id_ != StageObject::COFFEE_MACHINE) return;
	const auto& machine = objects_[index];
	for (int i = 0; i < static_cast<int>(objects_.size()); ++i)
	{
		//ホット用カップ以外のオブジェクトは判定しない
		if (objects_[i]->GetParam().id_ != StageObject::HOT_CUP &&
			objects_[i]->GetParam().id_ != StageObject::CUP_WITH_ICE) continue;

		//マシンの判定だけさせたい
		if (machine->GetParam().id_ != StageObject::COFFEE_MACHINE)continue;

		//マシンの球体と設置されているカップだけ処理する
		if (objects_[i]->GetItemState() == StageObject::ITEM_STATE::PLACED &&
			CommonUtility::IsHitSpheres(machine->GetPos(), machine->GetSphereRad(),
				objects_[i]->GetSpherePos(), objects_[i]->GetSphereRad()))
		{
			if (objects_[i]->GetParam().id_ == StageObject::HOT_CUP)
			{
				MakeCoffee(i, *machine, StageObject::HOT_COFFEE);
				return; // ホットコーヒーを作ったら処理を終了
			}
			else {
				MakeCoffee(i, *machine, StageObject::ICE_COFFEE);
				return; // アイスコーヒーを作ったら処理を終了
			}
		}

	}
}

void StageManager::MakeCoffee(int index, StageObject& obj, std::string objName)
{
	//アイスコーヒーを作る処理
	if (objName == StageObject::ICE_COFFEE)
	{
		//氷入りカップの場合は氷も削除
		ItemObject* cupWithIce = dynamic_cast<ItemObject*>(objects_[index].get());
		if (cupWithIce->IsIce())
		{
			//蓋のインデックスを探す
			for (int i = 0; i < objects_.size(); ++i)
			{
				//dynamic_castでFollowingObject型に変換し、親参照を比較
				//蓋を削除する
				FollowingObject* follower = dynamic_cast<FollowingObject*>(objects_[i].get());
				if (follower && &(follower->GetFollowedObj()) == objects_[index].get())
				{
					objects_.erase(objects_.begin() + i);
					continue; // eraseしたのでiは進めず次のループへ
				}
			}
		}
	}

	//設置されているカップをコーヒーに上書きする
	objects_[index] = std::make_unique<ItemObject>(objName,player_);
	//マシンの上に乗るようにカップを配置する
	VECTOR cupPos = obj.GetTransform().pos;
	cupPos = VAdd(cupPos,
		{ 0.0f,StageObject::MACHINE_OFSET_Y ,StageObject::MACHINE_OFSET_Z });

	//マシンの回転に合わせてカップの位置を調整
	VECTOR rotPos = CommonUtility::RotXZPos(obj.GetTransform().pos, cupPos,
		Quaternion::ToEuler(obj.GetTransform().quaRotLocal).y);
	//カップ座標を設定して初期化
	objects_[index]->Init(rotPos);
	//設置状態に変更
	objects_[index]->ChangeItemState(StageObject::ITEM_STATE::PLACED);
}

void StageManager::DispenseIce2Cup(int index)
{
	//ディスペンサー以外のオブジェクトは判定しない
	if (objects_[index]->GetParam().id_ != StageObject::ICE_DISPENSER) return;
	const auto& dispenser = objects_[index];
	for (size_t i = 0; i < objects_.size(); ++i)
	{
		//ディスペンサーの判定だけさせたい
		if (objects_[index]->GetParam().id_ != StageObject::ICE_DISPENSER)continue;

		//マシンの球体と設置されているカップだけ処理する
		if (objects_[i]->GetItemState() == StageObject::ITEM_STATE::PLACED &&
			CommonUtility::IsHitSpheres(
				dispenser->GetPos(), dispenser->GetSphereRad(),
				objects_[i]->GetSpherePos(), objects_[i]->GetSphereRad()))
		{
			//設置されているカップに氷を入れる
			if (auto iceCup = dynamic_cast<ItemObject*>(objects_[i].get()))
			{
				iceCup->PouredIce();

				//氷を生成＆追従させる
				objects_.emplace_back(std::make_unique<FollowingObject>("Ice",player_, *objects_[i]));
				objects_.back()->Init(CommonUtility::VECTOR_ZERO);
				objects_.back()->Update();
				break;
			}
		}
	}
}

void StageManager::LidFollowCup(void)
{
	for (size_t i = 0; i < objects_.size(); ++i)
	{
		//コーヒー以外のオブジェクトは判定しない
		if (objects_[i]->GetParam().id_ != StageObject::HOT_COFFEE &&
			objects_[i]->GetParam().id_ != StageObject::ICE_COFFEE) continue;

		//既に蓋が乗っているカップは判定しない
		if (objects_[i]->IsLidOn())continue;

		for (const auto& lid : objects_)
		{
			//蓋の判定だけさせたい
			if (lid->GetParam().id_ != StageObject::CUP_LID_RACK)continue;

			//持っているコーヒーに蓋をする
			if (objects_[i]->GetItemState()== StageObject::ITEM_STATE::HOLD)
			{
				//コーヒーオブジェクトに蓋をする
				objects_[i]->PutOnTheLid();
				std::string lidType = "";
				if(objects_[i]->GetParam().id_ == StageObject::HOT_COFFEE)lidType = StageObject::HOTCUP_LID;
				else if (objects_[i]->GetParam().id_ == StageObject::ICE_COFFEE)lidType = StageObject::ICECUP_LID;
				//蓋を生成＆追従させる
				objects_.emplace_back(std::make_unique<FollowingObject>(lidType, player_, *objects_[i]));
				objects_.back()->Init(CommonUtility::VECTOR_ZERO);
				objects_.back()->Update();
				break;
			}
		}
	}
}

void StageManager::DustBoxInteract(void)
{
	auto& ins = InputManager::GetInstance();
	auto& pSphere = player_.GetSphere();

	// ゴミ箱の処理
	for (const auto& obj : objects_)
	{
		if (obj->GetParam().id_ != StageObject::DUST_BOX) continue;

		// プレイヤーが持っているアイテムをゴミ箱に近づけている場合
		if (player_.GetIsHolding() &&
			CommonUtility::IsHitSpheres(pSphere.GetPos(), pSphere.GetRadius(),
				obj->GetSpherePos(), obj->GetSphereRad()))
		{
			if(ins.IsInputTriggered("Interact"))
			{
				//ゴミ箱にアイテムを捨てる処理
				obj->Interact(player_.GetHoldItem());
			}
			break;
		}
	}
}

bool StageManager::IsOrderCompleted(void)
{
	//全ての注文が提供されたか確認
	for(bool isSurved : isServedItems_)
	{
		if (!isSurved)
		{
			return false; // 1つでも未提供のアイテムがあればfalse
			break;
		}
	}
	return true;
}