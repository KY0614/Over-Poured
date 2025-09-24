#pragma once
#include "../StageObject.h"

class Table : public StageObject
{
public:
	//�R���X�g���N�^
	Table(const std::string objId, Player& player,
		std::vector<std::unique_ptr<StageObject>>& objects);

	//�f�X�g���N�^
	~Table(void) = default;

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update(void)override;

private:
	//�I�u�W�F�N�g�̎Q��
	std::vector<std::unique_ptr<StageObject>>& objects_;
};

