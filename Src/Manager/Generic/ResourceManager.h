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

		PLAYER,
		PLAYER_SHADOW,
		SKY_DOME,
		FOOT_SMOKE,

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

		//�������e��UI
		UI_ORDER_BACK,
		UI_HOT,
		UI_ICE,
		UI_CHOCO,
		UI_BERRY,

		//�o�[�̃Q�[�W�Ƃ��̔w�i
		UI_BAR,
		UI_BARSHADOW,
		UI_CIRCLE,
		UI_CIRCLESHADOW,

		//�X�R�A�ɂ�郉���N�̃Q�[�W
		RANK_C,
		RANK_B,
		RANK_A,
		RANK_S,

		//�^�C�}�[�p
		TIMER_NUMBER,
		TIMER_COLON,

		//�X�R�A�p
		SCORE_NUMBER,

		//�J�E���g�_�E���p
		COUNTDOWN_NUMBER,
		TIME_UP,

		//��
		TITLE_BGM,
		GAME_BGM,
		TIMER,
		TIMER_FAST,
		GAME_FINISH,

		HOT_CUSTOMER,
		ICE_CUSTOMER,

		//���U���g�p���x��
		CURRENT_SCORE,
		RANKING,

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
