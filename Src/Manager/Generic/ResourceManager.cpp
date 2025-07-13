#include <DxLib.h>
#include <unordered_map>
#include <map>
#include "../../Application.h"
#include "Resource.h"
#include "ResourceManager.h"

ResourceManager* ResourceManager::instance_ = nullptr;

void ResourceManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new ResourceManager();
	}
	instance_->Init();
}

ResourceManager& ResourceManager::GetInstance(void)
{
	return *instance_;
}

void ResourceManager::Init(void)
{

}

void ResourceManager::Release(void)
{
	for (auto& p : loadedMap_)
	{
		p.second.Release();
	}

	loadedMap_.clear();
	resourcesMap_.clear();
}

void ResourceManager::Destroy(void)
{
	Release();
	resourcesMap_.clear();
	delete instance_;
}

const Resource& ResourceManager::Load(SRC src)
{
	Resource& res = _Load(src);
	if (res.type_ == Resource::TYPE::NONE)
	{
		return dummy_;
	}
	return res;
}

int ResourceManager::LoadModelDuplicate(SRC src)
{
	Resource& res = _Load(src);
	if (res.type_ == Resource::TYPE::NONE)
	{
		return -1;
	}

	int duId = MV1DuplicateModel(res.handleId_);
	res.duplicateModelIds_.push_back(duId);

	return duId;
}

ResourceManager::SRC ResourceManager::StringToSRC(const std::string& name)
{
	static const std::unordered_map<std::string, SRC> map = {
		{ "Counter", SRC::COUNTER },
		{ "Table",   SRC::TABLE },
		{ "Sweets_Choco_Rack",  SRC::CHOCO_RACK },
		{ "Sweets_Choco",		SRC::SWEETS_CHOCO },
		{ "Sweets_Strawberry_Rack",   SRC::BERRY_RACK },
		{ "Sweets_Strawberry",  SRC::SWEETS_BERRY },
		{ "Coffee_Machine",   SRC::COFFEE_MACHINE },
		{ "Hot_Cup",		SRC::HOTCUP },
		{ "Cup_Hot_Rack",   SRC::HOTCUP_RACK },
		{ "Hot_Coffee",		SRC::HOTCOFFEE },
		{ "Ice_Dispenser",			SRC::ICEDISPENSER },
		{ "Ice",			SRC::ICE },
		{ "Ice_Cup",		SRC::ICECUP },
		{ "Cup_Ice_Rack",	SRC::ICECUP_RACK },
		{ "Ice_Coffee",		SRC::ICECOFFEE },
		{ "Cup_Lid_Rack",	SRC::CUPLID_RACK },
		{ "Hot_Cup_Lid",		SRC::HOTCUP_LID },
		{ "Ice_Cup_Lid",		SRC::ICECUP_LID },
		{ "Dust_Box",		SRC::DUSTBOX },
		// 新しい要素はここに追加
	};

	auto it = map.find(name);
	if (it != map.end()) {
		return it->second;
	}

	return SRC::NONE; // 不正な名前が来たときのデフォルト対応
}

void ResourceManager::InitTitle(void)
{
	//推奨しませんが、どうしても使いたい方は
	using RES = Resource;
	using RES_T = RES::TYPE;
	static std::string PATH_IMG = Application::PATH_IMAGE;
	static std::string PATH_MDL = Application::PATH_MODEL;
	static std::string PATH_EFF = Application::PATH_EFFECT;

	std::unique_ptr<Resource> res;

	//PushSpace
	res = std::make_unique<RES>(RES_T::IMG, PATH_IMG + "PushSpace.png");
	resourcesMap_.emplace(SRC::PUSH_SPACE, std::move(res));

	//プレイヤー
	res = std::make_unique<RES>(RES_T::MODEL, PATH_MDL + "Player/Player.mv1");
	resourcesMap_.emplace(SRC::PLAYER, std::move(res));

	//スカイドーム
	res = std::make_unique<RES>(RES_T::MODEL, PATH_MDL + "SkyDome/SkyDome.mv1");
	resourcesMap_.emplace(SRC::SKY_DOME, std::move(res));
}

void ResourceManager::InitMovie(void)
{
}

void ResourceManager::InitSelect(void)
{
}

void ResourceManager::InitTutorial(void)
{
}

void ResourceManager::InitGame(void)
{
	using RES = Resource;
	using RES_T = RES::TYPE;
	static std::string PATH_IMG = Application::PATH_IMAGE;
	static std::string PATH_MDL = Application::PATH_MODEL;
	static std::string PATH_EFF = Application::PATH_EFFECT;

	std::unique_ptr<Resource> res;

	//プレイヤー
	res = std::make_unique<RES>(RES_T::MODEL, PATH_MDL + "Player/Player.mv1");
	resourcesMap_.emplace(SRC::PLAYER, std::move(res));

	//プレイヤー影
	res = std::make_unique<RES>(RES_T::IMG, PATH_IMG + "Shadow.png");
	resourcesMap_.emplace(SRC::PLAYER_SHADOW, std::move(res));

	//足煙
	res = std::make_unique<RES>(RES_T::EFFEKSEER, PATH_EFF + "Smoke/Smoke.efkefc");
	resourcesMap_.emplace(SRC::FOOT_SMOKE, std::move(res));

	//家具：床
	res = std::make_unique<RES>(RES_T::MODEL, PATH_MDL + "Stage/floor.mv1");
	resourcesMap_.emplace(SRC::FLOOR, std::move(res));

	//家具：植木鉢
	res = std::make_unique<RES>(RES_T::MODEL, PATH_MDL + "Stage/plant.mv1");
	resourcesMap_.emplace(SRC::PLANT, std::move(res));

	//家具：棚
	res = std::make_unique<RES>(RES_T::MODEL, PATH_MDL + "Stage/shelves.mv1");
	resourcesMap_.emplace(SRC::SHELVES, std::move(res));

	//家具：棚
	res = std::make_unique<RES>(RES_T::MODEL, PATH_MDL + "Stage/shelves_front.mv1");
	resourcesMap_.emplace(SRC::SHELVES_FRONT, std::move(res));

	//家具：机ら
	res = std::make_unique<RES>(RES_T::MODEL, PATH_MDL + "Stage/desk.mv1");
	resourcesMap_.emplace(SRC::DESK, std::move(res));

	//カウンター
	res = std::make_unique<RES>(RES_T::MODEL, PATH_MDL + "Stage/counter.mv1");
	resourcesMap_.emplace(SRC::COUNTER, std::move(res));

	//テーブル
	res = std::make_unique<RES>(RES_T::MODEL, PATH_MDL + "Stage/table.mv1");
	resourcesMap_.emplace(SRC::TABLE, std::move(res));
	
	//テーブル用コライダー
	res = std::make_unique<RES>(RES_T::MODEL, PATH_MDL + "Stage/table_bounds.mv1");
	resourcesMap_.emplace(SRC::TABLE_COL, std::move(res));

	//レジスター
	res = std::make_unique<RES>(RES_T::MODEL, PATH_MDL + "Stage/register.mv1");
	resourcesMap_.emplace(SRC::REGISTER, std::move(res));

	//チョコ用ラック
	res = std::make_unique<RES>(RES_T::MODEL, PATH_MDL + "Stage/SweetsRack.mv1");
	resourcesMap_.emplace(SRC::CHOCO_RACK, std::move(res));

	//スイーツ(チョコ)
	res = std::make_unique<RES>(RES_T::MODEL, PATH_MDL + "Stage/ChocoSweets.mv1");
	resourcesMap_.emplace(SRC::SWEETS_CHOCO, std::move(res));

	//ベリー用ラック
	res = std::make_unique<RES>(RES_T::MODEL, PATH_MDL + "Stage/SweetsRack.mv1");
	resourcesMap_.emplace(SRC::BERRY_RACK, std::move(res));

	//スイーツ（ベリー）
	res = std::make_unique<RES>(RES_T::MODEL, PATH_MDL + "Stage/BerrySweets.mv1");
	resourcesMap_.emplace(SRC::SWEETS_BERRY, std::move(res));

	//スイーツショーケース
	res = std::make_unique<RES>(RES_T::MODEL, PATH_MDL + "Stage/sweets_case.mv1");
	resourcesMap_.emplace(SRC::SWEETS_CASE, std::move(res));

	//コーヒーマシン
	res = std::make_unique<RES>(RES_T::MODEL, PATH_MDL + "Stage/coffee_machine.mv1");
	resourcesMap_.emplace(SRC::COFFEE_MACHINE, std::move(res));

	//ホット用カップ
	res = std::make_unique<RES>(RES_T::MODEL, PATH_MDL + "Stage/hotcup.mv1");
	resourcesMap_.emplace(SRC::HOTCUP, std::move(res));

	//ホットコーヒー
	res = std::make_unique<RES>(RES_T::MODEL, PATH_MDL + "Stage/hotcoffee.mv1");
	resourcesMap_.emplace(SRC::HOTCOFFEE, std::move(res));

	//ホット用カップのラック
	res = std::make_unique<RES>(RES_T::MODEL, PATH_MDL + "Stage/hotcupRack.mv1");
	resourcesMap_.emplace(SRC::HOTCUP_RACK, std::move(res));

	//アイス用カップ
	res = std::make_unique<RES>(RES_T::MODEL, PATH_MDL + "Stage/icecup.mv1");
	resourcesMap_.emplace(SRC::ICECUP, std::move(res));

	//アイスコーヒー
	res = std::make_unique<RES>(RES_T::MODEL, PATH_MDL + "Stage/icecoffee.mv1");
	resourcesMap_.emplace(SRC::ICECOFFEE, std::move(res));

	//アイス用カップのラック
	res = std::make_unique<RES>(RES_T::MODEL, PATH_MDL + "Stage/icecupRack.mv1");
	resourcesMap_.emplace(SRC::ICECUP_RACK, std::move(res));

	//製氷機
	res = std::make_unique<RES>(RES_T::MODEL, PATH_MDL + "Stage/icedispenser.mv1");
	resourcesMap_.emplace(SRC::ICEDISPENSER, std::move(res));

	//氷
	res = std::make_unique<RES>(RES_T::MODEL, PATH_MDL + "Stage/ice.mv1");
	resourcesMap_.emplace(SRC::ICE, std::move(res));

	//カップの蓋のラック
	res = std::make_unique<RES>(RES_T::MODEL, PATH_MDL + "Stage/cuplidRack.mv1");
	resourcesMap_.emplace(SRC::CUPLID_RACK, std::move(res));

	//カップの蓋(ホット）
	res = std::make_unique<RES>(RES_T::MODEL, PATH_MDL + "Stage/hotcupLid.mv1");
	resourcesMap_.emplace(SRC::HOTCUP_LID, std::move(res));

	//カップの蓋(アイス）
	res = std::make_unique<RES>(RES_T::MODEL, PATH_MDL + "Stage/icecupLid.mv1");
	resourcesMap_.emplace(SRC::ICECUP_LID, std::move(res));

	//ゴミ箱
	res = std::make_unique<RES>(RES_T::MODEL, PATH_MDL + "Stage/dustbox.mv1");
	resourcesMap_.emplace(SRC::DUSTBOX, std::move(res));

	//ゲージUI
	res = std::make_unique<RES>(RES_T::IMG, PATH_IMG + "bar_green.png");
	resourcesMap_.emplace(SRC::UI_BAR, std::move(res));

	//ゲージ影UI
	res = std::make_unique<RES>(RES_T::IMG, PATH_IMG + "bar_shadow.png");
	resourcesMap_.emplace(SRC::UI_BARSHADOW, std::move(res));

	//ゲージUI
	res = std::make_unique<RES>(RES_T::IMG, PATH_IMG + "CircleExpGage.png");
	resourcesMap_.emplace(SRC::UI_CIRCLE, std::move(res));

	//ゲージ影UI
	res = std::make_unique<RES>(RES_T::IMG, PATH_IMG + "CircleGage.png");
	resourcesMap_.emplace(SRC::UI_CIRCLESHADOW, std::move(res));

	//タイマー用の数字
	res = std::make_unique<RES>(RES_T::IMGS, PATH_IMG + "Number.png",
		NUMBER_NUM_X, NUMBER_NUM_Y, NUMBER_SIZE_X, NUMBER_SIZE_Y);
	resourcesMap_.emplace(SRC::TIMER_NUMBER, std::move(res));

	//仮客
	res = std::make_unique<RES>(RES_T::MODEL, PATH_MDL + "Customer/hotcup_customer.mv1");
	resourcesMap_.emplace(SRC::HOT_CUSTOMER, std::move(res));
	//仮客
	res = std::make_unique<RES>(RES_T::MODEL, PATH_MDL + "Customer/icecup_customer.mv1");
	resourcesMap_.emplace(SRC::ICE_CUSTOMER, std::move(res));
}

void ResourceManager::InitResult(void)
{
	using RES = Resource;
	using RES_T = RES::TYPE;
	static std::string PATH_IMG = Application::PATH_IMAGE;
	static std::string PATH_MDL = Application::PATH_MODEL;
	static std::string PATH_EFF = Application::PATH_EFFECT;

	std::unique_ptr<Resource> res;

	//円（C)
	res = std::make_unique<RES>(RES_T::IMG, PATH_IMG + "Circle_C.png");
	resourcesMap_.emplace(SRC::RANK_C, std::move(res));

	//円（B)
	res = std::make_unique<RES>(RES_T::IMG, PATH_IMG + "Circle_B.png");
	resourcesMap_.emplace(SRC::RANK_B, std::move(res));

	//円（A)
	res = std::make_unique<RES>(RES_T::IMG, PATH_IMG + "Circle_A.png");
	resourcesMap_.emplace(SRC::RANK_A, std::move(res));

	//円（S)
	res = std::make_unique<RES>(RES_T::IMG, PATH_IMG + "Circle_S.png");
	resourcesMap_.emplace(SRC::RANK_S, std::move(res));

	//ゲージ影UI
	res = std::make_unique<RES>(RES_T::IMG, PATH_IMG + "CircleGage.png");
	resourcesMap_.emplace(SRC::UI_CIRCLESHADOW, std::move(res));

}

ResourceManager::ResourceManager(void)
{
}

Resource& ResourceManager::_Load(SRC src)
{

	//ロード済みチェック
	const auto& lPair = loadedMap_.find(src);
	if (lPair != loadedMap_.end())
	{
		return lPair->second;
	}

	//リソース登録チェック
	const auto& rPair = resourcesMap_.find(src);
	if (rPair == resourcesMap_.end())
	{
		//登録されていない
		return dummy_;
	}

	//ロード処理
	rPair->second->Load();

	//念のためコピーコンストラクタ
	loadedMap_.emplace(src, *rPair->second);

	return *rPair->second;

}
