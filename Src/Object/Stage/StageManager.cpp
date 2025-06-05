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
#include "StageObject/Table.h"
#include "StageObject/Machine.h"
#include "StageObject/HotCup.h"
#include "StageObject/IceCup.h"
#include "StageObject/HotCupRack.h"
#include "StageObject/HotCoffee.h"
#include "StageObject/CupLidRack.h"
#include "StageObject/CupLid.h"
#include "StageObject/Counter.h"
#include "StageManager.h"

namespace {
	const std::string TABLE = "Table";		//テーブル
	const std::string COUNTER = "Counter";		//カウンター
	const std::string HOT_CUP = "Hot_Cup";	//ホット用カップ
	const std::string ICE_CUP = "Ice_Cup";	//アイス用カップ
	const std::string HOT_CUP_RACK = "Cup_Hot_Rack";	//ホット用ラック
	const std::string CUP_WITH_ICE = "Cup_With_Ice";		//アイス用カップ
	const std::string HOT_COFFEE = "Hot_Coffee";	//ホットコーヒー
	const std::string ICE_COFFEE = "Ice_Coffee";	//アイスコーヒー
	const std::string COFFEE_MACHINE = "Coffee_Machine";	//コーヒーマシン
	const std::string CUP_LID_RACK = "Cup_Lid_Rack";		//
	const std::string CUP_LID = "Cup_Lid";		//
}


StageManager::StageManager(Player& player):player_(player)
{
	isSurved_ = false;
	surveDrink_ = Order::DRINK::NONE;
	surveSweets_ = Order::SWEETS::NONE;

	surveDrinkLid_ = false;
}

StageManager::~StageManager(void)
{
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

	//横のテーブル群
	for (int x = 0; x < TABLE_NUM; x++)
	{
		VECTOR firstPos = TABLE_POS;
		firstPos.x += (x * TABLE_WIDTH);
		tables_.emplace_back(std::make_unique<Table>(TABLE, TABLE_WIDTH, 76.0f, 60.0f, player_,objects_));
		tables_.back()->Init();
		tables_.back()->SetPos(firstPos);
	}

	//縦のテーブル群
	for (int z = TABLE_NUM; z < TABLE_NUM + TABLE_NUM - 1; z++)
	{
		VECTOR firstPos = COLUMN_TABLE_POS;
		firstPos.z += ((z - TABLE_NUM) * TABLE_WIDTH);
		tables_.emplace_back(std::make_unique<Table>(TABLE, 60.0f, 76.0f, TABLE_WIDTH, player_,objects_));
		tables_.back()->Init();
		tables_.back()->SetPos(firstPos);
	}

	//カウンター用テーブル
	counter_ = std::make_unique<Counter>(COUNTER, TABLE_WIDTH, 76.0f, 60.0f, player_, objects_);
	counter_->Init();
	counter_->SetPos(COUNTER_POS);

	//ホット用カップのラック
	objects_.emplace_back(std::make_unique<HotCupRack>(HOT_CUP_RACK, 60.0f, 20.0f, 60.0f, player_));
	objects_.back()->Init();
	objects_.back()->SetPos(tables_[1]->GetTopCenter());
	
	//カップ用の蓋
	objects_.emplace_back(std::make_unique<CupLidRack>(CUP_LID_RACK, 60.0f, 20.0f, 60.0f, player_,objects_));
	objects_.back()->Init();
	objects_.back()->SetPos(tables_[2]->GetTopCenter());
	
	//ホット用カップ
	objects_.emplace_back(std::make_unique<HotCup>(HOT_CUP, 40.0f, 30.0f, 40.0f, player_));
	StageObject& cupHotRef = *objects_.back();
	objects_.back()->Init();
	objects_.back()->SetPos(tables_.back()->GetTopCenter());

	//アイス用カップ
	objects_.emplace_back(std::make_unique<IceCup>(ICE_CUP, 40.0f, 30.0f, 40.0f, player_));
	StageObject& cupIceRef = *objects_.back(); // 新しく追加されたIceCupへの参照を取得
	objects_.back()->Init();
	objects_.back()->SetPos(tables_[tables_.size() - 2]->GetTopCenter());

	//コーヒーマシン
	objects_.emplace_back(std::make_unique<Machine>(COFFEE_MACHINE, 75.0f, 60.0f, 50.0f,
		 player_,objects_));
	objects_.back()->Init();
	VECTOR pos = AsoUtility::VECTOR_ZERO;
	pos = tables_.front()->GetTopCenter();
	objects_.back()->SetPos(pos);

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
	auto& ins = InputManager::GetInstance();

	auto& pSphere = player_.GetSphere();

	counter_->Update();

	for (const auto& obj : objects_)
	{
		obj->Update();
	}

	for (const auto& obj : tables_)
	{
		obj->Update();
	}

	//ラックからカップを取り出す処理
	for (const auto& obj : objects_)
	{
		//プレイヤーが何も持っていないときの処理
		if (!player_.GetIsHolding() && obj->GetIsInteractable() &&
			AsoUtility::IsHitSpheres(pSphere.GetPos(), pSphere.GetRadius(),
				obj->GetSpherePos(), obj->GetSphereRad()))
		{
			obj->PickUp(objects_);
			break;
		}
	}

	//for (auto it = objects_.begin(); it != objects_.end(); /* incrementは中で */)
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
			}
			else 
			{
				//アイテムが設置されていない場合は提供データをリセット
				ResetServeData();
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

		//if (obj->IsActioned())
		//{
		//	++it;
		//	break;
		//}
		//else
		//{
		//	++it;
		//}
	}

	//マシンとカップの処理
	for (const auto& obj : objects_)
	{
		if (obj->GetObjectId() != COFFEE_MACHINE)continue;

		//持っているアイテムをマシンに設置する処理
		if (obj->GetInteractType() == "machine" && player_.GetIsHolding() &&
			AsoUtility::IsHitSpheres(pSphere.GetPos(), pSphere.GetRadius(),
				obj->GetSpherePos(), obj->GetSphereRad()))
		{
			obj->Interact(player_.GetHoldItem());
		}

		//設置して一定時間経ったらコーヒーを出力する
		if (obj->GetInteractTime() <= 0.0f)
		{
			MakeHotCoffee();
			break;
		}
	}

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

		//インタラクトし続けて一定時間経ったらコーヒーを出力する
		if (obj->GetInteractTime() <= 0.0f)
		{
			LidFollowCup();
			break;
		}
	}

#ifdef _DEBUG

	//ImGuiの操作を行う
	//UpdateDebugImGui();

#endif // _DEBUG

	transform_.Update();
	sphereTran_.Update();
}

void StageManager::Draw(void)
{
	//モデルの描画

	MV1DrawModel(transform_.modelId);
	//DrawSphere3D(sphereTran_.pos, 30, 8, 0xff0000, 0xff0000, false);

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

	int line = 8;	//行
	int lineHeight = 30;	//行
	DebugDrawFormat::FormatString(L"item : %s", StringUtility::StringToWstring(player_.GetHoldItem()).c_str(), line, lineHeight);
	DebugDrawFormat::FormatString(L"hold : %d", player_.GetIsHolding(), line, lineHeight);

	//size_t size = objects_.size();
	////蓋生成数確認用
	//DebugDrawFormat::FormatString(L"end - 2 : %s",
	//	StringUtility::StringToWstring(objects_[size - 3]->GetObjectId()).c_str(), line, lineHeight);
	//
	//DebugDrawFormat::FormatString(L"end - 1 : %s",
	//	StringUtility::StringToWstring(objects_[size - 2]->GetObjectId()).c_str(), line, lineHeight);

	//DebugDrawFormat::FormatString(L"end : %s",
	//	StringUtility::StringToWstring(objects_.back()->GetObjectId()).c_str(), line, lineHeight);

	DebugDrawFormat::FormatString(L"surveD : %d",
		surveDrink_, line, lineHeight);
	
	DebugDrawFormat::FormatString(L"surveS : %d",
		surveSweets_, line, lineHeight);
	
	DebugDrawFormat::FormatString(L"surveL : %d",
		surveDrinkLid_, line, lineHeight);
	
	DebugDrawFormat::FormatString(L"surve : %d",
		isSurved_, line, lineHeight);

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

	for (int i = 0; i < tables_.size(); i++)
	{
		DebugDrawFormat::FormatString(L"table%d.placeable  : %d", i,
			tables_[i]->GetIsPlaceable(), line, lineHeight);
	}

	////テーブル番号を表示
	//for (int i = 0; i < TABLE_NUM + TABLE_NUM - 1; i++)
	//{
	//	VECTOR screenPos = ConvWorldPosToScreenPos(tables_[i]->GetTransform().pos);
	//	// 変換成功
	//	DrawFormatString(static_cast<int>(screenPos.x) - 25, static_cast<int>(screenPos.y) - 50, GetColor(255, 255, 255),
	//		L"%s : %d",
	//		StringUtility::StringToWstring(tables_[i]->GetObjectId().c_str()).c_str(), i);
	//}

	for (const auto& obj : objects_)
	{
		VECTOR screenPos = ConvWorldPosToScreenPos(obj->GetTransform().pos);
		// 変換成功
		DrawFormatString(static_cast<int>(screenPos.x) - 25, static_cast<int>(screenPos.y) - 50, GetColor(255, 255, 255),
			L"%s",
			StringUtility::StringToWstring(obj->GetObjectId().c_str()).c_str());
	}

#endif // _DEBUG
}

void StageManager::MakeHotCoffee(void)
{
	for (size_t i = 0; i < objects_.size(); ++i)
	{
		//ホット用カップ以外のオブジェクトは判定しない
		if (objects_[i]->GetObjectId() != HOT_CUP) continue;
			
		for (const auto& machine : objects_)
		{
			//マシンの判定だけさせたい
			if (machine->GetObjectId() != COFFEE_MACHINE)continue;

			//マシンの球体と当たっているカップだけ処理する
			if (AsoUtility::IsHitSpheres(machine->GetPos(), machine->GetSphereRad(),
				objects_[i]->GetSpherePos(), objects_[i]->GetSphereRad()))
			{
				//当たっているカップをコーヒーに上書きする
				objects_[i] = std::make_unique<HotCoffee>(HOT_COFFEE, 20.0f, 30.0f, 20.0f, player_);
				objects_[i]->Init();
				objects_[i]->ChangeItemState(StageObject::ITEM_STATE::PLACED);
				objects_[i]->SetPos(MACHINE_POS);
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
				objects_.emplace_back(std::make_unique<CupLid>(CUP_LID, 23.0f, 5.0f, 23.0f, player_,*objects_[i]));
				objects_.back()->Init();
				objects_.back()->Update();
				isCreate = true;
				break;
			}
		}
		if (isCreate)break;
	}
}

void StageManager::DeleteSurvedItem(void)
{
	// カウンターの球体と当たっているオブジェクトを削除
	for (auto it = objects_.begin(); it != objects_.end(); )
	{
		if (AsoUtility::IsHitSpheres(
			(*it)->GetSpherePos(), (*it)->GetSphereRad(),
			counter_->GetSpherePos(), counter_->GetSphereRad()))
		{
			// 必要なら提供処理などをここで呼ぶ
			ResetServeData(); // 提供データをリセット
			it = objects_.erase(it); // eraseは次の要素のイテレータを返す
		}
		else
		{
			++it;
		}
	}
}

void StageManager::ResetServeData(void)
{
	isSurved_ = false;
	surveDrink_ = Order::DRINK::NONE;
	surveSweets_ = Order::SWEETS::NONE;
	surveDrinkLid_ = false;
}

void StageManager::SurveItem(StageObject& obj)
{
	auto& ins = InputManager::GetInstance();

	isSurved_ = true;

	surveDrinkLid_ = obj.IsLidOn();

	if (obj.GetObjectId() == HOT_COFFEE)
	{
		surveDrink_ = Order::DRINK::HOT;
	}
	else if (obj.GetObjectId() == ICE_COFFEE)
	{
		surveDrink_ = Order::DRINK::ICE;
	}
	else
	{
		surveDrink_ = Order::DRINK::NONE;
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
