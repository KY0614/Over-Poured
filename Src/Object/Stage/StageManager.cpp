#include <DxLib.h>
#include "../Common/DebugDrawFormat.h"
#include "../Utility/AsoUtility.h"
#include "../Libs/ImGui/imgui.h"
#include "../Manager/Generic/ResourceManager.h"
#include "../Manager/Generic/InputManager.h"
#include "../Player.h"
#include "../Common/Sphere.h"
#include "../Common/AnimationController.h"
#include "StageObjectLibrary.h"
#include "StageObject.h"
#include "StageObject/ItemObject.h"
#include "StageObject/RackObject.h"
#include "StageObject/FollowingObject.h"
#include "StageObject/Table.h"
#include "StageObject/Machine.h"
#include "StageObject/CupLidRack.h"
#include "StageObject/DustBox.h"
#include "StageObject/IceDispenser.h"	
#include "Interact2D.h"	
#include "../UI/GaugeUI.h"	
#include "StageManager.h"

namespace {
	const std::string TABLE = "Table";		//テーブル
	const std::string COUNTER = "Counter";	//カウンター
	const std::string HOT_CUP = "Hot_Cup";	//ホット用カップ
	const std::string ICE_CUP = "Ice_Cup";	//アイス用カップ
	const std::string HOT_CUP_RACK = "Cup_Hot_Rack";		//ホット用ラック
	const std::string ICE_CUP_RACK = "Cup_Ice_Rack";		//アイス用ラック
	const std::string CHOCO_SWEETSRACK = "Sweets_Choco_Rack";		//チョコスイーツ用ラック
	const std::string BERRY_SWEETSRACK = "Sweets_Strawberry_Rack";	//ベリースイーツ用ラック
	const std::string CHOCO_SWEETS = "Sweets_Choco";		//チョコスイーツ
	const std::string BERRY_SWEETS = "Sweets_Strawberry";	//ベリースイーツ
	const std::string CUP_WITH_ICE = "Cup_With_Ice";		//アイス用カップ
	const std::string HOT_COFFEE = "Hot_Coffee";			//ホットコーヒー
	const std::string ICE_COFFEE = "Ice_Coffee";			//アイスコーヒー
	const std::string COFFEE_MACHINE = "Coffee_Machine";	//コーヒーマシン
	const std::string CUP_LID_RACK = "Cup_Lid_Rack";		//蓋のラック
	const std::string HOTCUP_LID = "Hot_Cup_Lid";					//蓋（ホット）
	const std::string ICECUP_LID = "Ice_Cup_Lid";					//蓋
	const std::string ICE_DISPENSER = "Ice_Dispenser";		//製氷機
	const std::string DUST_BOX = "Dust_Box";				//ゴミ箱
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

	mode_ = MODE::GAME_3D; // 初期モードを3Dゲームに設定

	//状態管理
	stateChanges_.emplace(MODE::GAME_3D, std::bind(&StageManager::ChangeMode3DGame, this));
	stateChanges_.emplace(MODE::MACHINE_2D, std::bind(&StageManager::ChangeModeMachine2D, this));
	stateChanges_.emplace(MODE::ICE_2D, std::bind(&StageManager::ChangeModeIce2D, this));
	stateChanges_.emplace(MODE::LIDRACK_2D, std::bind(&StageManager::ChangeModeLidRack2D, this));

	animationController_ = nullptr;
}

StageManager::~StageManager(void)
{
	isServedItems_.clear();
}

void StageManager::Init(void)
{
	//モデル制御の基本情報
	caseTran_.SetModel(
		ResourceManager::GetInstance().LoadModelDuplicate(
			ResourceManager::SRC::SWEETS_CASE));
	caseTran_.scl = AsoUtility::VECTOR_ONE;
	caseTran_.pos = CASE_POS;
	caseTran_.quaRot = Quaternion::Euler(
		0.0f,0.0f,0.0f);

	caseTran_.quaRotLocal = Quaternion();
	caseTran_.MakeCollider(Collider::TYPE::STAGE);
	caseTran_.Update();

	//モデル制御の基本情報
	transform_.SetModel(
		ResourceManager::GetInstance().LoadModelDuplicate(
			ResourceManager::SRC::REGISTER));
	transform_.scl = AsoUtility::VECTOR_ONE;
	transform_.pos = AsoUtility::VECTOR_ZERO;
	transform_.quaRot = Quaternion::Euler(
		0.0f,
		AsoUtility::Deg2RadF(0.0f),
		0.0f
	);

	transform_.quaRotLocal = Quaternion();
	transform_.MakeCollider(Collider::TYPE::STAGE);
	transform_.Update();

	InitAnimation(); // アニメーションの初期化

	//横のテーブル群(手前)
	for (int x = 0; x < TABLE_ROW_FRONT_NUM; x++)
	{
		VECTOR firstPos = TABLE_POS_BACK;
		firstPos.x += (x * TABLE_WIDTH);
		tables_.emplace_back(std::make_unique<Table>(TABLE, TABLE_WIDTH, 76.0f, 60.0f, player_,objects_));
		tables_.back()->Init(firstPos);
		//tables_.back()->SetPos(firstPos);
		tables_.back()->SetQuaRotY(180.0f);
	}

	//縦のテーブル群
	for (int z = TABLE_ROW_FRONT_NUM; z < TABLE_ROW_FRONT_NUM + TABLE_COLUMN_NUM; z++)
	{
		VECTOR firstPos = COLUMN_TABLE_POS;
		firstPos.z += ((z - TABLE_ROW_FRONT_NUM) * TABLE_WIDTH);
		tables_.emplace_back(std::make_unique<Table>(TABLE, 60.0f, 76.0f, TABLE_WIDTH, player_,objects_));
		tables_.back()->Init(firstPos);
		//tables_.back()->SetPos(firstPos);
		tables_.back()->SetQuaRotY(-90.0f);
	}

	//横のテーブル群(奥側)
	for (int x = 0; x < TABLE_ROW_BACK_NUM; x++)
	{
		VECTOR firstPos = TABLE_POS_FRONT;
		firstPos.x += (x * (TABLE_WIDTH + 20.0f));
		tables_.emplace_back(std::make_unique<Table>(TABLE, TABLE_WIDTH + 20.0f, 76.0f, 60.0f, player_, objects_));
		tables_.back()->Init(firstPos);
	}

	//カウンター用テーブル
	counter_ = std::make_unique<Table>(COUNTER, TABLE_WIDTH, 76.0f, 60.0f, player_, objects_);
	counter_->Init(COUNTER_POS);

	VECTOR pos = tables_[TABLE_ROW_FRONT_NUM + TABLE_COLUMN_NUM - 1]->GetTopCenter();

	//ホット用カップのラック
	objects_.emplace_back(std::make_unique<RackObject>(HOT_CUP_RACK, 60.0f, 20.0f, 60.0f, player_));
	objects_.back()->Init(pos);
	objects_.back()->SetQuaRotY(-90.0f);
	
	//アイス用カップのラック
	pos = tables_[TABLE_COLUMN_NUM]->GetTopCenter();
	objects_.emplace_back(std::make_unique<RackObject>(ICE_CUP_RACK, 60.0f, 20.0f, 60.0f, player_));
	objects_.back()->Init(pos);
	
	//チョコスイーツ用のラック
	pos = tables_[tables_.size() - 2]->GetTopCenter();
	objects_.emplace_back(std::make_unique<RackObject>(CHOCO_SWEETSRACK, 60.0f, 20.0f, 60.0f, player_));
	objects_.back()->Init(pos);
	
	//ベリースイーツ用のラック
	pos = tables_.back()->GetTopCenter();
	objects_.emplace_back(std::make_unique<RackObject>(BERRY_SWEETSRACK, 60.0f, 20.0f, 60.0f, player_));
	objects_.back()->Init(pos);
	
	//カップ用の蓋
	pos = tables_[2]->GetTopCenter();
	objects_.emplace_back(std::make_unique<CupLidRack>(CUP_LID_RACK, 60.0f, 20.0f, 60.0f, player_,objects_));
	objects_.back()->Init(pos);

	//コーヒーマシン
	pos = tables_[5]->GetTopCenter();
	objects_.emplace_back(std::make_unique<Machine>(COFFEE_MACHINE, 50.0f, 60.0f, 75.0f,
		 player_,objects_));
	objects_.back()->Init(pos);
	objects_.back()->SetQuaRotY(-90.0f);

	//アイスディスペンサー
	pos = tables_[4]->GetTopCenter();
	objects_.emplace_back(std::make_unique<IceDispenser>(ICE_DISPENSER, 50.0f, 75.0f, 60.0f,
		 player_,objects_));
	objects_.back()->Init(pos);
	objects_.back()->SetQuaRotY(-180.0f);

	//ゴミ箱
	objects_.emplace_back(std::make_unique<DustBox>(DUST_BOX, 50.0f, 75.0f, 60.0f,
		 player_,objects_));
	objects_.back()->Init(DUST_BOX_POS);
	objects_.back()->SetQuaRotY(-180.0f);
	dustBoxTran_ = objects_.back()->GetTransform(); // ゴミ箱のTransformを保存

	interact2D_ = std::make_unique<Interact2D>();
	interact2D_->Init(); // 2Dインタラクト用の初期化

	ChangeMode(MODE::GAME_3D); // 初期モードを3Dゲームに設定
}

void StageManager::Update(void)
{
	//更新ステップ
	modeUpdate_();
	animationController_->Update();

	interact2D_->Update(); // 2Dインタラクトの更新

	if (animationController_->IsEnd())
	{
		//アニメーションが終わったらマシンモードに切り替え
		animationController_->Play((int)ANIM_TYPE::IDLE);
	}

	auto& ins = InputManager::GetInstance();
	if(ins.IsTrgDown(KEY_INPUT_Q))
	{
		//interact2D_->ChangeMode(Interact2D::MODE::MACHINE_2D);
		//ChangeMode(MODE::MACHINE_2D);
		
		//生成アニメーション
		animationController_->Play((int)ANIM_TYPE::CREATE,false);
	}
	if (ins.IsTrgDown(KEY_INPUT_E))
	{
		//interact2D_->ChangeMode(Interact2D::MODE::GAME_3D);
		//ChangeMode(MODE::GAME_3D);
		animationController_->Play((int)ANIM_TYPE::PAYING, false);
	}

#ifdef _DEBUG

	//ImGuiの操作を行う
	//UpdateDebugImGui();

#endif // _DEBUG
}

void StageManager::Draw(void)
{
	modeDraw_(); // 現在のモードに応じた描画関数を呼び出す

	//モデルの描画
	MV1DrawModel(transform_.modelId);
	MV1DrawModel(caseTran_.modelId);

	for (const auto& table : tables_)
	{
		table->Draw();
	}

	counter_->Draw();

	for (const auto& obj : objects_)
	{
		obj->Draw();
	}

	//interact2D_->Draw(); // 2Dインタラクトの描画
#ifdef _DEBUG
	//DrawDebug();
#endif // _DEBUG
}

void StageManager::SetCurrentOrder(const Order::OrderData& order)
{
	//注文数文の配列を確保し、初期化＆お客の注文内容を受け取る
	size_t size = static_cast<size_t>(order.num_);
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

Transform StageManager::GetCounterTran(void) const
{
	return counter_->GetTransform(); 
}

Transform StageManager::GetTableTran(int index) const
{
	if (index > tables_.size())return Transform(); // 範囲外のインデックスは無視
	return tables_[index]->GetTransform();
}

Transform StageManager::GetShowCase(void) const
{
	return caseTran_;
}

Transform StageManager::GetDustBox(void) const
{
	return dustBoxTran_;
}

void StageManager::InitAnimation(void)
{
	std::string path = Application::PATH_MODEL + "Stage/";
	animationController_ = std::make_unique<AnimationController>(transform_.modelId);

	animationController_->Add((int)ANIM_TYPE::CREATE, path + "create_register.mv1", 30.0f);

	animationController_->Add((int)ANIM_TYPE::PAYING, path + "paying.mv1", 30.0f);
	animationController_->Play((int)ANIM_TYPE::IDLE);
}

void StageManager::SurveItem(StageObject& obj)
{
	// 商品情報をOrderDataに変換
	Order::OrderData data;
	// 例: objのIDや状態からOrderDataをセット
	if (obj.GetParam().id_ == HOT_COFFEE)
	{
		servedItems_.drink_ = Order::DRINK::HOT;
		servedItems_.lid_ = obj.IsLidOn();
		isServedItems_.front() = true;
	}
	else if (obj.GetParam().id_ == ICE_COFFEE)
	{
		servedItems_.drink_ = Order::DRINK::ICE;
		servedItems_.lid_ = obj.IsLidOn();
		isServedItems_.front() = true;
	}

	if (obj.GetParam().id_ == CHOCO_SWEETS)
	{
		servedItems_.sweets_ = Order::SWEETS::CHOCO;
		isServedItems_.back() = true;
	}
	else if (obj.GetParam().id_ == BERRY_SWEETS)
	{
		servedItems_.sweets_ = Order::SWEETS::STRAWBERRY;
		isServedItems_.back() = true;
	}

	DeleteSurvedItem(); // 提供後はオブジェクトを削除

	// 注文が揃ったか判定
	if (IsOrderCompleted()) 
	{
		isServed_ = true;
		animationController_->Play((int)ANIM_TYPE::PAYING, false);
	}
}

void StageManager::DeleteSurvedItem(void)
{
	//カウンターの球体と当たっているオブジェクトを削除
	for (auto it = objects_.begin(); it != objects_.end(); )
	{
		if (AsoUtility::IsHitSpheres(
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

void StageManager::CarryableObjInteract(void)
{
	auto& pSphere = player_.GetSphere();
	for (const auto& obj : objects_)
	{
		//カウンターで商品を提供する処理
		if (AsoUtility::IsHitSpheres(obj->GetSpherePos(), obj->GetSphereRad(),
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
			AsoUtility::IsHitSpheres(pSphere.GetPos(), pSphere.GetRadius(),
				obj->GetSpherePos(), obj->GetSphereRad()))
		{
			obj->ItemCarry();
			break;
		}

		//プレイヤーがアイテムを持っているときの処理
		if (player_.GetIsHolding())
		{
			//カウンターに設置
			if (AsoUtility::IsHitSpheres(obj->GetSpherePos(), obj->GetSphereRad(),
				counter_->GetSpherePos(), counter_->GetSphereRad()
			))
			{
				auto items = counter_->GetParam().acceptedItems_;
				//objIdがインタラクト対象物に存在するかどうか
				bool isAccepted = std::find(items.begin(), items.end(), obj->GetParam().id_) != items.end();
				if (!isAccepted)continue;	//存在しなかったら処理しない

				obj->ItemPlaced(counter_->GetTopCenter());
			}

			for (const auto& table : tables_)
			{
				//設置可能なテーブルの上にアイテムを設置する処理
				if (table->GetParam().placeable_ &&
					AsoUtility::IsHitSpheres(pSphere.GetPos(), pSphere.GetRadius(),
						table->GetSpherePos(), table->GetSphereRad()
					))
				{
					obj->ItemPlaced(table->GetTopCenter());
				}

				if (obj->IsActioned())
				{
					break;
				}
			}
		}
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
	for (const auto& obj : objects_)
	{
		if (obj->GetParam().id_ != COFFEE_MACHINE)continue;

		//持っているアイテムをマシンに設置する処理
		if (player_.GetIsHolding() &&
			AsoUtility::IsHitSpheres(pSphere.GetPos(), pSphere.GetRadius(),
				obj->GetSpherePos(), obj->GetSphereRad()))
		{
 			obj->Interact(player_.GetHoldItem());
		}

		//設置して一定時間経ったらコーヒーを出力する
		if (obj->GetParam().id_ == COFFEE_MACHINE &&
			obj->GetParam().interactTime_ <= 0.0f)
		{
			ProduceCoffee();
			break;
		}
	}

	//ディスペンサーとカップの処理
	for (const auto& obj : objects_)
	{
		if (obj->GetParam().id_ != ICE_DISPENSER)continue;

		//持っているアイテムをマシンに設置する処理
		if (player_.GetIsHolding() &&
			AsoUtility::IsHitSpheres(pSphere.GetPos(), pSphere.GetRadius(),
				obj->GetSpherePos(), obj->GetSphereRad()))
		{
			obj->Interact(player_.GetHoldItem());
		}

		//設置して一定時間経ったら氷入りカップを出力する
		if (obj->GetParam().interactTime_ <= 0.0f)
		{
			DispenseIce2Cup();
			break;
		}
	}
}

void StageManager::LidRackInteract(void)
{
	auto& pSphere = player_.GetSphere();
	//コーヒーと蓋の処理
	for (const auto& obj : objects_)
	{
		if (obj->GetParam().id_ != CUP_LID_RACK)continue;

		//持っているコーヒーに蓋をつける処理
		if (player_.GetIsHolding() &&
			AsoUtility::IsHitSpheres(pSphere.GetPos(), pSphere.GetRadius(),
				obj->GetSpherePos(), obj->GetSphereRad()))
		{
			obj->Interact(player_.GetHoldItem());
		}
		else
		{
			//判定外の場合は初期値に戻す
			obj->IsNotActioned();
			obj->SetInteractTime(3.0f);
		}

		//インタラクトし続けて一定時間経ったら蓋をする
		if (obj->GetParam().interactTime_ <= 0.0f)
		{
			LidFollowCup();
			break;
		}
	}
}

void StageManager::ProduceCoffee(void)
{
	for (size_t i = 0; i < objects_.size(); ++i)
	{
		//ホット用カップ以外のオブジェクトは判定しない
		if (objects_[i]->GetParam().id_ != HOT_CUP &&
			objects_[i]->GetParam().id_ != CUP_WITH_ICE) continue;

		for (const auto& machine : objects_)
		{
			//マシンの判定だけさせたい
			if (machine->GetParam().id_ != COFFEE_MACHINE)continue;

			//マシンの球体と設置されているカップだけ処理する
			if (objects_[i]->GetItemState() == StageObject::ITEM_STATE::PLACED &&
				AsoUtility::IsHitSpheres(machine->GetPos(), machine->GetSphereRad(),
					objects_[i]->GetSpherePos(), objects_[i]->GetSphereRad()))
			{
				if(objects_[i]->GetParam().id_ == HOT_CUP)
				{
					MakeCoffee(i,machine->GetTransform().pos, HOT_COFFEE);
					return; // ホットコーヒーを作ったら処理を終了
				}
				else {
					MakeCoffee(i, machine->GetTransform().pos, ICE_COFFEE);
					return; // アイスコーヒーを作ったら処理を終了
				}
			}
		}
	}
}

void StageManager::MakeCoffee(int index, VECTOR pos, std::string objName)
{
	//アイスコーヒーを生成する場合は氷を先に削除しておく
	if (objName == ICE_COFFEE)
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
	objects_[index] = std::make_unique<ItemObject>(objName, 20.0f, 30.0f, 20.0f, player_);
	//マシンの上に乗るようにカップを配置する
	VECTOR cupPos = pos;
	cupPos.y += StageObject::MACHINE_OFSET_Y;	//少し上にずらす
	cupPos.x += StageObject::MACHINE_OFSET_X;	//少し右にずらす
	objects_[index]->Init(cupPos);
	objects_[index]->ChangeItemState(StageObject::ITEM_STATE::PLACED);
}

void StageManager::DispenseIce2Cup(void)
{
	for (size_t i = 0; i < objects_.size(); ++i)
	{
		//アイス用カップ以外のオブジェクトは判定しない
		if (objects_[i]->GetParam().id_ != ICE_CUP) continue;

		for (const auto& machine : objects_)
		{
			//ディスペンサーの判定だけさせたい
			if (machine->GetParam().id_ != ICE_DISPENSER)continue;

			//マシンの球体と設置されているカップだけ処理する
			if (objects_[i]->GetItemState() == StageObject::ITEM_STATE::PLACED &&
				AsoUtility::IsHitSpheres(machine->GetPos(), machine->GetSphereRad(),
					objects_[i]->GetSpherePos(), objects_[i]->GetSphereRad()))
			{
				//設置されているカップに氷を入れる
				if (auto iceCup = dynamic_cast<ItemObject*>(objects_[i].get()))
				{
					iceCup->PouredIce();

					//氷を生成＆追従させる
					objects_.emplace_back(std::make_unique<FollowingObject>("Ice", 23.0f, 5.0f, 23.0f, player_, *objects_[i]));
					objects_.back()->Init(AsoUtility::VECTOR_ZERO);
					objects_.back()->Update();
					break;
				}
			}
		}
	}
}

void StageManager::LidFollowCup(void)
{
	bool isCreate = false;
	for (size_t i = 0; i < objects_.size(); ++i)
	{
		//コーヒー以外のオブジェクトは判定しない
		if (objects_[i]->GetParam().id_ != HOT_COFFEE &&
			objects_[i]->GetParam().id_ != ICE_COFFEE) continue;

		//既に蓋が乗っているカップは判定しない
		if (objects_[i]->IsLidOn())continue;

		for (const auto& lid : objects_)
		{
			//蓋の判定だけさせたい
			if (lid->GetParam().id_ != CUP_LID_RACK)continue;

			//蓋の球体と当たっているカップだけ処理する
			if (AsoUtility::IsHitSpheres(lid->GetPos(), lid->GetSphereRad(),
				objects_[i]->GetSpherePos(), objects_[i]->GetSphereRad()))
			{
				//コーヒーオブジェクトに蓋をする
				objects_[i]->PutOnTheLid();
				std::string lidType = "";
				if(objects_[i]->GetParam().id_ == HOT_COFFEE)lidType = HOTCUP_LID;
				else if (objects_[i]->GetParam().id_ == ICE_COFFEE)lidType = ICECUP_LID;
				//蓋を生成＆追従させる
				objects_.emplace_back(std::make_unique<FollowingObject>(lidType, 23.0f, 5.0f, 23.0f, player_, *objects_[i]));
				objects_.back()->Init(AsoUtility::VECTOR_ZERO);
				objects_.back()->Update();
				isCreate = true;
				break;
			}
		}
		if (isCreate)break;
	}
}

void StageManager::DustBoxInteract(void)
{
	auto& ins = InputManager::GetInstance();
	auto& pSphere = player_.GetSphere();

	// ゴミ箱の処理
	for (const auto& obj : objects_)
	{
		if (obj->GetParam().id_ != DUST_BOX) continue;

		// プレイヤーが持っているアイテムをゴミ箱に近づけている場合
		if (player_.GetIsHolding() &&
			AsoUtility::IsHitSpheres(pSphere.GetPos(), pSphere.GetRadius(),
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


void StageManager::ChangeMode(MODE mode)
{
	mode_ = mode;

	//各状態遷移の初期処理
	stateChanges_[mode_]();
}

void StageManager::ChangeMode3DGame(void)
{
	modeUpdate_ = std::bind(&StageManager::Update3DGame, this);
	modeDraw_ = std::bind(&StageManager::Draw3DGame, this);
}

void StageManager::ChangeModeMachine2D(void)
{
	modeUpdate_ = std::bind(&StageManager::UpdateMachine2D, this);
	modeDraw_ = std::bind(&StageManager::DrawMachine2D, this);
}

void StageManager::ChangeModeIce2D(void)
{
	modeUpdate_ = std::bind(&StageManager::UpdateIce2D, this);
	modeDraw_ = std::bind(&StageManager::DrawIce2D, this);
}

void StageManager::ChangeModeLidRack2D(void)
{
	modeUpdate_ = std::bind(&StageManager::UpdateLidRack2D, this);
	modeDraw_ = std::bind(&StageManager::DrawLidRack2D, this);
}

void StageManager::Update3DGame(void)
{
	auto& ins = InputManager::GetInstance();

	auto& pSphere = player_.GetSphere();

	for (const auto& obj : objects_)
	{
		obj->Update();
	}
 
	for (const auto& obj : tables_)                 
	{
		obj->Update();
	}

	counter_->Update();

	//ラックからカップを取り出す処理
	for (const auto& obj : objects_)
	{
		//プレイヤーが何も持っていないときの処理
		if (!player_.GetIsHolding() && obj->GetParam().interactable_ &&
			AsoUtility::IsHitSpheres(pSphere.GetPos(), pSphere.GetRadius(),
				obj->GetSpherePos(), obj->GetSphereRad()))
		{
			obj->PickUp(obj->GetParam().id_, objects_);
			break;
		}
	}

	//持ち運び可能なオブジェクトのインタラクト処理
	CarryableObjInteract();

	//マシンとのインタラクト処理
	MachineInteract();

	//蓋ラックとのインタラクト処理
	LidRackInteract();

	DustBoxInteract();

	transform_.Update();
	caseTran_.Update();
}

void StageManager::UpdateMachine2D(void)
{
	// TODO: 2Dマシンモードの更新処理
}

void StageManager::UpdateIce2D(void)
{
	// TODO: 2D製氷機モードの更新処理
}

void StageManager::UpdateLidRack2D(void)
{
	// TODO: 2Dリッドラックモードの更新処理
}

void StageManager::Draw3DGame(void)
{
	// TODO: 3Dゲームモードの描画処理
}

void StageManager::DrawMachine2D(void)
{
	// TODO: 2Dマシンモードの描画処理
}

void StageManager::DrawIce2D(void)
{
	// TODO: 2D製氷機モードの描画処理
}

void StageManager::DrawLidRack2D(void) {
	// TODO: 2Dリッドラックモードの描画処理
}

void StageManager::DrawDebug(void)
{
	//DrawSphere3D(objects_[3]->GetSpherePos(),
	//	objects_[3]->GetSphereRad(), 8, 0xff0000, 0xff0000, true);

	int line = 8;	//行
	int lineHeight = 30;	//行
	DebugDrawFormat::FormatString(L"item : %s", StringUtility::StringToWstring(player_.GetHoldItem()).c_str(), line, lineHeight);
	DebugDrawFormat::FormatString(L"hold : %d", player_.GetIsHolding(), line, lineHeight);
	//DebugDrawFormat::FormatString(L"mode : %d", mode_, line, lineHeight);

	//DebugDrawFormat::FormatString(L"currentD,S : %d,%d",
	//		currentOrder_.drink_, currentOrder_.sweets_, line, lineHeight);

	//DebugDrawFormat::FormatString(L"surevdD,S : %d,%d",
	//	servedItems_.drink_, servedItems_.sweets_, line, lineHeight);

	//DebugDrawFormat::FormatString(L"boolSize : %d",
	//		isServedItems_.size(), line, lineHeight);
		
	DebugDrawFormat::FormatString(L"back   : %s",
		StringUtility::StringToWstring(objects_.back()->GetParam().id_).c_str(), line, lineHeight);
	
	int size = static_cast<int>(objects_.size());
	DebugDrawFormat::FormatString(L"back -1  : %s",
		StringUtility::StringToWstring(objects_[size -2]->GetParam().id_).c_str(), line, lineHeight);
		
	DebugDrawFormat::FormatString(L"back -2  : %s",
		StringUtility::StringToWstring(objects_[size -3]->GetParam().id_).c_str(), line, lineHeight);
		
	DebugDrawFormat::FormatString(L"back -3  : %s",
		StringUtility::StringToWstring(objects_[size -4]->GetParam().id_).c_str(), line, lineHeight);	
		
	DebugDrawFormat::FormatString(L"back -4  : %s",
		StringUtility::StringToWstring(objects_[size -5]->GetParam().id_).c_str(), line, lineHeight);
		
	//size_t size = objects_.size();
	////蓋生成数確認用
	//DebugDrawFormat::FormatString(L"end - 2 : %s",
	//	StringUtility::StringToWstring(objects_[size - 3]->GetObjectId()).c_str(), line, lineHeight);
	//
	//DebugDrawFormat::FormatString(L"end - 1 : %s",
	//	StringUtility::StringToWstring(objects_[size - 2]->GetObjectId()).c_str(), line, lineHeight);

	//DebugDrawFormat::FormatString(L"end : %s",
	//	StringUtility::StringToWstring(objects_.back()->GetObjectId()).c_str(), line, lineHeight);

	//DebugDrawFormat::FormatString(L"surveD : %d",
	//	surveDrink_, line, lineHeight);
	//
	//DebugDrawFormat::FormatString(L"surveS : %d",
	//	surveSweets_, line, lineHeight);
	//
	//DebugDrawFormat::FormatString(L"surveL : %d",
	//	surveDrinkLid_, line, lineHeight);
	//
	//DebugDrawFormat::FormatString(L"surve : %d",
	//	isSurved_, line, lineHeight);

	//for (const auto& obj : objects_)
	//{
	//	DebugDrawFormat::FormatString(L"objActioned : %d",
	//		obj->IsActioned() , line, lineHeight);
	//}

	for (size_t i = 0; i < objects_.size(); ++i)
	{
		//コーヒー以外のオブジェクトは判定しない
		if (objects_[i]->GetParam().id_ != HOT_COFFEE &&
			objects_[i]->GetParam().id_ != ICE_COFFEE) continue;

		DebugDrawFormat::FormatString(L"coffee%d.lid  : %d", i,
			objects_[i]->IsLidOn(), line, lineHeight);
	}

	//for (int i = 0; i < tables_.size(); i++)
	//{
	//	DebugDrawFormat::FormatString(L"table%d.placeable  : %d", i,
	//		tables_[i]->GetIsPlaceable(), line, lineHeight);
	//}

	//テーブル番号を表示
	for (int i = 0; i < TABLE_COLUMN_NUM + TABLE_ROW_FRONT_NUM - 1; i++)
	{
		VECTOR screenPos = ConvWorldPosToScreenPos(tables_[i]->GetTransform().pos);
		// 変換成功
		DrawFormatString(static_cast<int>(screenPos.x) - 25, static_cast<int>(screenPos.y) - 50, GetColor(255, 255, 255),
			L"%s : %d",
			StringUtility::StringToWstring(tables_[i]->GetParam().id_.c_str()).c_str(), i);
	}

	for (const auto& obj : objects_)
	{
		VECTOR screenPos = ConvWorldPosToScreenPos(obj->GetTransform().pos);
		// 変換成功
		DrawFormatString(static_cast<int>(screenPos.x) - 25, static_cast<int>(screenPos.y) - 50, GetColor(255, 255, 255),
			L"%s",
			StringUtility::StringToWstring(obj->GetParam().id_.c_str()).c_str());
	}
}

void StageManager::UpdateDebugImGui(void)
{
	//ウィンドウタイトル&開始処理
	ImGui::Begin("case");
	////大きさ
	//ImGui::Text("scale");
	//ImGui::InputFloat("SclX", &sphereTran_.scl.x);
	//ImGui::InputFloat("SclY", &sphereTran_.scl.y);
	//ImGui::InputFloat("SclZ", &sphereTran_.scl.z);

	//角度
	//VECTOR rotDeg = VECTOR();
	//rotDeg.x = AsoUtility::Rad2DegF(sphereTran_.rot.x);
	//rotDeg.y = AsoUtility::Rad2DegF(sphereTran_.rot.y);
	//rotDeg.z = AsoUtility::Rad2DegF(sphereTran_.rot.z);
	//ImGui::Text("angle(deg)");
	//ImGui::SliderFloat("RotX", &rotDeg.x, 0.0f, 360.0f);
	//ImGui::SliderFloat("RotY", &rotDeg.y, 0.0f, 360.0f);
	//ImGui::SliderFloat("RotZ", &rotDeg.z, 0.0f, 360.0f);
	//sphereTran_.rot.x = AsoUtility::Deg2RadF(rotDeg.x);
	//sphereTran_.rot.y = AsoUtility::Deg2RadF(rotDeg.y);
	//sphereTran_.rot.z = AsoUtility::Deg2RadF(rotDeg.z);

	//位置
	ImGui::Text("position");
	//構造体の先頭ポインタを渡し、xyzと連続したメモリ配置へアクセス
	ImGui::InputFloat3("Pos", &caseTran_.pos.x);
	ImGui::SliderFloat("PosX", &caseTran_.pos.x, -500.0f, 500.0f);
	ImGui::SliderFloat("PosY", &caseTran_.pos.y, 0.0f, 500.0f);
	ImGui::SliderFloat("PosZ", &caseTran_.pos.z, -500.0f, 500.0f);

	//終了処理
	ImGui::End();
}
