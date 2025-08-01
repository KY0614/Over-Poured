#pragma once
#include <memory>
#include "SceneBase.h"

class StageManager;
class Player;
class OrderCustomerManager;
class Timer;

class TutorialScene : public SceneBase
{
public:
	//背景画像関連
	static constexpr int BACK_IMG_MARGINE = 250;	//初期座標から少しだけ間隔をあける
	static constexpr int BACK_IMG_SCALE = 500;		//背景画像の大きさ

	static constexpr int CURSOR_IMG_MARGINE = 300;	//初期座標から少しだけ間隔をあける

	static constexpr int TUTORIAL_IMG_MAX_NUM = 3;				//説明用画像の最大枚数
	static constexpr int INDEX_MAX = TUTORIAL_IMG_MAX_NUM - 1;	//０からなので１引いておく

	static constexpr float HIGH_LIGHT_INTERVAL = 1.2f;

	static constexpr int LOGO_HEIGHT = 1024;

	enum class STEP
	{
		OPERATION,	//操作説明
		ORDER,		//注文説明
		CUSTOMER,	//客の説明
		FINISH,		//終了
	};

	//コンストラクタ
	TutorialScene(void);

	//デストラクタ
	~TutorialScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;

private:
	//関数ポインタ
	using UpdateFunc_t = void(TutorialScene::*)();
	using DrawFunc_t = void(TutorialScene::*)();

	UpdateFunc_t update_;
	DrawFunc_t draw_;

	//ステージ
	std::unique_ptr<StageManager> stage_;

	//プレイヤー
	std::unique_ptr<Player> player_;

	//客
	std::unique_ptr<OrderCustomerManager> customer_;

	void UpdateOperation(void);
	
	void DrawOperation(void);

	//画像で説明用ーーーーーーーーーーーーーーーーーーーーーー

	//説明用画像
	int* tutorialImgs_;
	int tutorialBackImg_;
	int* cursorImg_;

	int decoImg_;

	int pushImg_;	//押下画像

	//説明用画像のインデックス
	int imgIdx_;

	int blinkTime_;
	int blinkIdx_;

	bool isBlinkR_;
	bool isBlinkL_;

	bool isView_;
	float highlightTime_;

	void ImageInit(void);
	void ImageUpdate(void);
	void ImageDraw(void);
};

