#pragma once
class ResourceManager;

class SceneBase
{

public:
	//音量の最大値
	static constexpr int VOLUME_MAX = 256; 

	// コンストラクタ
	SceneBase(void);

	// デストラクタ
	virtual ~SceneBase(void) = 0;

	// 初期化処理
	virtual void Init(void) = 0;

	// 更新ステップ
	virtual void Update(void) = 0;

	// 描画処理
	virtual void Draw(void) = 0;

protected:

};
