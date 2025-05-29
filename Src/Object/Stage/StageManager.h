#pragma once
#include <memory>
#include <map>
#include "../ActorBase.h"
#include "../../Common/Vector2.h"

class ResourceManager;
class StageObject;
class Player;

class StageManager : public ActorBase
{

public:

	static constexpr int TABLE_NUM = 4;
	static constexpr float TABLE_WIDTH = 95.0f;
	static constexpr VECTOR TABLE_POS = { -140.0f, 0.0f, -175.0f };
	static constexpr VECTOR COLUMN_TABLE_POS = { -222.0f, 0.0f, -98.0f };
	static constexpr VECTOR MACHINE_POS = { -128.0f, 76.0f, -175.0f };
	static constexpr VECTOR CUPHOT_POS = { -45.0f, 76.0f, -175.0f };
	static constexpr VECTOR CUPICE_POS = { 45.0f, 76.0f, -175.0f };
	static constexpr VECTOR ICEDIS_POS = { 133.0f, 76.0f, -175.0f };
	static constexpr VECTOR LIBS_POS = { 74.0f, 76.0f, -175.0f };
	static constexpr VECTOR DUSTBOX_POS = { 227.0f, 0.0f, -175.0f };
	
	//コンストラクタ
	StageManager(Player& player);

	//デストラクタ
	~StageManager(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;

	void MakeHotCoffee(void);

	void SurveItem(void);

	template <typename Value>
	Value* FindValue(const std::vector<std::unique_ptr<StageObject>>& objects);

private:

	Player& player_;

	//std::unique_ptr<StageObject> machine_;
	//std::unique_ptr<StageObject> iceDispenser_;
	//std::unique_ptr<StageObject> table_;
	//std::unique_ptr<StageObject> sweetsC_;
	//std::unique_ptr<StageObject> sweetsS_;
	//std::unique_ptr<StageObject> cupH_;
	//std::unique_ptr<StageObject> cupI_;
	//std::unique_ptr<StageObject> cupIWithIce_;
	//std::unique_ptr<StageObject> libs_;
	//std::unique_ptr<StageObject> dustBox_;

	std::vector<std::unique_ptr<StageObject>> objects_;
	std::vector<std::unique_ptr<StageObject>> tables_;

	std::vector<std::vector<std::unique_ptr<StageObject>>> grid_;

	bool IsInBounds(int x, int y) const;

	void UpdateDebugImGui(void);
};
