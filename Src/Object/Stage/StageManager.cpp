#include <DxLib.h>
#include "../Common/DebugDrawFormat.h"
#include "../Utility/AsoUtility.h"
#include "../Libs/ImGui/imgui.h"
#include "../Manager/Generic/ResourceManager.h"
#include "../Manager/Generic/InputManager.h"
#include "../Player.h"
#include "../Common/Sphere.h"
#include "StageObjectLibrary.h"
#include "StageObject.h"
#include "StageObject/ItemObject.h"
#include "StageObject/RackObject.h"
#include "StageObject/Table.h"
#include "StageObject/Machine.h"
#include "StageObject/CupLidRack.h"
#include "StageObject/CupLid.h"
#include "StageObject/DustBox.h"
#include "StageObject/IceDispenser.h"	
#include "StageManager.h"

#pragma region メモ
//クラスが多すぎるので減らすためにカテゴリ分けしたクラスにしたい
//例：HotCup,HotCoffee,Machine,IceDisoenser→ItemObjectとかMachineObjectとか

//カテゴリ分けしたクラスにする際jsonデータをいじくるか検討中
#pragma endregion


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
	const std::string CUP_LID = "Cup_Lid";					//蓋
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

	//更新処理
	updateFunc_.emplace(MODE::GAME_3D, std::bind(&StageManager::Update3DGame, this));
	updateFunc_.emplace(MODE::MACHINE_2D, std::bind(&StageManager::UpdateMachine2D, this));
	updateFunc_.emplace(MODE::ICE_2D, std::bind(&StageManager::UpdateIce2D, this));
	updateFunc_.emplace(MODE::LIDRACK_2D, std::bind(&StageManager::UpdateLidRack2D, this));

	//描画処理
	drawFunc_.emplace(MODE::GAME_3D, std::bind(&StageManager::Draw3DGame, this));
	drawFunc_.emplace(MODE::MACHINE_2D, std::bind(&StageManager::DrawMachine2D, this));
	drawFunc_.emplace(MODE::ICE_2D, std::bind(&StageManager::DrawIce2D, this));
	drawFunc_.emplace(MODE::LIDRACK_2D, std::bind(&StageManager::DrawLidRack2D, this));

	ChangeMode(MODE::GAME_3D); // 初期モードを3Dゲームに設定
}

StageManager::~StageManager(void)
{
	isServedItems_.clear();
}

void StageManager::Init(void)
{
	//モデル制御の基本情報
	transform_.SetModel(
		ResourceManager::GetInstance().LoadModelDuplicate(
			ResourceManager::SRC::STAGE));
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

	//横のテーブル群(手前)
	for (int x = 0; x < TABLE_ROW_FRONT_NUM; x++)
	{
		VECTOR firstPos = TABLE_POS_BACK;
		firstPos.x += (x * TABLE_WIDTH);
		tables_.emplace_back(std::make_unique<Table>(TABLE, TABLE_WIDTH, 76.0f, 60.0f, player_,objects_));
		tables_.back()->Init();
		tables_.back()->SetPos(firstPos);
	}

	//縦のテーブル群
	for (int z = TABLE_ROW_FRONT_NUM; z < TABLE_ROW_FRONT_NUM + TABLE_COLUMN_NUM; z++)
	{
		VECTOR firstPos = COLUMN_TABLE_POS;
		firstPos.z += ((z - TABLE_ROW_FRONT_NUM) * TABLE_WIDTH);
		tables_.emplace_back(std::make_unique<Table>(TABLE, 60.0f, 76.0f, TABLE_WIDTH, player_,objects_));
		tables_.back()->Init();
		tables_.back()->SetPos(firstPos);
	}

	//横のテーブル群(奥側)
	for (int x = 0; x < TABLE_ROW_BACK_NUM; x++)
	{
		VECTOR firstPos = TABLE_POS_FRONT;
		firstPos.x += (x * TABLE_WIDTH);
		tables_.emplace_back(std::make_unique<Table>(TABLE, TABLE_WIDTH, 76.0f, 60.0f, player_, objects_));
		tables_.back()->Init();
		tables_.back()->SetPos(firstPos);
	}

	//カウンター用テーブル
	counter_ = std::make_unique<Table>(COUNTER, TABLE_WIDTH, 76.0f, 60.0f, player_, objects_);
	counter_->Init();
	counter_->SetPos(COUNTER_POS);

	//ホット用カップのラック
	objects_.emplace_back(std::make_unique<RackObject>(HOT_CUP_RACK, 60.0f, 20.0f, 60.0f, player_));
	objects_.back()->Init();
	objects_.back()->SetPos(tables_[TABLE_ROW_FRONT_NUM + TABLE_COLUMN_NUM - 1]->GetTopCenter());
	
	//アイス用カップのラック
	objects_.emplace_back(std::make_unique<RackObject>(ICE_CUP_RACK, 60.0f, 20.0f, 60.0f, player_));
	objects_.back()->Init();
	objects_.back()->SetPos(tables_[TABLE_COLUMN_NUM]->GetTopCenter());

	//チョコスイーツ用のラック
	objects_.emplace_back(std::make_unique<RackObject>(CHOCO_SWEETSRACK, 60.0f, 20.0f, 60.0f, player_));
	objects_.back()->Init();
	objects_.back()->SetPos(tables_[tables_.size() - 2]->GetTopCenter());
	
	//ベリースイーツ用のラック
	objects_.emplace_back(std::make_unique<RackObject>(BERRY_SWEETSRACK, 60.0f, 20.0f, 60.0f, player_));
	objects_.back()->Init();
	objects_.back()->SetPos(tables_.back()->GetTopCenter());
	
	//カップ用の蓋
	objects_.emplace_back(std::make_unique<CupLidRack>(CUP_LID_RACK, 60.0f, 20.0f, 60.0f, player_,objects_));
	objects_.back()->Init();
	objects_.back()->SetPos(tables_[2]->GetTopCenter());

	//コーヒーマシン
	objects_.emplace_back(std::make_unique<Machine>(COFFEE_MACHINE, 50.0f, 60.0f, 75.0f,
		 player_,objects_));
	objects_.back()->Init();
	VECTOR pos = AsoUtility::VECTOR_ZERO;
	pos = tables_.front()->GetTopCenter();
	objects_.back()->SetPos(tables_[5]->GetTopCenter());

	//アイスディスペンサー
	objects_.emplace_back(std::make_unique<IceDispenser>(ICE_DISPENSER, 50.0f, 75.0f, 60.0f,
		 player_,objects_));
	objects_.back()->Init();
	objects_.back()->SetPos(tables_[4]->GetTopCenter());

	//ゴミ箱
	objects_.emplace_back(std::make_unique<DustBox>(DUST_BOX, 50.0f, 75.0f, 60.0f,
		 player_,objects_));
	objects_.back()->Init();
	objects_.back()->SetPos(DUST_BOX_POS);

#ifdef _DEBUG

	//カウンター前の当たり判定用の球体
	sphereTran_.Update();

	sphereTran_.scl = AsoUtility::VECTOR_ONE;
	sphereTran_.pos = { 221.0f, 0.0f, 271.0f };
	sphereTran_.quaRot = Quaternion();
	sphereTran_.quaRotLocal =
		Quaternion::Euler({ 0.0f, AsoUtility::Deg2RadF(180.0f), 0.0f });

#endif // _DEBUG
}

void StageManager::Update(void)
{
	//更新ステップ
	modeUpdate_();

	auto& ins = InputManager::GetInstance();
	if(ins.IsTrgDown(KEY_INPUT_Q))
	{
		ChangeMode(MODE::ICE_2D);
	}
	if (ins.IsTrgDown(KEY_INPUT_E))
	{
		ChangeMode(MODE::GAME_3D);
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

	for (const auto& table : tables_)
	{
		table->Draw();
	}

	counter_->Draw();

	for (const auto& obj : objects_)
	{
		obj->Draw();
	}

#ifdef _DEBUG
	DrawDebug();
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

void StageManager::SurveItem(StageObject& obj)
{
	// 商品情報をOrderDataに変換
	Order::OrderData data;
	// 例: objのIDや状態からOrderDataをセット
	if (obj.GetObjectId() == HOT_COFFEE) 
	{
		servedItems_.drink_ = Order::DRINK::HOT;
		servedItems_.lid_ = obj.IsLidOn();
		isServedItems_.front() = true;
	}
	else if (obj.GetObjectId() == ICE_COFFEE) 
	{
		servedItems_.drink_ = Order::DRINK::ICE;
		servedItems_.lid_ = obj.IsLidOn();
		isServedItems_.front() = true;
	}

	if (obj.GetObjectId() == CHOCO_SWEETS)
	{
		servedItems_.sweets_ = Order::SWEETS::CHOCO;
		isServedItems_.back() = true;
	}
	else if (obj.GetObjectId() == BERRY_SWEETS)
	{
		servedItems_.sweets_ = Order::SWEETS::STRAWBERRY;
		isServedItems_.back() = true;
	}

	DeleteSurvedItem(); // 提供後はオブジェクトを削除

	// 注文が揃ったか判定
	if (IsOrderCompleted()) 
	{
		isServed_ = true;
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
			it = objects_.erase(it); //eraseは次の要素のイテレータを返す
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
		if (!player_.GetIsHolding() && obj->GetIsCarryable() &&
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
				bool isAccepted = std::find(items.begin(), items.end(), obj->GetObjectId()) != items.end();
				if (!isAccepted)continue;	//存在しなかったら処理しない

				obj->ItemPlaced(counter_->GetTopCenter());
			}

			for (const auto& table : tables_)
			{
				//設置可能なテーブルの上にアイテムを設置する処理
				if (table->GetIsPlaceable() &&
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
		if (obj->GetObjectId() != COFFEE_MACHINE)continue;

		//持っているアイテムをマシンに設置する処理
		if (player_.GetIsHolding() &&
			AsoUtility::IsHitSpheres(pSphere.GetPos(), pSphere.GetRadius(),
				obj->GetSpherePos(), obj->GetSphereRad()))
		{
 			obj->Interact(player_.GetHoldItem());
		}

		//if(obj->GetMachineState() == StageObject::MACHINE_STATE::ACTIVE)
		//{
		//	ChangeMode(MODE::MACHINE_2D); // マシンモードに切り替え
		//}

		//設置して一定時間経ったらコーヒーを出力する
		if (obj->GetObjectId() == COFFEE_MACHINE &&
			obj->GetInteractTime() <= 0.0f)
		{
			MakeCoffee();
			break;
		}
	}

	//ディスペンサーとカップの処理
	for (const auto& obj : objects_)
	{
		if (obj->GetObjectId() != ICE_DISPENSER)continue;

		//持っているアイテムをマシンに設置する処理
		if (player_.GetIsHolding() &&
			AsoUtility::IsHitSpheres(pSphere.GetPos(), pSphere.GetRadius(),
				obj->GetSpherePos(), obj->GetSphereRad()))
		{
			obj->Interact(player_.GetHoldItem());
		}

		//設置して一定時間経ったら氷入りカップを出力する
		if (obj->GetInteractTime() <= 0.0f)
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
		if (obj->GetObjectId() != CUP_LID_RACK)continue;

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
		if (obj->GetInteractTime() <= 0.0f)
		{
			LidFollowCup();
			break;
		}
	}
}

void StageManager::MakeCoffee(void)
{
	for (size_t i = 0; i < objects_.size(); ++i)
	{
		//ホット用カップ以外のオブジェクトは判定しない
		if (objects_[i]->GetObjectId() != HOT_CUP &&
			objects_[i]->GetObjectId() != CUP_WITH_ICE) continue;

		for (const auto& machine : objects_)
		{
			//マシンの判定だけさせたい
			if (machine->GetObjectId() != COFFEE_MACHINE)continue;

			//マシンの球体と設置されているカップだけ処理する
			if (objects_[i]->GetItemState() == StageObject::ITEM_STATE::PLACED &&
				AsoUtility::IsHitSpheres(machine->GetPos(), machine->GetSphereRad(),
					objects_[i]->GetSpherePos(), objects_[i]->GetSphereRad()))
			{
				if(objects_[i]->GetObjectId() == HOT_CUP)
				{
					MakeHotCoffee(i);
					return; // ホットコーヒーを作ったら処理を終了
				}
				else {
					MakeIceCoffee(i);
					return; // アイスコーヒーを作ったら処理を終了
				}
			}
		}
	}
}

void StageManager::MakeHotCoffee(int i)
{
	//設置されているカップをコーヒーに上書きする
	objects_[i] = std::make_unique<ItemObject>(HOT_COFFEE, 20.0f, 30.0f, 20.0f, player_);
	objects_[i]->Init();
	objects_[i]->ChangeItemState(StageObject::ITEM_STATE::PLACED);
	objects_[i]->SetPos(tables_[5]->GetTopCenter());
}

void StageManager::MakeIceCoffee(int i)
{
	//設置されているカップをコーヒーに上書きする
	objects_[i] = std::make_unique<ItemObject>(ICE_COFFEE, 20.0f, 30.0f, 20.0f, player_);
	objects_[i]->Init();
	objects_[i]->ChangeItemState(StageObject::ITEM_STATE::PLACED);
	objects_[i]->SetPos(tables_[5]->GetTopCenter());
}

void StageManager::DispenseIce2Cup(void)
{
	for (size_t i = 0; i < objects_.size(); ++i)
	{
		//アイス用カップ以外のオブジェクトは判定しない
		if (objects_[i]->GetObjectId() != ICE_CUP) continue;

		for (const auto& machine : objects_)
		{
			//ディスペンサーの判定だけさせたい
			if (machine->GetObjectId() != ICE_DISPENSER)continue;

			//マシンの球体と設置されているカップだけ処理する
			if (objects_[i]->GetItemState() == StageObject::ITEM_STATE::PLACED &&
				AsoUtility::IsHitSpheres(machine->GetPos(), machine->GetSphereRad(),
					objects_[i]->GetSpherePos(), objects_[i]->GetSphereRad()))
			{
				//設置されているカップに氷を入れる
				if (auto iceCup = dynamic_cast<ItemObject*>(objects_[i].get()))
				{
					iceCup->PouredIce();
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
		if (objects_[i]->GetObjectId() != HOT_COFFEE &&
			objects_[i]->GetObjectId() != ICE_COFFEE) continue;

		//既に蓋が乗っているカップは判定しない
		if (objects_[i]->IsLidOn())continue;

		for (const auto& lid : objects_)
		{
			//蓋の判定だけさせたい
			if (lid->GetObjectId() != CUP_LID_RACK)continue;

			//蓋の球体と当たっているカップだけ処理する
			if (AsoUtility::IsHitSpheres(lid->GetPos(), lid->GetSphereRad(),
				objects_[i]->GetSpherePos(), objects_[i]->GetSphereRad()))
			{
				//コーヒーオブジェクトに蓋をする
				objects_[i]->PutOnTheLid();

				//蓋を生成＆追従させる
				objects_.emplace_back(std::make_unique<CupLid>(CUP_LID, 23.0f, 5.0f, 23.0f, player_, *objects_[i]));
				objects_.back()->Init();
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
		if (obj->GetObjectId() != DUST_BOX) continue;

		// プレイヤーが持っているアイテムをゴミ箱に近づけている場合
		if (player_.GetIsHolding() &&
			AsoUtility::IsHitSpheres(pSphere.GetPos(), pSphere.GetRadius(),
				obj->GetSpherePos(), obj->GetSphereRad()))
		{
			if(ins.IsInputTriggered("Interact"))
			{
				//ゴミ箱にアイテムを捨てる処理
				//DiscardHoldObject();
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

	// 関数ポインタ切り替え
	modeUpdate_ = updateFunc_[mode_];
	modeDraw_ = drawFunc_[mode_];
}

void StageManager::ChangeMode3DGame(void)
{
	// TODO: ここに3Dゲームモード切り替え処理を書く
}

void StageManager::ChangeModeMachine2D(void)
{
	// TODO: ここに2Dマシンモード切り替え処理を書く
}

void StageManager::ChangeModeIce2D(void)
{
	// TODO: ここに2D製氷機モード切り替え処理を書く
}

void StageManager::ChangeModeLidRack2D(void)
{
	// TODO: ここに2Dリッドラックモード切り替え処理を書く
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
		if (!player_.GetIsHolding() && obj->GetIsInteractable() &&
			AsoUtility::IsHitSpheres(pSphere.GetPos(), pSphere.GetRadius(),
				obj->GetSpherePos(), obj->GetSphereRad()))
		{
			obj->PickUp(obj->GetObjectId(), objects_);
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
	sphereTran_.Update();
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
	DebugDrawFormat::FormatString(L"mode : %d", mode_, line, lineHeight);

	DebugDrawFormat::FormatString(L"currentD,S : %d,%d",
			currentOrder_.drink_, currentOrder_.sweets_, line, lineHeight);

	DebugDrawFormat::FormatString(L"surevdD,S : %d,%d",
		servedItems_.drink_, servedItems_.sweets_, line, lineHeight);

	DebugDrawFormat::FormatString(L"boolSize : %d",
			isServedItems_.size(), line, lineHeight);
		
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
		if (objects_[i]->GetObjectId() != HOT_COFFEE &&
			objects_[i]->GetObjectId() != ICE_COFFEE) continue;

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
			StringUtility::StringToWstring(tables_[i]->GetObjectId().c_str()).c_str(), i);
	}

	for (const auto& obj : objects_)
	{
		VECTOR screenPos = ConvWorldPosToScreenPos(obj->GetTransform().pos);
		// 変換成功
		DrawFormatString(static_cast<int>(screenPos.x) - 25, static_cast<int>(screenPos.y) - 50, GetColor(255, 255, 255),
			L"%s",
			StringUtility::StringToWstring(obj->GetObjectId().c_str()).c_str());
	}
}

void StageManager::UpdateDebugImGui(void)
{
	//ウィンドウタイトル&開始処理
	ImGui::Begin("Player:Circle");
	//大きさ
	ImGui::Text("scale");
	ImGui::InputFloat("SclX", &sphereTran_.scl.x);
	ImGui::InputFloat("SclY", &sphereTran_.scl.y);
	ImGui::InputFloat("SclZ", &sphereTran_.scl.z);

	//角度
	VECTOR rotDeg = VECTOR();
	rotDeg.x = AsoUtility::Rad2DegF(sphereTran_.rot.x);
	rotDeg.y = AsoUtility::Rad2DegF(sphereTran_.rot.y);
	rotDeg.z = AsoUtility::Rad2DegF(sphereTran_.rot.z);
	ImGui::Text("angle(deg)");
	ImGui::SliderFloat("RotX", &rotDeg.x, 0.0f, 360.0f);
	ImGui::SliderFloat("RotY", &rotDeg.y, 0.0f, 360.0f);
	ImGui::SliderFloat("RotZ", &rotDeg.z, 0.0f, 360.0f);
	sphereTran_.rot.x = AsoUtility::Deg2RadF(rotDeg.x);
	sphereTran_.rot.y = AsoUtility::Deg2RadF(rotDeg.y);
	sphereTran_.rot.z = AsoUtility::Deg2RadF(rotDeg.z);

	//位置
	ImGui::Text("position");
	//構造体の先頭ポインタを渡し、xyzと連続したメモリ配置へアクセス
	ImGui::InputFloat3("Pos", &sphereTran_.pos.x);
	ImGui::SliderFloat("PosX", &sphereTran_.pos.x, 0.0f, 360.0f);
	ImGui::SliderFloat("PosY", &sphereTran_.pos.y, 0.0f, 360.0f);
	ImGui::SliderFloat("PosZ", &sphereTran_.pos.z, 0.0f, 1000.0f);

	//終了処理
	ImGui::End();
}
