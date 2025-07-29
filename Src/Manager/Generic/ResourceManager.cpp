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
	static std::string PATH_SND = Application::PATH_SOUND;

	std::unique_ptr<Resource> res;

	//PushSpace�摜
	res = std::make_unique<RES>(RES_T::IMG, PATH_IMG + "PleaseKey.png");
	resourcesMap_.emplace(SRC::PUSH_SPACE, std::move(res));

	//�^�C�g�����S
	res = std::make_unique<RES>(RES_T::IMG, PATH_IMG + "Title.png");
	resourcesMap_.emplace(SRC::TITLE_LOGO, std::move(res));

	//�v���C���[
	res = std::make_unique<RES>(RES_T::MODEL, PATH_MDL + "Player/Player.mv1");
	resourcesMap_.emplace(SRC::PLAYER, std::move(res));

	//�X�J�C�h�[��
	res = std::make_unique<RES>(RES_T::MODEL, PATH_MDL + "SkyDome/SkyDome.mv1");
	resourcesMap_.emplace(SRC::SKY_DOME, std::move(res));

	//���X
	res = std::make_unique<RES>(RES_T::MODEL, PATH_MDL + "Stage/cafe.mv1");
	resourcesMap_.emplace(SRC::CAFE, std::move(res));

	//�Ƌ�F��
	res = std::make_unique<RES>(RES_T::MODEL, PATH_MDL + "Stage/floor.mv1");
	resourcesMap_.emplace(SRC::FLOOR, std::move(res));

	//�n�ʃe�N�X�`��
	res = std::make_unique<RES>(RES_T::IMG, PATH_MDL + "Stage/Tex/ground.png");
	resourcesMap_.emplace(SRC::GROUND, std::move(res));

	//��------------------------------------------------------------------------

	//BGM
	res = std::make_unique<RES>(RES_T::SOUND, PATH_SND + "BGM/Title.mp3");
	resourcesMap_.emplace(SRC::TITLE_BGM, std::move(res));

	//SE
	res = std::make_unique<RES>(RES_T::SOUND, PATH_SND + "SE/press_key.mp3");
	resourcesMap_.emplace(SRC::PUSH_SPACE_SE, std::move(res));

	//--------------------------------------------------------------------------
}

void ResourceManager::InitMovie(void)
{
}

void ResourceManager::InitSelect(void)
{
}

void ResourceManager::InitTutorial(void)
{
	using RES = Resource;
	using RES_T = RES::TYPE;
	static std::string PATH_IMG = Application::PATH_IMAGE;
	static std::string PATH_MDL = Application::PATH_MODEL;
	static std::string PATH_EFF = Application::PATH_EFFECT;
	static std::string PATH_SND = Application::PATH_SOUND;

	std::unique_ptr<Resource> res;

	//�����p�摜
	res = std::make_unique<RES>(RES_T::IMGS, PATH_IMG + "tutorial.png",
		3, 1, 1080, 1080);
	resourcesMap_.emplace(SRC::TUTORIAL, std::move(res));

	//�w�i�p�摜
	res = std::make_unique<RES>(RES_T::IMG, PATH_IMG + "tutorial_back.png");
	resourcesMap_.emplace(SRC::TUTORIAL_BACK, std::move(res));

	//�����p�摜
	res = std::make_unique<RES>(RES_T::IMG, PATH_IMG + "Hangin_Banner.png");
	resourcesMap_.emplace(SRC::PINK_BANNER, std::move(res));

	//�J�[�\���p�摜
	res = std::make_unique<RES>(RES_T::IMGS, PATH_IMG + "cursor_R.png",
		2,1,300,300);
	resourcesMap_.emplace(SRC::CURSOR_R, std::move(res));

	//PushSpace�摜
	res = std::make_unique<RES>(RES_T::IMG, PATH_IMG + "PleaseKey.png");
	resourcesMap_.emplace(SRC::PUSH_SPACE, std::move(res));

	//��----------------------------------------------------------------------------
	//BGM
	res = std::make_unique<RES>(RES_T::SOUND, PATH_SND + "BGM/Tutorial.mp3");
	resourcesMap_.emplace(SRC::TUTORIAL_BGM, std::move(res));

	//�y�[�W��i�܂���Ƃ���SE
	res = std::make_unique<RES>(RES_T::SOUND, PATH_SND + "SE/next_page.mp3");
	resourcesMap_.emplace(SRC::NEXT_PAGE, std::move(res));

	//�y�[�W��i�܂���Ƃ���SE
	res = std::make_unique<RES>(RES_T::SOUND, PATH_SND + "SE/return_page.mp3");
	resourcesMap_.emplace(SRC::RETURN_PAGE, std::move(res));

	//�y�[�W���i�܂Ȃ��Ƃ���SE
	res = std::make_unique<RES>(RES_T::SOUND, PATH_SND + "SE/not_page.mp3");
	resourcesMap_.emplace(SRC::NOT_PAGE, std::move(res));

	//�J�X�J�����R����
	res = std::make_unique<RES>(RES_T::SOUND, PATH_SND + "SE/open_door.mp3");
	resourcesMap_.emplace(SRC::OPEN_DOOR, std::move(res));
}

void ResourceManager::InitGame(void)
{
	using RES = Resource;
	using RES_T = RES::TYPE;
	static std::string PATH_IMG = Application::PATH_IMAGE;
	static std::string PATH_MDL = Application::PATH_MODEL;
	static std::string PATH_EFF = Application::PATH_EFFECT;
	static std::string PATH_SND = Application::PATH_SOUND;

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

	//�Ƌ�F��
	res = std::make_unique<RES>(RES_T::MODEL, PATH_MDL + "Stage/floor.mv1");
	resourcesMap_.emplace(SRC::FLOOR, std::move(res));

	//�Ƌ�F�A�ؔ�
	res = std::make_unique<RES>(RES_T::MODEL, PATH_MDL + "Stage/plant.mv1");
	resourcesMap_.emplace(SRC::PLANT, std::move(res));

	//�Ƌ�F�I
	res = std::make_unique<RES>(RES_T::MODEL, PATH_MDL + "Stage/shelves.mv1");
	resourcesMap_.emplace(SRC::SHELVES, std::move(res));

	//�Ƌ�F�I
	res = std::make_unique<RES>(RES_T::MODEL, PATH_MDL + "Stage/shelves_front.mv1");
	resourcesMap_.emplace(SRC::SHELVES_FRONT, std::move(res));

	//�Ƌ�F����
	res = std::make_unique<RES>(RES_T::MODEL, PATH_MDL + "Stage/desk.mv1");
	resourcesMap_.emplace(SRC::DESK, std::move(res));

	//�J�E���^�[
	res = std::make_unique<RES>(RES_T::MODEL, PATH_MDL + "Stage/counter.mv1");
	resourcesMap_.emplace(SRC::COUNTER, std::move(res));

	//�e�[�u��
	res = std::make_unique<RES>(RES_T::MODEL, PATH_MDL + "Stage/table.mv1");
	resourcesMap_.emplace(SRC::TABLE, std::move(res));
	
	//�e�[�u���p�R���C�_�[
	res = std::make_unique<RES>(RES_T::MODEL, PATH_MDL + "Stage/table_bounds.mv1");
	resourcesMap_.emplace(SRC::TABLE_COL, std::move(res));

	//���W�X�^�[
	res = std::make_unique<RES>(RES_T::MODEL, PATH_MDL + "Stage/register.mv1");
	resourcesMap_.emplace(SRC::REGISTER, std::move(res));

	//�`���R�p���b�N
	res = std::make_unique<RES>(RES_T::MODEL, PATH_MDL + "Stage/SweetsRack.mv1");
	resourcesMap_.emplace(SRC::CHOCO_RACK, std::move(res));

	//�X�C�[�c(�`���R)
	res = std::make_unique<RES>(RES_T::MODEL, PATH_MDL + "Stage/ChocoSweets.mv1");
	resourcesMap_.emplace(SRC::SWEETS_CHOCO, std::move(res));

	//�x���[�p���b�N
	res = std::make_unique<RES>(RES_T::MODEL, PATH_MDL + "Stage/SweetsRack.mv1");
	resourcesMap_.emplace(SRC::BERRY_RACK, std::move(res));

	//�X�C�[�c�i�x���[�j
	res = std::make_unique<RES>(RES_T::MODEL, PATH_MDL + "Stage/BerrySweets.mv1");
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

	//�A�C�X�p�J�b�v
	res = std::make_unique<RES>(RES_T::MODEL, PATH_MDL + "Stage/icecup.mv1");
	resourcesMap_.emplace(SRC::ICECUP, std::move(res));

	//�A�C�X�R�[�q�[
	res = std::make_unique<RES>(RES_T::MODEL, PATH_MDL + "Stage/icecoffee.mv1");
	resourcesMap_.emplace(SRC::ICECOFFEE, std::move(res));

	//�A�C�X�p�J�b�v�̃��b�N
	res = std::make_unique<RES>(RES_T::MODEL, PATH_MDL + "Stage/icecupRack.mv1");
	resourcesMap_.emplace(SRC::ICECUP_RACK, std::move(res));

	//���X�@
	res = std::make_unique<RES>(RES_T::MODEL, PATH_MDL + "Stage/icedispenser.mv1");
	resourcesMap_.emplace(SRC::ICEDISPENSER, std::move(res));

	//�X
	res = std::make_unique<RES>(RES_T::MODEL, PATH_MDL + "Stage/ice.mv1");
	resourcesMap_.emplace(SRC::ICE, std::move(res));

	//�J�b�v�̊W�̃��b�N
	res = std::make_unique<RES>(RES_T::MODEL, PATH_MDL + "Stage/cuplidRack_.mv1");
	resourcesMap_.emplace(SRC::CUPLID_RACK, std::move(res));

	//�J�b�v�̊W(�z�b�g�j
	res = std::make_unique<RES>(RES_T::MODEL, PATH_MDL + "Stage/hotcupLid.mv1");
	resourcesMap_.emplace(SRC::HOTCUP_LID, std::move(res));

	//�J�b�v�̊W(�A�C�X�j
	res = std::make_unique<RES>(RES_T::MODEL, PATH_MDL + "Stage/icecupLid.mv1");
	resourcesMap_.emplace(SRC::ICECUP_LID, std::move(res));

	//�S�~��
	res = std::make_unique<RES>(RES_T::MODEL, PATH_MDL + "Stage/dustbox.mv1");
	resourcesMap_.emplace(SRC::DUSTBOX, std::move(res));

	//�C���^���N�g�\�摜
	res = std::make_unique<RES>(RES_T::IMG, PATH_IMG + "interact.png");
	resourcesMap_.emplace(SRC::INTERACT, std::move(res));

	//�R�[�q�[�\�摜
	res = std::make_unique<RES>(RES_T::IMG, PATH_IMG + "coffee.png");
	resourcesMap_.emplace(SRC::BREW_COFFEE, std::move(res));

	//�����pUI---------------------------------------------------------------------
	//�w�i�p
	res = std::make_unique<RES>(RES_T::IMG, PATH_IMG + "order.png");
	resourcesMap_.emplace(SRC::UI_ORDER_BACK, std::move(res));

	//�z�b�g
	res = std::make_unique<RES>(RES_T::IMG, PATH_IMG + "order_UI.png");
	resourcesMap_.emplace(SRC::UI_HOT, std::move(res));

	//�A�C�X
	res = std::make_unique<RES>(RES_T::IMG, PATH_IMG + "order_ice.png");
	resourcesMap_.emplace(SRC::UI_ICE, std::move(res));

	//�`���R
	res = std::make_unique<RES>(RES_T::IMG, PATH_IMG + "order_choco.png");
	resourcesMap_.emplace(SRC::UI_CHOCO, std::move(res));

	//�x���[
	res = std::make_unique<RES>(RES_T::IMG, PATH_IMG + "oroder_berry.png");
	resourcesMap_.emplace(SRC::UI_BERRY, std::move(res));

	//�`�F�b�N�}�[�N
	res = std::make_unique<RES>(RES_T::IMG, PATH_IMG + "check.png");
	resourcesMap_.emplace(SRC::UI_CHECK, std::move(res));

	//-------------------------------------------------------------------------
	//�Q�[�WUI
	res = std::make_unique<RES>(RES_T::IMG, PATH_IMG + "bar_green.png");
	resourcesMap_.emplace(SRC::UI_BAR, std::move(res));

	//�Q�[�W�eUI
	res = std::make_unique<RES>(RES_T::IMG, PATH_IMG + "bar_shadow.png");
	resourcesMap_.emplace(SRC::UI_BARSHADOW, std::move(res));

	//�T�[�N���Q�[�WUI
	res = std::make_unique<RES>(RES_T::IMG, PATH_IMG + "order_timer.png");
	resourcesMap_.emplace(SRC::UI_CIRCLE, std::move(res));

	//�T�[�N���Q�[�W�eUI
	res = std::make_unique<RES>(RES_T::IMG, PATH_IMG + "order_timer_back.png");
	resourcesMap_.emplace(SRC::UI_CIRCLESHADOW, std::move(res));

	//�^�C�}�[�p-----------------------------------------------------------
	//����
	res = std::make_unique<RES>(RES_T::IMGS, PATH_IMG + "Timer_Numbers.png",
		NUMBER_NUM_X, NUMBER_NUM_Y, NUMBER_SIZE_X, NUMBER_SIZE_Y);
	resourcesMap_.emplace(SRC::TIMER_NUMBER, std::move(res));

	//�R����
	res = std::make_unique<RES>(RES_T::IMG, PATH_IMG + "colon.png");
	resourcesMap_.emplace(SRC::TIMER_COLON, std::move(res));
	//---------------------------------------------------------------------

	//�J�E���g�_�E���A�J�E���g�A�b�v�p------------------------------------------------------------------
	//������start�p�摜
	res = std::make_unique<RES>(RES_T::IMGS, PATH_IMG + "count_down.png",
		1, 4, 400, 150);
	resourcesMap_.emplace(SRC::COUNTDOWN_NUMBER, std::move(res));
		
	//�^�C���A�b�v�p�摜
	res = std::make_unique<RES>(RES_T::IMG, PATH_IMG + "time_up.png");
	resourcesMap_.emplace(SRC::TIME_UP, std::move(res));

	//--------------------------------------------------------------------------
	
	//�X�R�A�p------------------------------------------------------------------
	//����
	res = std::make_unique<RES>(RES_T::IMGS, PATH_IMG + "Numbers.png",
		NUMBER_NUM_X, NUMBER_NUM_Y, 128, 128);
	resourcesMap_.emplace(SRC::SCORE_NUMBER, std::move(res));

	//--------------------------------------------------------------------------

	//��------------------------------------------------------------------------
	
	//BGM
	res = std::make_unique<RES>(RES_T::SOUND, PATH_SND + "BGM/GameScene.mp3");
	resourcesMap_.emplace(SRC::GAME_BGM, std::move(res));

	res = std::make_unique<RES>(RES_T::SOUND, PATH_SND + "SE/game_start.mp3");
	resourcesMap_.emplace(SRC::GAME_START, std::move(res));

	res = std::make_unique<RES>(RES_T::SOUND, PATH_SND + "SE/count_down.mp3");
	resourcesMap_.emplace(SRC::COUNT_DOWN, std::move(res));

	res = std::make_unique<RES>(RES_T::SOUND, PATH_SND + "SE/game_finish.mp3");
	resourcesMap_.emplace(SRC::GAME_FINISH, std::move(res));

	res = std::make_unique<RES>(RES_T::SOUND, PATH_SND + "SE/timer.mp3");
	resourcesMap_.emplace(SRC::TIMER, std::move(res));

	res = std::make_unique<RES>(RES_T::SOUND, PATH_SND + "SE/timer_fast.mp3");
	resourcesMap_.emplace(SRC::TIMER_FAST, std::move(res));

	res = std::make_unique<RES>(RES_T::SOUND, PATH_SND + "SE/pick_up.mp3");
	resourcesMap_.emplace(SRC::PICK_UP, std::move(res));

	res = std::make_unique<RES>(RES_T::SOUND, PATH_SND + "SE/add_stock.mp3");
	resourcesMap_.emplace(SRC::ADD_STOCK, std::move(res));

	res = std::make_unique<RES>(RES_T::SOUND, PATH_SND + "SE/paying.mp3");
	resourcesMap_.emplace(SRC::PAYING, std::move(res));

	//--------------------------------------------------------------------------
	
	//���q
	res = std::make_unique<RES>(RES_T::MODEL, PATH_MDL + "Customer/hotcup_customer.mv1");
	resourcesMap_.emplace(SRC::HOT_CUSTOMER, std::move(res));
	//���q
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
	static std::string PATH_SND = Application::PATH_SOUND;

	std::unique_ptr<Resource> res;

	//�~�iC)
	res = std::make_unique<RES>(RES_T::IMG, PATH_IMG + "Circle_C.png");
	resourcesMap_.emplace(SRC::RANK_C, std::move(res));

	//�~�iB)
	res = std::make_unique<RES>(RES_T::IMG, PATH_IMG + "Circle_B.png");
	resourcesMap_.emplace(SRC::RANK_B, std::move(res));

	//�~�iA)
	res = std::make_unique<RES>(RES_T::IMG, PATH_IMG + "Circle_A.png");
	resourcesMap_.emplace(SRC::RANK_A, std::move(res));

	//�~�iS)
	res = std::make_unique<RES>(RES_T::IMG, PATH_IMG + "Circle_S.png");
	resourcesMap_.emplace(SRC::RANK_S, std::move(res));

	//�Q�[�W�eUI
	res = std::make_unique<RES>(RES_T::IMG, PATH_IMG + "CircleGage.png");
	resourcesMap_.emplace(SRC::UI_CIRCLESHADOW, std::move(res));

	//����
	res = std::make_unique<RES>(RES_T::IMGS, PATH_IMG + "Numbers.png",
		NUMBER_NUM_X, NUMBER_NUM_Y, 128, 128);
	resourcesMap_.emplace(SRC::SCORE_NUMBER, std::move(res));
	
	//���݃X�R�A���x��
	res = std::make_unique<RES>(RES_T::IMG, PATH_IMG + "current_score.png");
	resourcesMap_.emplace(SRC::CURRENT_SCORE, std::move(res));

	//�����N�iC,B,A,S)
	res = std::make_unique<RES>(RES_T::IMGS, PATH_IMG + "Ranks.png",
		4, 1, 300, 300);
	resourcesMap_.emplace(SRC::RANKS, std::move(res));

	//�����L���O���x��
	res = std::make_unique<RES>(RES_T::IMGS, PATH_IMG + "ranking.png",
		1, 5, 300, 100);
	resourcesMap_.emplace(SRC::RANKING_LABEL, std::move(res));

	//�����L���O�w�i
	res = std::make_unique<RES>(RES_T::IMG, PATH_IMG + "ranking_back.png");
	resourcesMap_.emplace(SRC::RANKING_BACK, std::move(res));

	//�����p�摜
	res = std::make_unique<RES>(RES_T::IMG, PATH_IMG + "Hangin_Banner.png");
	resourcesMap_.emplace(SRC::PINK_BANNER, std::move(res));

	//PushSpace�摜
	res = std::make_unique<RES>(RES_T::IMG, PATH_IMG + "PleaseKey.png");
	resourcesMap_.emplace(SRC::PUSH_SPACE, std::move(res));

	//��------------------------------------------------------------------------

	//BGM
	res = std::make_unique<RES>(RES_T::SOUND, PATH_SND + "BGM/Result.mp3");
	resourcesMap_.emplace(SRC::RESULT_BGM, std::move(res));

	res = std::make_unique<RES>(RES_T::SOUND, PATH_SND + "SE/normal.mp3");
	resourcesMap_.emplace(SRC::SCORE_NORMAL, std::move(res));

	res = std::make_unique<RES>(RES_T::SOUND, PATH_SND + "SE/good.mp3");
	resourcesMap_.emplace(SRC::SCORE_GOOD, std::move(res));

	res = std::make_unique<RES>(RES_T::SOUND, PATH_SND + "SE/greate.mp3");
	resourcesMap_.emplace(SRC::SCORE_GREATE, std::move(res));

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
