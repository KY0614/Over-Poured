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
		{ "Sweets_Strawberry",   SRC::SWEETS_BERRY },
		{ "Coffee_Machine",   SRC::COFFEE_MACHINE },
		{ "Hot_Cup",		SRC::HOTCUP },
		{ "Cup_Hot_Rack",   SRC::HOTCUP_RACK },
		{ "Hot_Coffee",   SRC::HOTCOFFEE },
		//{ "lid",     SRC::LID },
		// �V�����v�f�͂����ɒǉ�
	};

	auto it = map.find(name);
	if (it != map.end()) {
		return it->second;
	}

	return SRC::NONE; // �s���Ȗ��O�������Ƃ��̃f�t�H���g�Ή�
}

void ResourceManager::InitTitle(void)
{
	//�������܂��񂪁A�ǂ����Ă��g����������
	using RES = Resource;
	using RES_T = RES::TYPE;
	static std::string PATH_IMG = Application::PATH_IMAGE;
	static std::string PATH_MDL = Application::PATH_MODEL;
	static std::string PATH_EFF = Application::PATH_EFFECT;

	std::unique_ptr<Resource> res;

	//PushSpace
	res = std::make_unique<RES>(RES_T::IMG, PATH_IMG + "PushSpace.png");
	resourcesMap_.emplace(SRC::PUSH_SPACE, std::move(res));

	//�v���C���[
	res = std::make_unique<RES>(RES_T::MODEL, PATH_MDL + "Player/Player.mv1");
	resourcesMap_.emplace(SRC::PLAYER, std::move(res));

	//�X�J�C�h�[��
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
	//�������܂��񂪁A�ǂ����Ă��g����������
	using RES = Resource;
	using RES_T = RES::TYPE;
	static std::string PATH_IMG = Application::PATH_IMAGE;
	static std::string PATH_MDL = Application::PATH_MODEL;
	static std::string PATH_EFF = Application::PATH_EFFECT;

	std::unique_ptr<Resource> res;

	//�v���C���[
	res = std::make_unique<RES>(RES_T::MODEL, PATH_MDL + "Player/Player.mv1");
	resourcesMap_.emplace(SRC::PLAYER, std::move(res));

	//�v���C���[�e
	res = std::make_unique<RES>(RES_T::IMG, PATH_IMG + "Shadow.png");
	resourcesMap_.emplace(SRC::PLAYER_SHADOW, std::move(res));

	//����
	res = std::make_unique<RES>(RES_T::EFFEKSEER, PATH_EFF + "Smoke/Smoke.efkefc");
	resourcesMap_.emplace(SRC::FOOT_SMOKE, std::move(res));

	//���X�e�[�W
	res = std::make_unique<RES>(RES_T::MODEL, PATH_MDL + "Stage/kari_stage.mv1");
	resourcesMap_.emplace(SRC::STAGE, std::move(res));

	//�J�E���^�[
	res = std::make_unique<RES>(RES_T::MODEL, PATH_MDL + "Stage/counter.mv1");
	resourcesMap_.emplace(SRC::COUNTER, std::move(res));

	//�e�[�u��
	res = std::make_unique<RES>(RES_T::MODEL, PATH_MDL + "Stage/table_bounds.mv1");
	resourcesMap_.emplace(SRC::TABLE, std::move(res));

	//���W�X�^�[
	res = std::make_unique<RES>(RES_T::MODEL, PATH_MDL + "Stage/register.mv1");
	resourcesMap_.emplace(SRC::REGISTER, std::move(res));

	//�`���R�p���b�N
	res = std::make_unique<RES>(RES_T::MODEL, PATH_MDL + "Stage/chocoRack.mv1");
	resourcesMap_.emplace(SRC::CHOCO_RACK, std::move(res));

	//�X�C�[�c(�`���R)
	res = std::make_unique<RES>(RES_T::MODEL, PATH_MDL + "Stage/sweets_choco.mv1");
	resourcesMap_.emplace(SRC::SWEETS_CHOCO, std::move(res));

	//�x���[�p���b�N
	res = std::make_unique<RES>(RES_T::MODEL, PATH_MDL + "Stage/berryRack.mv1");
	resourcesMap_.emplace(SRC::BERRY_RACK, std::move(res));

	//�X�C�[�c�i�x���[�j
	res = std::make_unique<RES>(RES_T::MODEL, PATH_MDL + "Stage/sweets_berry.mv1");
	resourcesMap_.emplace(SRC::SWEETS_BERRY, std::move(res));

	//�X�C�[�c�V���[�P�[�X
	res = std::make_unique<RES>(RES_T::MODEL, PATH_MDL + "Stage/sweets_case.mv1");
	resourcesMap_.emplace(SRC::SWEETS_CASE, std::move(res));

	//�R�[�q�[�}�V��
	res = std::make_unique<RES>(RES_T::MODEL, PATH_MDL + "Stage/coffee_machine.mv1");
	resourcesMap_.emplace(SRC::COFFEE_MACHINE, std::move(res));

	//�z�b�g�p�J�b�v
	res = std::make_unique<RES>(RES_T::MODEL, PATH_MDL + "Stage/hotcup.mv1");
	resourcesMap_.emplace(SRC::HOTCUP, std::move(res));

	//�z�b�g�R�[�q�[
	res = std::make_unique<RES>(RES_T::MODEL, PATH_MDL + "Stage/hotcoffee.mv1");
	resourcesMap_.emplace(SRC::HOTCOFFEE, std::move(res));

	//�z�b�g�p�J�b�v�̃��b�N
	res = std::make_unique<RES>(RES_T::MODEL, PATH_MDL + "Stage/hotcupRack.mv1");
	resourcesMap_.emplace(SRC::HOTCUP_RACK, std::move(res));

	//���q
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

	//���[�h�ς݃`�F�b�N
	const auto& lPair = loadedMap_.find(src);
	if (lPair != loadedMap_.end())
	{
		return lPair->second;
	}

	//���\�[�X�o�^�`�F�b�N
	const auto& rPair = resourcesMap_.find(src);
	if (rPair == resourcesMap_.end())
	{
		//�o�^����Ă��Ȃ�
		return dummy_;
	}

	//���[�h����
	rPair->second->Load();

	//�O�̂��߃R�s�[�R���X�g���N�^
	loadedMap_.emplace(src, *rPair->second);

	return *rPair->second;

}
