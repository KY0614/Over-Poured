#include <string>
#include <cassert>
#include<EffekseerForDXLib.h>
#include "../Application.h"
#include "../Utility/AsoUtility.h"
#include "../Libs/ImGui/imgui.h"
#include "../Common/DebugDrawFormat.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/ResourceManager.h"
#include "../Manager/Generic/InputManager.h"
#include "../Manager/Generic/Camera.h"
#include "Common/AnimationController.h"
#include "Common/Capsule.h"
#include "Common/Collider.h"
#include "Common/Sphere.h"
#include "Common/Cube.h"
#include "Order/Order.h"
#include "Player.h"

Player::Player(void)
{

	animationController_ = nullptr;
	state_ = STATE::NONE;

	//状態管理
	stateChanges_.emplace(STATE::NONE, std::bind(&Player::ChangeStateNone, this));
	stateChanges_.emplace(STATE::PLAY, std::bind(&Player::ChangeStatePlay, this));

	//衝突チェック
	gravHitPosDown_ = AsoUtility::VECTOR_ZERO;
	gravHitPosUp_ = AsoUtility::VECTOR_ZERO;

	isHolding_ = false;
}

Player::~Player(void)
{
}

void Player::Init(void)
{

	//モデルの基本設定
	transform_.SetModel(ResourceManager::GetInstance().LoadModelDuplicate(
		ResourceManager::SRC::CUSTOMER));
	transform_.scl = AsoUtility::VECTOR_ONE;
	transform_.pos = { 0.0f, 30.0f, 0.0f };
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal =
		Quaternion::Euler({ 0.0f, AsoUtility::Deg2RadF(180.0f), 0.0f });
	transform_.Update();

	//丸影画像
	imgShadow_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::PLAYER_SHADOW).handleId_;

	//カプセルコライダ
	capsule_ = std::make_unique<Capsule>(transform_);
	capsule_->SetLocalPosTop({ 0.0f, 20.0f, 0.0f });
	capsule_->SetLocalPosDown({ 0.0f, -10.0f, 0.0f });
	capsule_->SetRadius(20.0f);
	
	////オブジェクト用コライダ
	//capsule2_ = std::make_unique<Capsule>(transform_);
	//capsule2_->SetLocalPosTop({ 0.0f, 80.0f, 50.0f });
	//capsule2_->SetLocalPosDown({ 0.0f, 0.0f, 50.0f });
	//capsule2_->SetRadius(30.0f);
	
	//オブジェクト用コライダ
	sphere_ = std::make_unique<Sphere>(transform_);
	sphere_->SetLocalPos({ 0.0f, 60.0f, 50.0f });
	sphere_->SetRadius(20.0f);
	

	//足煙エフェクト
	effectSmokeResId_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::FOOT_SMOKE).handleId_;	

	//アニメーションの設定
	InitAnimation();

	//初期状態
	ChangeState(STATE::PLAY);

	stepFootSmoke_ = TERM_FOOT_SMOKE;

#ifdef _DEBUG

	//カウンター前の当たり判定用の球体
	sphereTran_.Update();

	sphereTran_.scl = AsoUtility::VECTOR_ONE;
	sphereTran_.pos = { 221.0f, 0.0f, 139.0f };
	sphereTran_.quaRot = Quaternion();
	sphereTran_.quaRotLocal =
		Quaternion::Euler({ 0.0f, AsoUtility::Deg2RadF(180.0f), 0.0f });

#endif // _DEBUG

	data_.drink_ = Order::DRINK::ICE;
	data_.sweets_ = Order::SWEETS::NONE;
	isHolding_ = false;
}

void Player::Update(void)
{
	transform_.pos.y = 30.0f;

	//更新ステップ
	stateUpdate_();

	transform_.Update();
	sphereTran_.Update();

	//アニメーション再生
	animationController_->Update();

	//ImGuiの操作を行う
	//UpdateDebugImGui();
}

void Player::Draw(void)
{
	DrawSphere3D(sphereTran_.pos, 30, 8, 0xffff00, 0xffff00, false);

	//capsule_->Draw();
	//capsule2_->Draw();
	//cube_->Draw();
	sphere_->Draw();

	//モデルの描画
	MV1DrawModel(transform_.modelId);

	//丸影描画
	DrawShadow();

	//デバッグ用描画
	DrawDebug();

}

void Player::AddCollider(std::weak_ptr<Collider> collider)
{
	colliders_.emplace_back(collider);
}

void Player::ClearCollider(void)
{
	colliders_.clear();
}

const Capsule& Player::GetCapsule(void) const
{
	return *capsule_;
}

bool Player::IsPlay(void)
{
	return state_ == STATE::PLAY;
}

void Player::SurveItem(void)
{
	isHolding_ = false;
	holdItemId_ = "";
}

void Player::ProcessSelect(void)
{
	InputManager& ins = InputManager::GetInstance();

	if (ins.IsTrgDown(KEY_INPUT_Q))
	{
		data_.drink_ = Order::DRINK::HOT;
	}
	if (ins.IsTrgDown(KEY_INPUT_E))
	{
		data_.drink_ = Order::DRINK::ICE;
	}

	switch (data_.sweets_)
	{
	case Order::SWEETS::NONE:
		if (ins.IsTrgDown(KEY_INPUT_M))
		{
			data_.sweets_ = Order::SWEETS::CHOCO;
		}
		break;

	case Order::SWEETS::CHOCO:
		if (ins.IsTrgDown(KEY_INPUT_M))
		{
			data_.sweets_ = Order::SWEETS::STRAWBERRY;
		}
		break;

	case Order::SWEETS::STRAWBERRY:
		if (ins.IsTrgDown(KEY_INPUT_M))
		{
			data_.sweets_ = Order::SWEETS::NONE;
		}
		break;
	default:
		break;
	}
}

void Player::UpdateDebugImGui(void)
{
	//ウィンドウタイトル&開始処理
	ImGui::Begin("Player");
	//大きさ
	ImGui::Text("scale");
	ImGui::InputFloat("SclX", &transform_.scl.x);
	ImGui::InputFloat("SclY", &transform_.scl.y);
	ImGui::InputFloat("SclZ", &transform_.scl.z);

	//位置
	ImGui::Text("position");
	//構造体の先頭ポインタを渡し、xyzと連続したメモリ配置へアクセス
	ImGui::InputFloat3("Pos", &transform_.pos.x);
	ImGui::SliderFloat("PosX", &transform_.pos.x, -500.0f, 360.0f);
	ImGui::SliderFloat("PosY", &transform_.pos.y, -500.0f, 360.0f);
	ImGui::SliderFloat("PosZ", &transform_.pos.z, -500.0f, 1000.0f);
	//終了処理
	ImGui::End();
}

void Player::InitAnimation(void)
{

	std::string path = Application::PATH_MODEL + "Player/";
	animationController_ = std::make_unique<AnimationController>(transform_.modelId);
	animationController_->Add((int)ANIM_TYPE::IDLE, path + "Idle.mv1", 20.0f);
	animationController_->Add((int)ANIM_TYPE::RUN, path + "Run.mv1", 20.0f);
	animationController_->Add((int)ANIM_TYPE::FAST_RUN, path + "FastRun.mv1", 20.0f);
	animationController_->Add((int)ANIM_TYPE::JUMP, path + "Jump.mv1", 60.0f);
	animationController_->Add((int)ANIM_TYPE::FALLING, path + "Falling.mv1", 80.0f);

	animationController_->Play((int)ANIM_TYPE::IDLE);
}

void Player::ChangeState(STATE state)
{

	//状態変更
	state_ = state;

	//各状態遷移の初期処理
	stateChanges_[state_]();

}

void Player::ChangeStateNone(void)
{
	stateUpdate_ = std::bind(&Player::UpdateNone, this);
}

void Player::ChangeStatePlay(void)
{
	stateUpdate_ = std::bind(&Player::UpdatePlay, this);
}

void Player::UpdateNone(void)
{
}

void Player::UpdatePlay(void)
{
	//if (!isHolding_)holdItemId_ = "";
	if (holdItemId_ == "")isHolding_ = false;
	else isHolding_ = true;

	if(holdItemId_ == "Hot_Coffee")data_.drink_ = Order::DRINK::HOT;

	//移動処理
	ProcessMove();

	//選択処理(仮の機能)
	ProcessSelect();

	//移動方向に応じた回転
	Rotate();

	//衝突判定
	Collision();

	//歩きエフェクト
	EffectFootSmoke();

	//重力方向に沿って回転させる
	transform_.quaRot = Quaternion::Quaternion();
	transform_.quaRot = transform_.quaRot.Mult(playerRotY_);
}

void Player::DrawDebug(void)
{
	SetFontSize(24);
	int line = 0;
	DebugDrawFormat::FormatStringRight(L"提供品 : %d, %d", data_.drink_, data_.sweets_,line);
	SetFontSize(16);

	auto orders = data_;

	//注文に合わせて四角の色を変える
	int startX = Application::SCREEN_SIZE_X - (DebugDrawFormat::GetFormatSize(L"提供品 : %d, %d", data_.drink_, data_.sweets_) * 1.5);
	//startX = startX * 1.5;//フォントサイズが1.5倍なので
	int scale = 25;
	int endX = startX + scale;
	int startY = 30;
	int endY = startY + scale;
	int drinkCol = GetColor(0, 0, 0);

	if (orders.drink_ == Order::DRINK::HOT)
	{
		drinkCol = GetColor(255, 0, 0);
	}
	else
	{
		drinkCol = GetColor(0, 255, 255);
	}
	//飲み物用
	DrawBox(startX, startY, endX, endY, drinkCol, true);

	int foodCol = GetColor(0, 0, 0);
	switch (orders.sweets_)
	{
	case Order::SWEETS::NONE:
		foodCol = GetColor(0, 0, 0);
		break;

	case Order::SWEETS::CHOCO:
		foodCol = GetColor(132, 98, 68);
		break;

	case Order::SWEETS::STRAWBERRY:
		foodCol = GetColor(255, 198, 244);
		break;
	default:
		break;
	}

	//食べ物用
	DrawBox(endX + scale, startY, endX + (scale * 2), endY, foodCol, true);
}

void Player::DrawShadow(void)
{
	int i, j;
	MV1_COLL_RESULT_POLY_DIM HitResDim;
	MV1_COLL_RESULT_POLY* HitRes;
	VERTEX3D Vertex[3];
	VECTOR SlideVec;
	int ModelHandle;

	//ライティングを無効にする
	SetUseLighting(FALSE);

	//Ｚバッファを有効にする
	SetUseZBuffer3D(TRUE);

	//テクスチャアドレスモードを CLAMP にする( テクスチャの端より先は端のドットが延々続く )
	SetTextureAddressMode(DX_TEXADDRESS_CLAMP);

	//影を落とすモデルの数だけ繰り返し
	for (auto c : colliders_)
	{
		//チェックするモデルは、jが0の時はステージモデル、1以上の場合はコリジョンモデル

		ModelHandle = c.lock()->modelId_;

		float PLAYER_SHADOW_HEIGHT = 700.0f;
		float PLAYER_SHADOW_SIZE = 50.0f;

		//プレイヤーの直下に存在する地面のポリゴンを取得
		HitResDim = MV1CollCheck_Capsule(ModelHandle, -1, transform_.pos, VAdd(transform_.pos, VGet(0.0f, -PLAYER_SHADOW_HEIGHT, 0.0f)), PLAYER_SHADOW_SIZE);

		//頂点データで変化が無い部分をセット
		Vertex[0].dif = GetColorU8(255, 255, 255, 255);
		Vertex[0].spc = GetColorU8(0, 0, 0, 0);
		Vertex[0].su = 0.0f;
		Vertex[0].sv = 0.0f;
		Vertex[1] = Vertex[0];
		Vertex[2] = Vertex[0];

		//球の直下に存在するポリゴンの数だけ繰り返し
		HitRes = HitResDim.Dim;
		for (i = 0; i < HitResDim.HitNum; i++, HitRes++)
		{
			//ポリゴンの座標は地面ポリゴンの座標
			Vertex[0].pos = HitRes->Position[0];
			Vertex[1].pos = HitRes->Position[1];
			Vertex[2].pos = HitRes->Position[2];

			//ちょっと持ち上げて重ならないようにする
			SlideVec = VScale(HitRes->Normal, 0.5f);
			Vertex[0].pos = VAdd(Vertex[0].pos, SlideVec);
			Vertex[1].pos = VAdd(Vertex[1].pos, SlideVec);
			Vertex[2].pos = VAdd(Vertex[2].pos, SlideVec);

			//ポリゴンの不透明度を設定する
			Vertex[0].dif.a = 0;
			Vertex[1].dif.a = 0;
			Vertex[2].dif.a = 0;
			if (HitRes->Position[0].y > transform_.pos.y - PLAYER_SHADOW_HEIGHT)
				Vertex[0].dif.a = 128 * (1.0f - fabs(HitRes->Position[0].y - transform_.pos.y) / PLAYER_SHADOW_HEIGHT);

			if (HitRes->Position[1].y > transform_.pos.y - PLAYER_SHADOW_HEIGHT)
				Vertex[1].dif.a = 128 * (1.0f - fabs(HitRes->Position[1].y - transform_.pos.y) / PLAYER_SHADOW_HEIGHT);

			if (HitRes->Position[2].y > transform_.pos.y - PLAYER_SHADOW_HEIGHT)
				Vertex[2].dif.a = 128 * (1.0f - fabs(HitRes->Position[2].y - transform_.pos.y) / PLAYER_SHADOW_HEIGHT);

			//ＵＶ値は地面ポリゴンとプレイヤーの相対座標から割り出す
			Vertex[0].u = (HitRes->Position[0].x - transform_.pos.x) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
			Vertex[0].v = (HitRes->Position[0].z - transform_.pos.z) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
			Vertex[1].u = (HitRes->Position[1].x - transform_.pos.x) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
			Vertex[1].v = (HitRes->Position[1].z - transform_.pos.z) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
			Vertex[2].u = (HitRes->Position[2].x - transform_.pos.x) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
			Vertex[2].v = (HitRes->Position[2].z - transform_.pos.z) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;

			//影ポリゴンを描画
			DrawPolygon3D(Vertex, 1, imgShadow_, TRUE);
		}

		//検出した地面ポリゴン情報の後始末
		MV1CollResultPolyDimTerminate(HitResDim);
	}

	//ライティングを有効にする
	SetUseLighting(TRUE);

	//Ｚバッファを無効にする
	SetUseZBuffer3D(FALSE);
}

void Player::ProcessMove(void)
{
	InputManager& ins = InputManager::GetInstance();
	Quaternion cameraRot = mainCamera->GetQuaRotOutX();

	double rotRad = 0;

	//WASDで位置を変える
	VECTOR dir = AsoUtility::VECTOR_ZERO;
	movePow_ = AsoUtility::VECTOR_ZERO;
	if (ins.IsNew(KEY_INPUT_W)) { dir = cameraRot.GetForward();  rotRad = AsoUtility::Deg2RadF(0.0f); }
	if (ins.IsNew(KEY_INPUT_A)) { dir = cameraRot.GetLeft();  rotRad = AsoUtility::Deg2RadF(-90.0f); }
	if (ins.IsNew(KEY_INPUT_S)) { dir = cameraRot.GetBack();  rotRad = AsoUtility::Deg2RadF(180.0f); }
	if (ins.IsNew(KEY_INPUT_D)) { dir = cameraRot.GetRight(); rotRad = AsoUtility::Deg2RadF(90.0f); }

	if (!AsoUtility::EqualsVZero(dir))
	{
		//移動スピード
		speed_ = SPEED_MOVE;
		if (ins.IsNew(KEY_INPUT_RSHIFT))
		{
			speed_ = SPEED_RUN;
		}

		//移動量
		movePow_ = VScale(dir, speed_);

		//回転処理
		SetGoalRotate(rotRad);

		if (IsEndLanding())
		{
			//アニメーション
			if (ins.IsNew(KEY_INPUT_RSHIFT))
			{
				//速く走るアニメーション
				animationController_->Play((int)ANIM_TYPE::FAST_RUN);
			}
			else
			{
				//走るアニメーション
				animationController_->Play((int)ANIM_TYPE::RUN);
			}
		}
	}
	else
	{
		if (IsEndLanding())
		{
			animationController_->Play((int)ANIM_TYPE::IDLE);
		}
	}

}

void Player::SetGoalRotate(double rotRad)
{
	VECTOR cameraRot = mainCamera->GetAngles();
	Quaternion axis =
		Quaternion::AngleAxis(
			(double)cameraRot.y + rotRad, AsoUtility::AXIS_Y);
	
	//現在設定されている回転との角度差を取る
	double angleDiff = Quaternion::Angle(axis, goalQuaRot_);
	
	//しきい値
	if (angleDiff > 0.1)
	{
		stepRotTime_ = TIME_ROT;
	}
	goalQuaRot_ = axis;
}

void Player::Rotate(void)
{
	stepRotTime_ -= SceneManager::GetInstance().GetDeltaTime();
	
	//回転の球面補間
	playerRotY_ = Quaternion::Slerp(
		playerRotY_, goalQuaRot_, (TIME_ROT - stepRotTime_) / TIME_ROT);
}

void Player::Collision(void)
{
	//現在座標を起点に移動後座標を決める
	movedPos_ = VAdd(transform_.pos, movePow_);
	
	//衝突(カプセル)
	CollisionCapsule();
	
	//移動
	moveDiff_ = VSub(movedPos_, transform_.pos);
	transform_.pos = movedPos_;
}

bool Player::IsEndLanding(void)
{
	bool ret = true;

	//アニメーションがジャンプではない
	if (animationController_->GetPlayType() != (int)ANIM_TYPE::JUMP)
	{
		return ret;
	}
	
	//アニメーションが終了しているか
	if (animationController_->IsEnd())
	{
		return ret;
	}

	return false;
}

void Player::CollisionCapsule(void)
{
	//カプセルを移動させる
	Transform trans = Transform(transform_);
	trans.pos = movedPos_;
	trans.Update();
	Capsule cap = Capsule(*capsule_, trans);
	//カプセルとの衝突判定
	for (const auto c : colliders_)
	{
		auto hits = MV1CollCheck_Capsule(
			c.lock()->modelId_, -1,
			cap.GetPosTop(), cap.GetPosDown(), cap.GetRadius());
		//衝突した複数のポリゴンと衝突回避するまで、
		//プレイヤーの位置を移動させる
		for (int i = 0; i < hits.HitNum; i++)
		{
			auto hit = hits.Dim[i];
			//地面と異なり、衝突回避位置が不明なため、何度か移動させる
			//この時、移動させる方向は、移動前座標に向いた方向であったり、
			//衝突したポリゴンの法線方向だったりする
			for (int tryCnt = 0; tryCnt < 10; tryCnt++)
			{
				//再度、モデル全体と衝突検出するには、効率が悪過ぎるので、
				//最初の衝突判定で検出した衝突ポリゴン1枚と衝突判定を取る
				int pHit = HitCheck_Capsule_Triangle(
					cap.GetPosTop(), cap.GetPosDown(), cap.GetRadius(),
					hit.Position[0], hit.Position[1], hit.Position[2]);

				if (pHit)
				{
					//法線の方向にちょっとだけ移動させる
					movedPos_ = VAdd(movedPos_, VScale(hit.Normal, 1.0f));
					//カプセルも一緒に移動させる
					trans.pos = movedPos_;
					trans.Update();
					continue;
				}
				break;
			}
		}
		//検出した地面ポリゴン情報の後始末
		MV1CollResultPolyDimTerminate(hits);
	}
}

void Player::EffectFootSmoke(void)
{
	stepFootSmoke_ -= SceneManager::GetInstance().GetDeltaTime();

	float len = AsoUtility::MagnitudeF(moveDiff_);

	if (len >= 1.0f &&
		stepFootSmoke_ < 0.0f)
	{
		stepFootSmoke_ = TERM_FOOT_SMOKE;

		//エフェクト再生
		effectSmokePlayId_ = PlayEffekseer3DEffect(effectSmokeResId_);

		//大きさ
		float SCALE = 5.0f;
		SetScalePlayingEffekseer3DEffect(effectSmokePlayId_, SCALE, SCALE, SCALE);

		//位置の設定
		SetPosPlayingEffekseer3DEffect(
			effectSmokePlayId_,
			transform_.pos.x,
			transform_.pos.y,
			transform_.pos.z);
	}
}