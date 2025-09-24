#pragma once
#include "../StageObject.h"

class DustBox : public StageObject
{
public:
	//�R���X�g���N�^
	DustBox(const std::string objId,Player& player,
		std::vector<std::unique_ptr<StageObject>>& objects);

	//�f�X�g���N�^
	~DustBox(void) = default;

	/// <summary>
	/// �C���^���N�g����
	/// </summary>
	/// <param name="objId">�̂Ă�I�u�W�F�N�gid</param>
	void Interact(const std::string& objId) override;

private:
	//�I�u�W�F�N�g�̎Q��
	std::vector<std::unique_ptr<StageObject>>& objects_;
};

