#pragma once
#include <memory>
#include <vector>
#include <map>
#include <functional>
#include <DxLib.h>
#include "Stage.h"
#include "ActorBase.h"

class AnimationController;
class Collider;
class Capsule;

class Player : public ActorBase
{

public:

	//スピード
	static constexpr float SPEED_MOVE = 5.0f;
	static constexpr float SPEED_RUN = 10.0f;

	//回転完了までの時間
	static constexpr float TIME_ROT = 1.0f;

	//煙エフェクト発生間隔
	static constexpr float TERM_FOOT_SMOKE = 0.3f;

	//状態
	enum class STATE
	{
		NONE,
		PLAY,
	};

	//アニメーション種別
	enum class ANIM_TYPE
	{
		IDLE,
		RUN,
		FAST_RUN,
		JUMP,
		FALLING
	};

	//コンストラクタ
	Player(void);

	//デストラクタ
	~Player(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;

	//衝突判定に用いられるコライダ制御
	void AddCollider(std::weak_ptr<Collider> collider);

	void ClearCollider(void);

	//衝突用カプセルの取得
	const Capsule& GetCapsule(void) const;

	bool IsPlay(void);

	VECTOR GetHitPos(void);
	VECTOR GetHitNormal(void);

	void UpdateDebugImGui(void);

private:
	Transform spereTran_;

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
	VECTOR gravHitPosDown_;
	VECTOR gravHitPosUp_;
	
	//丸影
	int imgShadow_;

	//カプセル
	std::unique_ptr<Capsule> capsule_;

	//足煙エフェクト
	int effectSmokeResId_;
	int effectSmokePlayId_;
	float stepFootSmoke_;	

	//フレームごとの移動値
	VECTOR moveDiff_;
	
	//傾斜角
	float slopeAngleDeg_;
	//傾斜の力
	VECTOR slopePow_;
	//傾斜の方向
	VECTOR slopeDir_;
	//衝突している地面ポリゴンの法線
	VECTOR hitNormal_;
	//衝突している地面との座標
	VECTOR hitPos_;
	
	void InitAnimation(void);

	//状態遷移
	void ChangeState(STATE state);
	void ChangeStateNone(void);
	void ChangeStatePlay(void);

	//更新ステップ
	void UpdateNone(void);
	void UpdatePlay(void);

	//描画系
	void DrawDebug(void);
	void DrawShadow(void);

	//操作 
	void ProcessMove(void);

	//回転
	void SetGoalRotate(double rotRad);
	void Rotate(void);

	//衝突判定
	void Collision(void);
	
	//着地モーション終了
	bool IsEndLanding(void);

	void CollisionCapsule(void);

	//足煙エフェクト
	void EffectFootSmoke(void);
};
