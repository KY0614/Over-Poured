#pragma once
#include <memory>
#include <vector>
#include <map>
#include <functional>
#include <DxLib.h>
#include "Stage/StageManager.h"
#include "Order/Order.h"
#include "ActorBase.h"

class AnimationController;
class Collider;
class Capsule;
class Sphere;

class Player : public ActorBase
{

public:

	//スピード
	static constexpr float SPEED_MOVE = 5.0f;	//歩きスピード
	static constexpr float SPEED_RUN = 7.0f;	//走りスピード

	//回転完了までの時間
	static constexpr float TIME_ROT = 0.5f;

	//煙エフェクト発生間隔
	static constexpr float TERM_FOOT_SMOKE = 0.3f;

	//状態
	enum class STATE
	{
		NONE,
		PLAY,
		STOP,
	};

	//アニメーション種別
	enum class ANIM_TYPE
	{
		IDLE,
		WALK,
		RUN,
		IDLE_HOLD,
		WALK_HOLD,
		JUMP,
	};

	//コンストラクタ
	Player(void);

	//デストラクタ
	~Player(void);

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init(void) override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update(void) override;

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw(void) override;

	/// <summary>
	/// 衝突判定に用いられるコライダの追加
	/// </summary>
	/// <param name="collider">衝突判定するコライダ</param>
	void AddCollider(const std::weak_ptr<Collider> collider);

	/// <summary>
	/// 追加したコライダを全部削除する関数
	/// </summary>
	void ClearCollider(void);

	/// <summary>
	/// 衝突用カプセルの取得
	/// </summary>
	/// <returns>プレイヤーに追従しているカプセル</returns>
	const Capsule& GetCapsule(void) const;

	/// <summary>
	/// オブジェクト判定用の球体の取得
	/// </summary>
	/// <returns>衝突判定用の球体</returns>
	const Sphere& GetSphere(void) const { return *sphere_; }

	/// <summary>
	/// プレイヤーがPLAY状態かどうか
	/// </summary>
	/// <param name=""></param>
	/// <returns>true:PLAY中　false:PLAY以外</returns>
	const bool& IsPlay(void)const;

	/// <summary>
	/// プレイヤーがオブジェクトを持っているかどうか設定する
	/// </summary>
	/// <param name="hold">true:所持中　false:未所持</param>
	void SetIsHolding(const bool hold) { isHolding_ = hold; }

	/// <summary>
	/// プレイヤーの所持状態を取得する
	/// </summary>
	/// <returns>true:所持中　false:未所持</returns>
	const bool& GetIsHolding(void)const { return isHolding_; }

	/// <summary>
	/// プレイヤーの所持しているアイテムのIDを取得する
	/// </summary>
	/// <returns>持っているアイテムのID</returns>
	const std::string& GetHoldItem(void)const { return holdItemId_; }

	/// <summary>
	/// プレイヤーの所持しているアイテムのIDを設定する
	/// </summary>
	/// <param name="item">持たせるアイテムのID</param>
	void SetHoldItem(const std::string item) { holdItemId_ = item; }

	/// <summary>
	/// 指定されたSTATEに状態を変更する
	/// </summary>
	/// <param name="state">新しく設定するSTATE型の状態</param>
	void ChangeState(const STATE state);

private:

	//アニメーション
	std::unique_ptr<AnimationController> animationController_;

	//状態管理
	STATE state_;

	//状態管理(状態遷移時初期処理)
	std::map<STATE, std::function<void(void)>> stateChanges_;

	//状態管理(更新ステップ)
	std::function<void(void)> stateUpdate_;

	//移動スピード
	float speed_;
	
	//移動方向
	VECTOR moveDir_;
	
	//移動量
	VECTOR movePow_;
	
	//移動後の座標
	VECTOR movedPos_;

	//回転
	Quaternion playerRotY_;
	Quaternion goalQuaRot_;
	float stepRotTime_;
	
	//衝突判定に用いられるコライダ
	std::vector<std::weak_ptr<Collider>> colliders_;
	
	//衝突チェック
	VECTOR gravHitPosDown_;	//下方向(カプセルの下の球体)
	VECTOR gravHitPosUp_;	//上方向(カプセルの上の球体)
	
	//丸影
	int imgShadow_;

	//カプセル
	std::unique_ptr<Capsule> capsule_;
	//球体
	std::unique_ptr<Sphere> sphere_;

	//足煙エフェクト
	int effectSmokeResId_;	//エフェクトリソースID
	int effectSmokePlayId_;	//エフェクト再生ID
	float stepFootSmoke_;	//エフェクト発生までの時間計測用

	//フレームごとの移動値
	VECTOR moveDiff_;

	//プレイヤーがオブジェクトを持っているかどうか
	bool isHolding_;

	//持っているアイテムのID
	std::string holdItemId_;

	int chestFrmNo_;
	VECTOR chestPos_;

	/// <summary>
	/// アニメーション初期化処理
	/// </summary>
	void InitAnimation(void);

	//状態遷移

	/// <summary>
	/// NONE状態に遷移する処理
	/// </summary>
	void ChangeStateNone(void);
	/// <summary>	
	/// PLAY状態に遷移する処理
	/// </summary>
	void ChangeStatePlay(void);
	/// <summary>
	/// STOP状態に遷移する処理
	/// </summary>
	void ChangeStateStop(void);

	//更新ステップ

	/// <summary>
	/// NONE状態の更新処理
	/// </summary>
	void UpdateNone(void);
	/// <summary>
	/// PLAY状態の更新処理
	/// </summary>
	void UpdatePlay(void);
	/// <summary>
	/// STOP状態の更新処理
	/// </summary>
	void UpdateStop(void);

	/// <summary>
	/// 影を描画する処理(影は画像で対応)
	/// </summary>
	void DrawShadow(void);

	//操作 

	/// <summary>
	/// 移動処理
	/// </summary>
	void ProcessMove(void);

	/// <summary>
	/// 回転処理(目標角度を設定)
	/// </summary>
	/// <param name="rotRad">目標角度</param>
	void SetGoalRotate(const double rotRad);

	/// <summary>
	/// 回転する処理(徐々に回転)
	/// </summary>
	void Rotate(void);

	/// <summary>
	/// 衝突判定処理
	/// </summary>
	void Collision(void);
	
	/// <summary>
	/// カプセルの衝突判定処理
	/// </summary>
	void CollisionCapsule(void);

	/// <summary>
	/// 足煙エフェクト発生処理
	/// </summary>
	void EffectFootSmoke(void);
};
