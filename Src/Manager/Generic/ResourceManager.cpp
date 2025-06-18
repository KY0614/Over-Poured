#include <DxLib.h>
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

	//足煙
	res = std::make_unique<RES>(RES_T::EFFEKSEER, PATH_EFF + "Smoke/Smoke.efkefc");
	resourcesMap_.emplace(SRC::FOOT_SMOKE, std::move(res));
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
	//推奨しませんが、どうしても使いたい方は
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

	//仮ステージ
	res = std::make_unique<RES>(RES_T::MODEL, PATH_MDL + "Stage/kari_stage.mv1");
	resourcesMap_.emplace(SRC::STAGE, std::move(res));

	//カウンター
	res = std::make_unique<RES>(RES_T::MODEL, PATH_MDL + "Stage/counter.mv1");
	resourcesMap_.emplace(SRC::COUNTER, std::move(res));

	//レジスター
	res = std::make_unique<RES>(RES_T::MODEL, PATH_MDL + "Stage/register.mv1");
	resourcesMap_.emplace(SRC::REGISTER, std::move(res));

	//仮客
	res = std::make_unique<RES>(RES_T::MODEL, PATH_MDL + "Customer/Model.mv1");
	resourcesMap_.emplace(SRC::CUSTOMER, std::move(res));
}

void ResourceManager::InitResult(void)
{
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
