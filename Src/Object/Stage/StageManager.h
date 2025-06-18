#pragma once
#include <memory>
#include <vector>
#include <map>
#include <functional>
#include "../Order/Order.h"
#include "../ActorBase.h"

class StageObject;
class Player;
class Interact2D;
class AnimationController;

class StageManager : public ActorBase
{

public:
	//�I�u�W�F�N�g�֘A

	//�e�[�u���֘A
	static constexpr int TABLE_COLUMN_NUM = 3;		//�e�[�u���̐�
	static constexpr int TABLE_ROW_FRONT_NUM = 5;	//�e�[�u���̐�
	static constexpr int TABLE_ROW_BACK_NUM = 2;	//�e�[�u���̐�

	static constexpr float TABLE_WIDTH = 95.0f;	//�e�[�u���̉���

	//���W

	static constexpr VECTOR TABLE_POS_BACK = { -140.0f, 0.0f, -175.0f };	//�e�[�u���̍��W
	static constexpr VECTOR TABLE_POS_FRONT = { -115.0f, 0.0f, 190.0f };	//�e�[�u���̍��W
	static constexpr VECTOR COLUMN_TABLE_POS = { -222.0f, 0.0f, -98.0f };	//��e�[�u���̍��W
	static constexpr VECTOR COUNTER_POS = { 225.0f, 0.0f, 190.0f };			//�J�E���^�[�̍��W
	static constexpr VECTOR DUST_BOX_POS = { 320.0f, 0.0f, -173.0f };		//�J�E���^�[�̍��W

	static constexpr VECTOR MACHINE_POS = { -128.0f, 76.0f, -175.0f };	//�R�[�q�[�}�V���̍��W
	static constexpr VECTOR CUPHOT_POS = { -45.0f, 76.0f, -175.0f };	//�z�b�g�p�J�b�v�̍��W
	static constexpr VECTOR CUPICE_POS = { 45.0f, 76.0f, -175.0f };		//�A�C�X�p�J�b�v�̍��W
	static constexpr VECTOR ICEDIS_POS = { 133.0f, 76.0f, -175.0f };	//�A�C�X�f�B�X�y���T�[�̍��W
	static constexpr VECTOR LIBS_POS = { 74.0f, 76.0f, -175.0f };		//�J�b�v�̊W�̍��W
	static constexpr VECTOR DUSTBOX_POS = { 227.0f, 0.0f, -175.0f };	//�S�~���̍��W

	//�A�j���[�V�������
	enum class ANIM_TYPE
	{
		CREATE,
		IDLE,
		PAYING,
		
	};

	enum class MODE {
		GAME_3D,		// 3D�Q�[�����[�h
		MACHINE_2D,		// 2D�}�V�����[�h
		ICE_2D,			// 2D���X�@���[�h
		LIDRACK_2D,		// 2D���b�h���b�N���[�h
	};
	
	//�R���X�g���N�^
	StageManager(Player& player);

	//�f�X�g���N�^
	~StageManager(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;

	/// <summary>
	/// ���݂̐擪�̂��q�̒�����ݒ肷��
	/// </summary>
	/// <param name="order">�擪�̂��q�̒���</param>
	void SetCurrentOrder(const Order::OrderData& order);

	/// <summary>
	/// �񋟂���A�C�e�����擾����
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	Order::OrderData GetServeItems(void)const { return servedItems_; }

	/// <summary>
	/// �񋟂������ǂ������擾����
	/// </summary>
	/// <param name=""></param>
	/// <returns>true:�񋟂����@flase:����</returns>
	bool IsServed(void) const { return isServed_; }

	/// <summary>
	/// �񋟃f�[�^�����Z�b�g����
	/// </summary>
	/// <param name=""></param>
	void ResetServeData(void);

private:

	//�A�j���[�V����
	std::unique_ptr<AnimationController> animationController_;

	//���[�h�Ǘ�
	MODE mode_;

	//��ԊǗ�(��ԑJ�ڎ���������)
	std::map<MODE, std::function<void(void)>> stateChanges_;

	//��ԊǗ�
	std::map<MODE, std::function<void(void)>> updateFunc_;
	std::map<MODE, std::function<void(void)>> drawFunc_;

	//��ԊǗ�(�X�V�X�e�b�v)
	std::function<void(void)> modeUpdate_;
	std::function<void(void)> modeDraw_;

	//�v���C���[�̎Q��
	Player& player_;

	//�X�e�[�W�ɔz�u����I�u�W�F�N�g�B
	std::vector<std::unique_ptr<StageObject>> objects_;

	//�X�e�[�W�ɔz�u�����
	std::vector<std::unique_ptr<StageObject>> tables_;

	//�J�E���^�[�p
	std::unique_ptr<StageObject> counter_;

	std::unique_ptr<Interact2D> interact2D_;	//2D�C���^���N�g�p

	bool isServed_;

	//�񋟍ςݏ��i
	Order::OrderData servedItems_;

	//���݂̂��q�̒������e
	Order::OrderData currentOrder_;

	//�e�������񋟂��ꂽ���ǂ����̃t���O
	std::vector<bool> isServedItems_; 

	void InitAnimation(void);

	/// <summary>
	/// �񋟂���A�C�e����ݒ肷��
	/// </summary>
	/// <param name="obj"></param>
	void SurveItem(StageObject& obj);

	/// <summary>
	/// �񋟂����A�C�e�����폜����
	/// </summary>
	/// <param name=""></param>
	void DeleteSurvedItem(void);

	/// <summary>
	/// �����^�щ\�ȃI�u�W�F�N�g�̃C���^���N�g����
	/// </summary>
	/// <param name=""></param>
	void CarryableObjInteract(void);

	/// <summary>
	/// �}�V���̃C���^���N�g����
	/// </summary>
	/// <param name="">�R�[�q�[�}�V���ƃA�C�X�f�B�X�y���T�[�̃C���^���N�g����</param>
	void MachineInteract(void);

	/// <summary>
	/// �W�̃��b�N�̃C���^���N�g����
	/// </summary>
	/// <param name="">�C���^���N�g���̓v���C���[�͑���ł��Ȃ�</param>
	void LidRackInteract(void);

	/// <summary>
	/// �z�b�g�R�[�q�[�����
	/// </summary>
	/// <param name="">�}�V���ƃz�b�g�p�J�b�v�����������A�J�b�v�̏ꏊ�ɃR�[�q�[����</param>
	void MakeCoffee(void);
	void MakeHotCoffee(int i);
	void MakeIceCoffee(int i);

	/// <summary>
	///�@�J�b�v�ɃA�C�X�f�B�X�y���T�[�ŕX�����鏈��
	/// </summary>
	/// <param name="">dynamic_cast�ŃA�C�X�J�b�v�̊֐����g�p</param>
	void DispenseIce2Cup(void);

	/// <summary>
	/// �R�[�q�[�ɊW������
	/// </summary>
	/// <param name=""></param>
	void LidFollowCup(void);

	/// <summary>
	/// �S�~���̃C���^���N�g����
	/// </summary>
	/// <param name=""></param>
	void DustBoxInteract(void);

	/// <summary>
	/// �I�[�_�[���ɉ��������̃A�C�e����񋟂��Ă��邩�ǂ���
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	bool IsOrderCompleted(void);

	//��ԑJ��
	void ChangeMode(MODE mode);
	void ChangeMode3DGame(void);
	void ChangeModeMachine2D(void);
	void ChangeModeIce2D(void);
	void ChangeModeLidRack2D(void);

	//�X�V�X�e�b�v
	void Update3DGame(void);
	void UpdateMachine2D(void);
	void UpdateIce2D(void);
	void UpdateLidRack2D(void);

	//�`��X�e�b�v
	void Draw3DGame(void);
	void DrawMachine2D(void);
	void DrawIce2D(void);
	void DrawLidRack2D(void);

	void DrawDebug(void);

	void UpdateDebugImGui(void);

};
