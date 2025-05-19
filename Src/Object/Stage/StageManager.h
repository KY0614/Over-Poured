#pragma once
#include <memory>
#include <map>
#include "../ActorBase.h"
#include "../../Common/Vector2.h"

class ResourceManager;
class StageObject;

class StageManager : public ActorBase
{

public:
	
	struct Size {
		int height_;
		int width_;
	};

	//コンストラクタ
	StageManager(Vector2 mapSize);

	//デストラクタ
	~StageManager(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;

	void LoadStage(const std::vector<std::vector<std::string>>& grid);

//	StageObject* GetObjectAt(Vector2 mapSize);
//	void SetObjectAt(Vector2 mapSize, std::unique_ptr<StageObject> obj);

	bool PlaceObject(int x, int y, const std::string& objectId);
	StageObject* GetStageObject(int x, int y);

private:

	Size size_;

	std::unique_ptr<StageObject> machine_;

	//std::vector<std::unique_ptr<StageObject>> objects_;
	std::vector<std::vector<std::unique_ptr<StageObject>>> grid_;
	//std::unordered_map<std::string, ObjectParams> objectMap_;

	bool IsInBounds(int x, int y) const;

	void UpdateDebugImGui(void);
};
