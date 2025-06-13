#pragma once
#include "../StageObject.h"

class InteractObject : public StageObject
{
public:

	static constexpr float COFFEE_PRODUCES_TIME = 3.0f;

	InteractObject(const std::string objId, const float width,
		const float height, const float depth, Player& player,
		std::vector<std::unique_ptr<StageObject>>& object);
	~InteractObject(void) = default;

	void Draw(void) override;

	void Interact(const std::string& objId) override;

	void UpdateInActive(void)override;
	void UpdateActive(void)override;

private:

	//
	std::vector<std::unique_ptr<StageObject>>& objects_;

	void IceDispenser(void);
	void CoffeeMachine(void);
	void CupLidRack(void);

	void SetProduceTime(float time) { param_.interactTime = time; }
};

