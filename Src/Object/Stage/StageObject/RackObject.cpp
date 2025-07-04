#include "../../../Manager/Generic/SceneManager.h"
#include "../../../Manager/Generic/InputManager.h"
#include "../../../Manager/Generic/ResourceManager.h"
#include "../../../Utility/AsoUtility.h"
#include "../../Common/Sphere.h"
#include "../Object/Player.h"
#include "../../UI/GaugeUI.h"
#include "ItemObject.h"
#include "RackObject.h"

RackObject::RackObject(const std::string objId,
	const float width, const float height,
	const float depth, Player& player) :
	StageObject(objId, width, height, depth, player)
{
	sweetsStockCnt_ = SWEETS_STOCK_MAX;
	cupsStockCnt_ = CUP_STOCK_MAX;
	hasStock_ = true;
}

void RackObject::PickUp(std::string rackName,std::vector<std::unique_ptr<StageObject>>& object)
{
	if (!hasStock_)return;

	auto& ins = InputManager::GetInstance();
	//ホットカップ用ラックにインタラクトしたときの処理
	if (rackName == "Cup_Hot_Rack" && ins.IsInputTriggered("Interact"))
	{
		object.emplace_back(std::make_unique<ItemObject>("Hot_Cup", 40.0f, 30.0f, 40.0f, player_));
		object.back()->Init(player_.GetSphere().GetPos());
		player_.SetHoldItem(object.back()->GetParam().id_);
		object.back()->ChangeItemState(ITEM_STATE::HOLD);
		isActioned_ = true;
		cupsStockCnt_--;
	}

	//アイスカップ用ラックにインタラクトしたときの処理
	if (rackName == "Cup_Ice_Rack" && ins.IsInputTriggered("Interact"))
	{
		object.emplace_back(std::make_unique<ItemObject>("Ice_Cup", 40.0f, 30.0f, 40.0f, player_));
		object.back()->Init(player_.GetSphere().GetPos());
		player_.SetHoldItem(object.back()->GetParam().id_);
		object.back()->ChangeItemState(ITEM_STATE::HOLD);
		isActioned_ = true;
		cupsStockCnt_--;
	}

	//スイーツ（ベリー）用ラックにインタラクトしたときの処理
	if (rackName == "Sweets_Strawberry_Rack" && ins.IsInputTriggered("Interact"))
	{
		//スイーツを取り出す
		object.emplace_back(std::make_unique<ItemObject>("Sweets_Strawberry", 40.0f, 30.0f, 40.0f, player_));
		object.back()->Init(player_.GetSphere().GetPos());
		object.back()->SetScale({ 1.2f,1.2f,1.2f }); // スイーツのサイズを調整
		player_.SetHoldItem(object.back()->GetParam().id_);
		object.back()->ChangeItemState(ITEM_STATE::HOLD);
		isActioned_ = true;
		sweetsStockCnt_--;
	}

	//スイーツ（チョコ）用ラックにインタラクトしたときの処理
	if (rackName == "Sweets_Choco_Rack" && ins.IsInputTriggered("Interact"))
	{
		//スイーツを取り出す
		object.emplace_back(std::make_unique<ItemObject>("Sweets_Choco", 40.0f, 30.0f, 40.0f, player_));
		object.back()->Init(player_.GetSphere().GetPos());
		object.back()->SetScale({ 1.2f,1.2f,1.2f }); // スイーツのサイズを調整
		player_.SetHoldItem(object.back()->GetParam().id_);
		object.back()->ChangeItemState(ITEM_STATE::HOLD);
		isActioned_ = true;
		sweetsStockCnt_--;
	}
}

void RackObject::AddStock(int addStockNum)
{
	if (hasStock_) return;
	
	auto& ins = InputManager::GetInstance();

	if(sweetsStockCnt_ >= SWEETS_STOCK_MAX &&
		cupsStockCnt_ >= CUP_STOCK_MAX)
	{
		hasStock_ = true;
		gaugeUI_->Reset();
		return;
	}

	if (ins.IsInputPressed("Interact"))
	{
		gaugeUI_->SetActive(true);
		addInterval_ += SceneManager::GetInstance().GetDeltaTime();
		gaugeUI_->Update();
	}

	if (addInterval_ > SWEETS_ADD_INTERVAL)
	{
		addInterval_ = 0.0f;
		sweetsStockCnt_++;
		cupsStockCnt_++;
	}
}

void RackObject::Init(VECTOR pos, float rotY)
{
	StageObject::Init(pos, rotY);

	//UIの初期化
	gaugeUI_ = std::make_unique<GaugeUI>(false, SWEETS_ADD_INTERVAL * SWEETS_STOCK_MAX);
	gaugeUI_->Init();
	gaugeUI_->SetUISize(50.0f, 10.0f);
	VECTOR uiPos = transform_.pos;
	uiPos.y -= 30.0f;	//UIの位置を調整
	gaugeUI_->SetPos(uiPos); // UIの位置を設定

	//文字列をSRCに変換してモデル設定
	ResourceManager::SRC srcType = ResourceManager::SRC::NONE;
	if (param_.id_ == "Sweets_Strawberry_Rack")
	{
		srcType = ResourceManager::SRC::SWEETS_BERRY; // デフォルトのラックIDを設定
	}
	else if (param_.id_ == "Sweets_Choco_Rack")
	{
		srcType = ResourceManager::SRC::SWEETS_CHOCO; // チョコレートラックIDを設定
	}

	if (srcType != ResourceManager::SRC::NONE)
	{
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
			sweetsOfRack_[i].scl = AsoUtility::VECTOR_ONE;
			VECTOR sweetsPos = VAdd(pos, sweetsOffsets[i]);
			sweetsOfRack_[i].pos = AsoUtility::RotXZPos(transform_.pos, sweetsPos, rotY);
			sweetsOfRack_[i].quaRot = Quaternion();
			sweetsOfRack_[i].quaRotLocal =
				Quaternion::Euler({ AsoUtility::Deg2RadF(SWEETS_ROT_X),AsoUtility::Deg2RadF(rotY), 0.0f });
		}
	}

	if (srcType != ResourceManager::SRC::NONE)return;

	//設定されていなかったらカップモデルを設定する
	if (param_.id_ == "Cup_Hot_Rack")
	{
		srcType = ResourceManager::SRC::HOTCUP; // ホットカップラックIDを設定
	}
	else if (param_.id_ == "Cup_Ice_Rack")
	{
		srcType = ResourceManager::SRC::ICECUP; // アイスカップラックIDを設定
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
		cupesOfRack_[i].scl = AsoUtility::VECTOR_ONE;
		VECTOR cupsPos = VAdd(pos, cupOffsets[i]);
		cupesOfRack_[i].pos = AsoUtility::RotXZPos(transform_.pos, cupsPos, AsoUtility::Deg2RadF(rotY));
		cupesOfRack_[i].quaRot = Quaternion();
		cupesOfRack_[i].quaRotLocal =
			Quaternion::Euler({ AsoUtility::Deg2RadF(180.0f), AsoUtility::Deg2RadF(rotY), 0.0f });
	}

}

void RackObject::Update(void)
{
	if(sweetsStockCnt_ <= 0 || cupsStockCnt_ <= 0)
	{
		hasStock_ = false;
	}

	for(auto& sweets : sweetsOfRack_)
	{
		sweets.Update();
	}

	for(auto& cups : cupesOfRack_)
	{
		cups.Update();
	}

	StageObject::Update();
}

void RackObject::Draw(void)
{
	//int line = 3;	//行
	//int lineHeight = 30;	//行

	//VECTOR screenPos = ConvWorldPosToScreenPos(GetTransform().pos);
	//// 変換成功
	//DrawFormatString(static_cast<int>(screenPos.x) - 30,
	//	static_cast<int>(screenPos.y) - 100, GetColor(255, 255, 255),
	//	L"在庫 %d", sweetsStockCnt_);

	StageObject::Draw();

	for (int i = 0; i < cupsStockCnt_; ++i)
	{
		//モデルの描画
		MV1DrawModel(cupesOfRack_[i].modelId);
	}

	for (int i = 0; i < sweetsStockCnt_; ++i)
	{
		//モデルの描画
		MV1DrawModel(sweetsOfRack_[i].modelId);
	}

	gaugeUI_->Draw(); // UIを描画
}
