#pragma once
#include "Common/Transform.h"
class ResourceManager;
class SceneManager;

class ActorBase
{

public:

	//コンストラクタ
	ActorBase(void);

	//デストラクタ
	virtual ~ActorBase(void);

	//解放
	virtual void Destroy(void);

	virtual void Init(void);
	virtual void Update(void);
	virtual void Draw(void);

	const Transform& GetTransform(void) const;

protected:

	//シングルトン参照
	SceneManager& scnMng_;

	//モデル制御の基本情報
	Transform transform_;

};
