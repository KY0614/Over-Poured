#include <DxLib.h>
#include "../../Application.h"
#include "Interact2D.h"

Interact2D::Interact2D()
{
	mode_ = MODE::GAME_3D; // 初期モードを3Dゲームに設定

	frame_ = 0;

	//状態管理
	stateChanges_.emplace(MODE::GAME_3D, std::bind(&Interact2D::ChangeMode3DGame, this));
	stateChanges_.emplace(MODE::MACHINE_2D, std::bind(&Interact2D::ChangeModeMachine2D, this));
	stateChanges_.emplace(MODE::ICE_2D, std::bind(&Interact2D::ChangeModeIce2D, this));
	stateChanges_.emplace(MODE::LIDRACK_2D, std::bind(&Interact2D::ChangeModeLidRack2D, this));
}

Interact2D::~Interact2D()
{
}

void Interact2D::Init(void)
{
	ChangeMode(MODE::GAME_3D); // 初期化時は3Dゲームモードに設定
}

void Interact2D::Update(void)
{
	//更新ステップ
	modeUpdate_();
}

void Interact2D::Draw(void)
{
	modeDraw_();
}

void Interact2D::ChangeMode(MODE mode)
{
	mode_ = mode;

	//各状態遷移の初期処理
	stateChanges_[mode_]();
}

void Interact2D::ChangeMode3DGame(void)
{
	modeUpdate_ = std::bind(&Interact2D::DisappearUpdate, this);
	modeDraw_ = std::bind(&Interact2D::ProcessDraw, this);
}

void Interact2D::ChangeModeMachine2D(void)
{
	modeUpdate_ = std::bind(&Interact2D::AppearUpdate, this);
	modeDraw_ = std::bind(&Interact2D::ProcessDraw, this);
}

void Interact2D::ChangeModeIce2D(void)
{
	modeUpdate_ = std::bind(&Interact2D::UpdateIce2D, this);
	modeDraw_ = std::bind(&Interact2D::DrawIce2D, this);
}

void Interact2D::ChangeModeLidRack2D(void)
{
	modeUpdate_ = std::bind(&Interact2D::UpdateLidRack2D, this);
	modeDraw_ = std::bind(&Interact2D::DrawLidRack2D, this);
}

void Interact2D::AppearUpdate(void)
{
	if (++frame_ >= 20) 
	{
		modeUpdate_ = std::bind(&Interact2D::UpdateMachine2D, this);
		modeDraw_ = std::bind(&Interact2D::DrawMachine2D, this);
		return;
	}
}

void Interact2D::DisappearUpdate(void)
{
	if (--frame_ <= 0) {
		modeUpdate_ = std::bind(&Interact2D::Update3DGame, this);
		modeDraw_ = std::bind(&Interact2D::Draw3DGame, this);
		return;
	}
}

void Interact2D::Update3DGame(void)
{
}

void Interact2D::UpdateMachine2D(void)
{
	// TODO: 2Dマシンモードの更新処理
}

void Interact2D::UpdateIce2D(void)
{
	// TODO: 2D製氷機モードの更新処理
}

void Interact2D::UpdateLidRack2D(void)
{
	// TODO: 2Dリッドラックモードの更新処理
}

void Interact2D::ProcessDraw(void)
{
	int centerY = Application::SCREEN_SIZE_Y / 2;//画面中心Y
	int frameHalfHeight = (Application::SCREEN_SIZE_Y - 20 * 2) / 2;//枠の高さの半分

	//出現・消滅時の高さ変化率(0.0～1.0)
	float rate = static_cast<float>(frame_) /
		static_cast<float>(20);

	frameHalfHeight *= rate;

	//白っぽいセロファン
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 168);
	DrawBox(20, centerY - frameHalfHeight,
		Application::SCREEN_SIZE_X - 20, centerY + frameHalfHeight,
		0xfffffff, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	//白枠
	DrawBoxAA(20, centerY - frameHalfHeight,
		Application::SCREEN_SIZE_X - 20, centerY + frameHalfHeight,
		0xfffffff, false, 3.0f);
}

void Interact2D::Draw3DGame(void)
{
	// TODO: 3Dゲームモードの描画処理
}

void Interact2D::DrawMachine2D(void)
{
	// TODO: 2Dマシンモードの描画処理
	//白っぽいセロファン
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 168);
	DrawBox(20, 20,
		Application::SCREEN_SIZE_X - 20, Application::SCREEN_SIZE_Y - 20,
		0xfffffff, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	//白枠
	DrawBoxAA(20, 20,
		Application::SCREEN_SIZE_X - 20, Application::SCREEN_SIZE_Y - 20,
		0xfffffff, false, 3.0f);
	DrawString(20 + 10, 20 + 10, L"Machine Draw2D", 0x0000ff);
}

void Interact2D::DrawIce2D(void)
{
	// TODO: 2D製氷機モードの描画処理
		// TODO: 2Dマシンモードの描画処理
	//白っぽいセロファン
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 168);
	DrawBox(20, 20,
		Application::SCREEN_SIZE_X - 20, Application::SCREEN_SIZE_Y - 20,
		0xfffffff, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	//白枠
	DrawBoxAA(20, 20,
		Application::SCREEN_SIZE_X - 20, Application::SCREEN_SIZE_Y - 20,
		0xfffffff, false, 3.0f);
	DrawString(20 + 10, 20 + 10, L"Ice Draw2D", 0x0000ff);
}

void Interact2D::DrawLidRack2D(void)
{
	// TODO: 2Dマシンモードの描画処理
//白っぽいセロファン
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 168);
	DrawBox(20, 20,
		Application::SCREEN_SIZE_X - 20, Application::SCREEN_SIZE_Y - 20,
		0xfffffff, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	//白枠
	DrawBoxAA(20, 20,
		Application::SCREEN_SIZE_X - 20, Application::SCREEN_SIZE_Y - 20,
		0xfffffff, false, 3.0f);
	DrawString(20 + 10, 20 + 10, L"lidRack Draw2D", 0x0000ff);
}