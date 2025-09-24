#include "../../../Manager/GameSystem/SoundManager.h"
#include "../../../Manager/Generic/SceneManager.h"
#include "../../../Manager/Generic/InputManager.h"
#include "../../../Manager/Generic/ResourceManager.h"
#include "../../../Utility/CommonUtility.h"
#include "../../Common/Sphere.h"
#include "../Object/Player.h"
#include "../../UI/GaugeUI.h"
#include "../../UI/IconUI.h"
#include "../../UI/UIManager.h"
#include "ItemObject.h"
#include "RackObject.h"

namespace 
{
	//スイーツの大きさ
	const float SWEETS_SCALE = 1.2f;				
}

RackObject::RackObject(const std::string objId,Player& player) :
	StageObject(objId, player)
{
	sweetsStockCnt_ = SWEETS_STOCK_MAX;
	cupsStockCnt_ = CUP_STOCK_MAX;
	hasStock_ = true;
	addInterval_ = 0.0f;
}

void RackObject::PickUp(std::string rackName,std::vector<std::unique_ptr<StageObject>>& object)
{
	if (!hasStock_)return;
	//isInteract_ = true;
	iconUI_->SetActive(true);
	auto& ins = InputManager::GetInstance();
	auto& sound = SoundManager::GetInstance();
	//ホットカップ用ラックにインタラクトしたときの処理
	if (rackName == HOT_CUP_RACK && ins.IsInputTriggered("Interact"))
	{
		//SE再生
		sound.Play(SoundManager::SOUND::PICK_UP);
		//ホットカップを取り出す
		object.emplace_back(std::make_unique<ItemObject>(HOT_CUP,player_));
		object.back()->Init(player_.GetSphere().GetPos());
		player_.SetHoldItem(object.back()->GetParam().id_);
		object.back()->ChangeItemState(ITEM_STATE::HOLD);	//持っている状態に変更
		isActioned_ = true;
		cupsStockCnt_--;	//在庫を減らす
	}

	//アイスカップ用ラックにインタラクトしたときの処理
	if (rackName == ICE_CUP_RACK && ins.IsInputTriggered("Interact"))
	{
		//SE再生
		sound.Play(SoundManager::SOUND::PICK_UP);
		//アイスカップを取り出す
		object.emplace_back(std::make_unique<ItemObject>(ICE_CUP, player_));
		object.back()->Init(player_.GetSphere().GetPos());
		player_.SetHoldItem(object.back()->GetParam().id_);
		object.back()->ChangeItemState(ITEM_STATE::HOLD);	//持っている状態に変更
		isActioned_ = true;
		cupsStockCnt_--;	//在庫を減らす
	}

	//スイーツ（ベリー）用ラックにインタラクトしたときの処理
	if (rackName == BERRY_SWEETS_RACK && ins.IsInputTriggered("Interact"))
	{
		//SE再生
		sound.Play(SoundManager::SOUND::PICK_UP);
		//スイーツを取り出す
		object.emplace_back(std::make_unique<ItemObject>(BERRY_SWEETS, player_));
		object.back()->Init(player_.GetSphere().GetPos());
		object.back()->SetScale({ SWEETS_SCALE,SWEETS_SCALE,SWEETS_SCALE }); // スイーツのサイズを調整
		player_.SetHoldItem(object.back()->GetParam().id_);
		object.back()->ChangeItemState(ITEM_STATE::HOLD);	//持っている状態に変更
		isActioned_ = true;
		sweetsStockCnt_--;	//在庫を減らす
	}

	//スイーツ（チョコ）用ラックにインタラクトしたときの処理
	if (rackName == CHOCO_SWEETS_RACK && ins.IsInputTriggered("Interact"))
	{
		sound.Play(SoundManager::SOUND::PICK_UP);
		//スイーツを取り出す
		object.emplace_back(std::make_unique<ItemObject>(CHOCO_SWEETS, player_));
		object.back()->Init(player_.GetSphere().GetPos());
		object.back()->SetScale({ SWEETS_SCALE,SWEETS_SCALE,SWEETS_SCALE }); // スイーツのサイズを調整
		player_.SetHoldItem(object.back()->GetParam().id_);
		object.back()->ChangeItemState(ITEM_STATE::HOLD);	//持っている状態に変更
		isActioned_ = true;
		sweetsStockCnt_--;	//在庫を減らす
	}
}

void RackObject::AddStock(void)
{
	//在庫がある場合は処理しない
	if (hasStock_) return;
	
	auto& ins = InputManager::GetInstance();
	auto& sound = SoundManager::GetInstance();
	if (ins.IsInputPressed("Interact"))
	{
		gaugeUI_->SetActive(true);
		addInterval_ += SceneManager::GetInstance().GetDeltaTime();
		gaugeUI_->Update();
	}
	//スイーツの在庫追加処理
	if(GetParam().id_ == BERRY_SWEETS_RACK ||
		GetParam().id_ == CHOCO_SWEETS_RACK)
	{
		//スイーツの在庫を追加
		if (addInterval_ > ADD_INTERVAL)
		{
			addInterval_ = 0.0f;
			sweetsStockCnt_++;
			sound.Play(SoundManager::SOUND::ADD_STOCK);
		}
		//スイーツの在庫が最大に達したら、在庫ありに設定
		if (sweetsStockCnt_ >= SWEETS_STOCK_MAX)
		{
			hasStock_ = true;
			gaugeUI_->Reset();
			stockIconUI_->SetActive(false);
			return;
		}
	}//カップの在庫追加処理
	else if(GetParam().id_ == HOT_CUP_RACK ||
			GetParam().id_ == ICE_CUP_RACK)
	{
		//カップの在庫を追加
		if (addInterval_ > ADD_INTERVAL)
		{
			addInterval_ = 0.0f;
			cupsStockCnt_++;
			sound.Play(SoundManager::SOUND::ADD_STOCK);
		}
		//カップの在庫が最大に達したら、在庫ありに設定
		if (cupsStockCnt_ >= CUP_STOCK_MAX)
		{
			hasStock_ = true;
			gaugeUI_->Reset();
			stockIconUI_->SetActive(false);
			return;
		}
	}
}

void RackObject::Init(VECTOR pos, float rotY, VECTOR scale)
{
	StageObject::Init(pos, rotY, scale);
	//UIのサイズと位置
	const float iconUISize = 70.0f;	//UIのサイズ
	const VECTOR iconUIPos = { 0.0f, 60.0f, 0.0f };	//UIの位置
	//スイーツ用UIの初期化
	iconUI_ = std::make_unique<IconUI>(iconUIPos,
		transform_.pos, ResourceManager::SRC::INTERACT);
	iconUI_->SetActive(false);
	iconUI_->Init();
	iconUI_->SetUISize(iconUISize);
	UIManager::GetInstance().AddIconUI(iconUI_.get());	//UI管理に追加

	stockIconUI_ = std::make_unique<IconUI>(iconUIPos,
	transform_.pos, ResourceManager::SRC::STOCK_ICON);
	stockIconUI_->SetActive(false);
	stockIconUI_->Init();
	stockIconUI_->SetUISize(iconUISize);
	UIManager::GetInstance().AddIconUI(stockIconUI_.get());	//UI管理に追加

	//文字列をSRCに変換してモデル設定
	ResourceManager::SRC srcType = ResourceManager::SRC::NONE;
	if (param_.id_ == BERRY_SWEETS_RACK)
	{
		srcType = ResourceManager::SRC::SWEETS_BERRY; //ベリーのラックIDを設定
		cupsStockCnt_ = 0;	//カップの在庫を0にする
	}
	else if (param_.id_ == CHOCO_SWEETS_RACK)
	{
		srcType = ResourceManager::SRC::SWEETS_CHOCO; //チョコレートラックIDを設定
		cupsStockCnt_ = 0;	//カップの在庫を0にする
	}

	if (srcType != ResourceManager::SRC::NONE)
	{
		//ゲージUIのサイズ
		const float sweetsGaugeWidth = 50.0f;	//UIの幅	
		const float sweetsGaugeHeight = 5.0f;	//UIの高さ
		//スイーツ用UIの初期化
		gaugeUI_ = std::make_unique<GaugeUI>(false, ADD_INTERVAL * SWEETS_STOCK_MAX);
		gaugeUI_->Init();
		gaugeUI_->SetUISize(sweetsGaugeWidth, sweetsGaugeHeight);
		VECTOR uiPos = transform_.pos;
		uiPos.y += SWEETS_UI_OFFSET_Y;	//UIの位置を調整
		gaugeUI_->SetPos(uiPos); // UIの位置を設定
		UIManager::GetInstance().AddGaugeUI(gaugeUI_.get());	//UI管理に追加

		// 各スイーツの基準座標からのオフセットを配列で定義
		const VECTOR sweetsOffsets[] = {
			{SWEETS_HALF_WIDTH,  SWEETS_HEIGHT_OFFSET, SWEETS_Z_BACK_OFFSET},
			{-SWEETS_HALF_WIDTH,  SWEETS_HEIGHT_OFFSET, SWEETS_Z_BACK_OFFSET},
			{-SWEETS_HALF_WIDTH,  SWEETS_HEIGHT_OFFSET,  SWEETS_Z_FRONT_OFFSET},
			{SWEETS_HALF_WIDTH,  SWEETS_HEIGHT_OFFSET,  SWEETS_Z_FRONT_OFFSET}
		};

		for (int i = 0; i < sweetsStockCnt_; ++i)
		{
			//モデルの基本設定
			sweetsOfRack_[i].SetModel(ResourceManager::GetInstance().LoadModelDuplicate(srcType));
			sweetsOfRack_[i].scl = CommonUtility::VECTOR_ONE;
			VECTOR sweetsPos = VAdd(pos, sweetsOffsets[i]);
			VECTOR rotPos = CommonUtility::RotXZPos(transform_.pos, sweetsPos, CommonUtility::Deg2RadF(rotY));
			sweetsOfRack_[i].pos = rotPos;
			sweetsOfRack_[i].quaRot = Quaternion();
			sweetsOfRack_[i].quaRotLocal =
				Quaternion::Euler({ CommonUtility::Deg2RadF(SWEETS_ROT_X),CommonUtility::Deg2RadF(rotY), 0.0f });
			sweetsOfRack_[i].Update();
		}
	}

	if (srcType != ResourceManager::SRC::NONE)return;

	const float cupGaugeWidth = 70.0f;	//UIの幅
	const float cupGaugeHeight = 10.0f;	//UIの高さ
	//カップ用UIの初期化
	gaugeUI_ = std::make_unique<GaugeUI>(false, ADD_INTERVAL * CUP_STOCK_MAX);
	gaugeUI_->Init();
	gaugeUI_->SetUISize(cupGaugeWidth, cupGaugeHeight);
	VECTOR uiPos = transform_.pos;
	uiPos.y -= CUP_UI_OFFSET_Y;	//UIの位置を調整
	gaugeUI_->SetPos(uiPos); // UIの位置を設定
	UIManager::GetInstance().AddGaugeUI(gaugeUI_.get());	//UI管理に追加

	//設定されていなかったらカップモデルを設定する
	if (param_.id_ == HOT_CUP_RACK)
	{
		srcType = ResourceManager::SRC::HOTCUP; // ホットカップラックIDを設定
		sweetsStockCnt_ = 0;
	}
	else if (param_.id_ == ICE_CUP_RACK)
	{
		srcType = ResourceManager::SRC::ICECUP; // アイスカップラックIDを設定
		sweetsStockCnt_ = 0;
	}

	// 各カップの基準座標からのオフセットを配列で定義
	float cupHeightOffset = 8.0f;
	const VECTOR cupOffsets[] = {
		{CUPS_HALF_WIDTH,  CUPS_HEIGHT_OFFSET,  CUPS_Z_OFFSET},
		{-CUPS_HALF_WIDTH, CUPS_HEIGHT_OFFSET,  CUPS_Z_OFFSET},
		//2段目のカップ
		{CUPS_HALF_WIDTH,  CUPS_HEIGHT_OFFSET + cupHeightOffset,  CUPS_Z_OFFSET},
		{-CUPS_HALF_WIDTH, CUPS_HEIGHT_OFFSET + cupHeightOffset,  CUPS_Z_OFFSET},
		//3段目のカップ								倍ずらす
		{CUPS_HALF_WIDTH,  CUPS_HEIGHT_OFFSET + (cupHeightOffset * 2.0f),  CUPS_Z_OFFSET}
	};

	for (int i = 0; i < cupsStockCnt_; ++i)
	{
		//モデルの基本設定
		cupesOfRack_[i].SetModel(ResourceManager::GetInstance().LoadModelDuplicate(srcType));
		cupesOfRack_[i].scl = CommonUtility::VECTOR_ONE;
		VECTOR cupsPos = VAdd(pos, cupOffsets[i]);
		VECTOR rotPos = CommonUtility::RotXZPos(transform_.pos, cupsPos, CommonUtility::Deg2RadF(rotY));
		cupesOfRack_[i].pos = rotPos;
		cupesOfRack_[i].quaRot = Quaternion();
		cupesOfRack_[i].quaRotLocal =
			Quaternion::Euler({ CommonUtility::Deg2RadF(180.0f), CommonUtility::Deg2RadF(rotY), 0.0f });
		cupesOfRack_[i].Update();
	}
}

void RackObject::Update(void)
{
	//アイコンは非表示にしておく
	iconUI_->SetActive(false);
	//在庫がない場合は!マークのアイコンを表示
	if(sweetsStockCnt_ <= 0 && cupsStockCnt_ <= 0)
	{
		stockIconUI_->SetActive(true);
		hasStock_ = false;
	}
	//モデルの更新
	for(auto& sweets : sweetsOfRack_)
	{
		sweets.Update();
	}
	//モデルの更新
	for(auto& cups : cupesOfRack_)
	{
		cups.Update();
	}

	//通常の更新処理
	StageObject::Update();
}

void RackObject::Draw(void)
{
	//親クラスの描画
	StageObject::Draw();

	//ラックの中のカップモデルを描画
	for (int i = 0; i < cupsStockCnt_; ++i)
	{
		MV1DrawModel(cupesOfRack_[i].modelId);
	}
	//ラックの中のスイーツモデルを描画
	for (int i = 0; i < sweetsStockCnt_; ++i)
	{
		MV1DrawModel(sweetsOfRack_[i].modelId);
	}
}
