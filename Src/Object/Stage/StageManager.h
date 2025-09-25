#pragma once
#include <memory>
#include <vector>
#include <map>
#include <functional>
#include "../Order/Order.h"
#include "../ActorBase.h"

class StageObject;
class Furnitures;
class Player;
class AnimationController;
class ModelMaterial;
class ModelRenderer;
class IconUI;

class StageManager : public ActorBase
{

public:

	//�A�j���[�V�������(���W�p)
	enum class ANIM_TYPE
	{
		CREATE,
		IDLE,
		PAYING,
	};
	
	//�R���X�g���N�^
	StageManager(Player& player);

	//�f�X�g���N�^
	~StageManager(void);

	/// <summary>
	/// ����������
	/// </summary>
	void Init(void) override;

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update(void) override;

	/// <summary>
	/// �`�揈��
	/// </summary>
	/// <param name=""></param>
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
	const bool& IsServed(void) const { return isServed_; }

	/// <summary>
	/// �񋟃f�[�^�����Z�b�g����
	/// </summary>
	/// <param name=""></param>
	void ResetServeData(void);

	/// <summary>
	/// �e�[�u���̐����擾����
	/// </summary>
	/// <param name=""></param>
	/// <returns>�������Ă���e�[�u���̐�</returns>
	int GetTableNum(void) const { return static_cast<int>(tables_.size()); }

	/// <summary>
	/// �J�E���^�[�̃��f�������擾����
	/// </summary>
	/// <returns>�J�E���^�[�̃��f�����</returns>
	const Transform& GetCounterTran(void) const;

	/// <summary>
	/// �e�[�u���̃��f�������擾����
	/// </summary>
	/// <returns�e�[�u���̃��f�����</returns>
	const Transform& GetTableTran(int index) const;

	/// <summary>
	/// �V���[�P�[�X�̃��f�������擾����
	/// </summary>
	/// <returns>�V���[�P�[�X�̃��f�����</returns>
	const Transform& GetShowCase(void) const;

	/// <summary>
	/// �S�~���̃��f�������擾����
	/// </summary>
	/// <returns>�S�~���̃��f�����</returns>
	const Transform& GetDustBox(void) const;

	/// <summary>
	/// ���̃��f�������擾����
	/// </summary>
	/// <returns>���̃��f�����</returns>
	const Transform& GetFloorTran(void) const;
			 
private:

	//�A�j���[�V����
	std::unique_ptr<AnimationController> animationController_;

	//�X�C�[�c�P�[�X�p�g�����X�t�H�[��
	Transform caseTran_;
	//���W�p�g�����X�t�H�[��
	Transform registerTran_;
	//�S�~���p�g�����X�t�H�[��
	Transform dustBoxTran_;

	//�v���C���[�̎Q��
	Player& player_;

	//�X�e�[�W�ɔz�u����I�u�W�F�N�g�B
	std::vector<std::unique_ptr<StageObject>> objects_;

	//�X�e�[�W�ɔz�u�����
	std::vector<std::unique_ptr<StageObject>> tables_;

	//�J�E���^�[�p
	std::unique_ptr<StageObject> counter_;

	//�Ƌ�p
	std::unique_ptr<Furnitures> furnitures_;

	//�������ꂽ����p�ӂ������ǂ���
	bool isServed_;

	//�񋟍ςݏ��i
	Order::OrderData servedItems_;

	//���݂̂��q�̒������e
	Order::OrderData currentOrder_;

	//�e�������񋟂��ꂽ���ǂ����̃t���O
	std::vector<bool> isServedItems_; 

	/// <summary>
	/// 3D���f���̏�����
	/// </summary>
	/// <param name="">�e��3D���f���̏������Ɠo�^</param>
	void Init3DModel(void);

	/// <summary>
	/// SE��BGM�̏�����
	/// </summary>
	void InitSound(void);

	/// <summary>
	/// �A�j���[�V�����̏�����
	/// </summary>
	/// <param name="">���W�̃A�j���[�V����</param>
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
	/// ���b�N�Ƃ̃C���^���N�g����
	/// </summary>
	/// <param name=""></param>
	void CupRackInteract(void);

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
	/// �R�[�q�[�𐶐����鏈��
	/// </summary>
	/// <param name="">�}�V���ɐݒu����Ă���J�b�v�ɑ΂��čs������/param>
	void ProduceCoffee(int index);

	/// <summary>
	/// �R�[�q�[����鏈��
	/// </summary>
	/// <param name="index">�z��ԍ��i�J�b�v���R�[�q�[�ɏ㏑������̂Łj</param>
	/// <param name="pos">����������W</param>
	/// <param name="objName">���R�[�q�[�̃p�����[�^���i�z�b�g���A�C�X)</param>
	void MakeCoffee(int index,StageObject& obj,std::string objName);

	/// <summary>
	/// �J�b�v�ɃA�C�X�f�B�X�y���T�[�ŕX�����鏈��
	/// </summary>
	/// <param name="index">�w�肷��A�C�X�f�B�X�y���T�[�̗v�f�ԍ�</param>
	void DispenseIce2Cup(int index);

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
};
