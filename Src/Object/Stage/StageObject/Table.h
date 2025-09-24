#pragma once
#include "../StageObject.h"

class Table : public StageObject
{
public:
	//コンストラクタ
	Table(const std::string objId, Player& player,
		std::vector<std::unique_ptr<StageObject>>& objects);

	//デストラクタ
	~Table(void) = default;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update(void)override;

private:
	//オブジェクトの参照
	std::vector<std::unique_ptr<StageObject>>& objects_;
};

