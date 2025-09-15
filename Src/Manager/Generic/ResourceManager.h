#pragma once
#include <memory>
#include <map>
#include <string>
#include "Resource.h"

class ResourceManager
{

public:
	static constexpr int NUMBER_NUM_X = 5;
	static constexpr int NUMBER_NUM_Y = 2;
	static constexpr int NUMBER_SIZE_X = 71;
	static constexpr int NUMBER_SIZE_Y = 100;

	// ���\�[�X��
	enum class SRC
	{
		NONE,
		TITLE_LOGO,
		CAFE,
		GROUND,
		PUSH_SPACE,
		PUSH_SPACE_SE,

		TUTORIAL,
		TUTORIAL_BGM,
		NEXT_PAGE,
		RETURN_PAGE,
		NOT_PAGE,
		TUTORIAL_BACK,
		PINK_BANNER,
		CURSOR_R,
		OPEN_DOOR,

		PLAYER,
		PLAYER_SHADOW,
		SKY_DOME,
		FOOT_SMOKE,

		//���q
		HOT_CUSTOMER,
		ICE_CUSTOMER,

		//�X�e�[�W�I�u�W�F�N�g
		FLOOR,
		PLANT,
		SHELVES,
		SHELVES_FRONT,
		DESK,
		COUNTER,
		TABLE,
		TABLE_COL,
		REGISTER,
		CHOCO_RACK,
		BERRY_RACK,
		SWEETS_CHOCO,
		SWEETS_BERRY,
		SWEETS_CASE,
		COFFEE_MACHINE,
		HOTCUP,
		HOTCUP_RACK,
		HOTCOFFEE,
		ICECUP,
		ICECUP_RACK,
		ICECOFFEE,
		ICEDISPENSER,
		ICE,
		HOTCUP_LID,
		ICECUP_LID,
		CUPLID_RACK,
		DUSTBOX,

		//�A�C�R���n��UI
		INTERACT,
		BREW_COFFEE,
		ICE_IN,
		HOT_ICON,
		ICE_ICON,
		STOCK_ICON,

		//�������e��UI
		UI_ORDER_BACK,
		UI_HOT,
		UI_ICE,
		UI_CHOCO,
		UI_BERRY,
		UI_CHECK,

		//�o�[�̃Q�[�W�Ƃ��̔w�i
		UI_BAR,
		UI_BARSHADOW,
		UI_CIRCLE,
		UI_CIRCLESHADOW,

		//�^�C�}�[�p
		TIMER_NUMBER,
		TIMER_COLON,

		//�J�E���g�_�E���p
		COUNTDOWN_NUMBER,
		TIME_UP,

		//�|�[�Y
		MENU_LIST,
		MENU_CURSOR,
		MENU_EXPLAIN,
		MENU_BACK_LOGO,
		MENU_KEY_CONFIG,

		//��
		TITLE_BGM,
		GAME_BGM,
		TIMER,
		TIMER_FAST,
		COUNT_DOWN,
		GAME_START,
		GAME_FINISH,
		PICK_UP,
		ADD_STOCK,
		PAYING,
		PUT_ON,
		MENU_BACK,
		MENU_OPEN,
		MENU_CLOSE,

		//���U���g
		RANKS,

		//�X�R�A�ɂ�郉���N�̃Q�[�W
		RANK_C,
		RANK_B,
		RANK_A,
		RANK_S,

		//�X�R�A�p
		SCORE_NUMBER,

		//��
		RESULT_BGM,
		SCORE_NORMAL,
		SCORE_GOOD,
		SCORE_GREATE,

		//���U���g�p���x��
		CURRENT_SCORE,
		RANKING_LABEL,
		RANKING_BACK,

		HAPPY_STAR,
		GURU,
		ORB,

	};

	// �����I�ɃC���X�e���X�𐶐�����
	static void CreateInstance(void);

	// �ÓI�C���X�^���X�̎擾
	static ResourceManager& GetInstance(void);

	// ������
	void Init(void);

	// ���(�V�[���ؑ֎��Ɉ�U���)
	void Release(void);

	// ���\�[�X�̊��S�j��
	void Destroy(void);

	// ���\�[�X�̃��[�h
	const Resource& Load(SRC src);

	// ���\�[�X�̕������[�h(���f���p)
	int LoadModelDuplicate(SRC src);

	// string����SRC�ɕϊ��i�X�e�[�W�I�u�W�F�N�g��string�ŊǗ�����Ă��邽�߁j
	SRC StringToSRC(const std::string& name);

	//�V�[�����ƂɃf�[�^��ǂݍ��ނ��Ƃɂ���
	void InitTitle(void);
	void InitMovie(void);
	void InitSelect(void);
	void InitTutorial(void);
	void InitGame(void);
	void InitPause(void);
	void InitResult(void);

private:

	// �ÓI�C���X�^���X
	static ResourceManager* instance_;

	// ���\�[�X�Ǘ��̑Ώ�
	std::map<SRC, std::unique_ptr<Resource>> resourcesMap_;

	// �ǂݍ��ݍς݃��\�[�X
	std::map<SRC, Resource&> loadedMap_;

	Resource dummy_;

	// �f�t�H���g�R���X�g���N�^��private�ɂ��āA
	// �O�����琶���ł��Ȃ��l�ɂ���
	ResourceManager(void);
	ResourceManager(const ResourceManager& manager) = default;
	~ResourceManager(void) = default;

	// �������[�h
	Resource& _Load(SRC src);

};
