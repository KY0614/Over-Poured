#pragma once
#include <memory>
#include <map>
#include <string>
#include "Resource.h"

class ResourceManager
{

public:
	static constexpr int NUMBER_NUM_X = 5;
	static constexpr int NUMBER_NUM_Y = 2;
	static constexpr int NUMBER_SIZE_X = 71;
	static constexpr int NUMBER_SIZE_Y = 100;

	// リソース名
	enum class SRC
	{
		NONE,
		TITLE_LOGO,
		CAFE,
		GROUND,
		PUSH_SPACE,
		PUSH_SPACE_SE,

		PLAYER,
		PLAYER_SHADOW,
		SKY_DOME,
		FOOT_SMOKE,

		//ステージオブジェクト
		FLOOR,
		PLANT,
		SHELVES,
		SHELVES_FRONT,
		DESK,
		COUNTER,
		TABLE,
		TABLE_COL,
		REGISTER,
		CHOCO_RACK,
		BERRY_RACK,
		SWEETS_CHOCO,
		SWEETS_BERRY,
		SWEETS_CASE,
		COFFEE_MACHINE,
		HOTCUP,
		HOTCUP_RACK,
		HOTCOFFEE,
		ICECUP,
		ICECUP_RACK,
		ICECOFFEE,
		ICEDISPENSER,
		ICE,
		HOTCUP_LID,
		ICECUP_LID,
		CUPLID_RACK,
		DUSTBOX,

		//注文内容のUI
		UI_ORDER_BACK,
		UI_HOT,
		UI_ICE,
		UI_CHOCO,
		UI_BERRY,

		//バーのゲージとその背景
		UI_BAR,
		UI_BARSHADOW,
		UI_CIRCLE,
		UI_CIRCLESHADOW,

		//スコアによるランクのゲージ
		RANK_C,
		RANK_B,
		RANK_A,
		RANK_S,

		//タイマー用
		TIMER_NUMBER,
		TIMER_COLON,

		//スコア用
		SCORE_NUMBER,

		//カウントダウン用
		COUNTDOWN_NUMBER,
		TIME_UP,

		//音
		TITLE_BGM,
		GAME_BGM,
		TIMER,
		TIMER_FAST,
		GAME_FINISH,

		HOT_CUSTOMER,
		ICE_CUSTOMER,

		//リザルト用ラベル
		CURRENT_SCORE,
		RANKING,

	};

	// 明示的にインステンスを生成する
	static void CreateInstance(void);

	// 静的インスタンスの取得
	static ResourceManager& GetInstance(void);

	// 初期化
	void Init(void);

	// 解放(シーン切替時に一旦解放)
	void Release(void);

	// リソースの完全破棄
	void Destroy(void);

	// リソースのロード
	const Resource& Load(SRC src);

	// リソースの複製ロード(モデル用)
	int LoadModelDuplicate(SRC src);

	// stringからSRCに変換（ステージオブジェクトがstringで管理されているため）
	SRC StringToSRC(const std::string& name);

	//シーンごとにデータを読み込むことにする
	void InitTitle(void);
	void InitMovie(void);
	void InitSelect(void);
	void InitTutorial(void);
	void InitGame(void);
	void InitResult(void);

private:

	// 静的インスタンス
	static ResourceManager* instance_;

	// リソース管理の対象
	std::map<SRC, std::unique_ptr<Resource>> resourcesMap_;

	// 読み込み済みリソース
	std::map<SRC, Resource&> loadedMap_;

	Resource dummy_;

	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	ResourceManager(void);
	ResourceManager(const ResourceManager& manager) = default;
	~ResourceManager(void) = default;

	// 内部ロード
	Resource& _Load(SRC src);

};
