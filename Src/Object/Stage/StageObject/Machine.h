#pragma once
#include "../StageObject.h"

class GaugeUI;
class IconUI;

class Machine : public StageObject
{
public:

	static constexpr float COFFEE_PRODUCES_TIME = 3.0f;
	static constexpr float UI_OFFSET_Y = 160.0f;

	Machine(const std::string objId,Player& player,
		std::vector<std::unique_ptr<StageObject>>& object);
	~Machine(void) = default;

	void Init(VECTOR pos, float rotY = 0.0f, VECTOR scale = { 1.0f,1.0f,1.0f }) override;
	void UpdateInActive(void)override;
	void UpdateActive(void)override;
	void Draw(void) override;

	void Interact(const std::string& objId) override;


private:

	//
	std::vector<std::unique_ptr<StageObject>>& objects_;

	//�Q�[�W��UI
	std::unique_ptr<GaugeUI> gaugeUI_;
	//�A�C�R����UI
	std::unique_ptr<IconUI> iconUI_;
	std::unique_ptr<IconUI> hotIconUI_;
	std::unique_ptr<IconUI> iceIconUI_;
};

