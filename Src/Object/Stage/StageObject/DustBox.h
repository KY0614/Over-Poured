#pragma once
#include "../StageObject.h"

class DustBox : public StageObject
{
public:
	//コンストラクタ
	DustBox(const std::string objId,Player& player,
		std::vector<std::unique_ptr<StageObject>>& objects);

	//デストラクタ
	~DustBox(void) = default;

	/// <summary>
	/// インタラクト処理
	/// </summary>
	/// <param name="objId">捨てるオブジェクトid</param>
	void Interact(const std::string& objId) override;

private:
	//オブジェクトの参照
	std::vector<std::unique_ptr<StageObject>>& objects_;
};

