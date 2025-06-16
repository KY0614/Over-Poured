#pragma once
#include <memory>
#include <map>
#include <functional>

class Interact2D
{
public:

	enum class MODE {
		GAME_3D,		// 3Dゲームモード
		MACHINE_2D,		// 2Dマシンモード
		ICE_2D,			// 2D製氷機モード
		LIDRACK_2D,		// 2Dリッドラックモード
	};

	Interact2D();
	~Interact2D();

	void Init(void);
	void Update(void);
	void Draw(void);

	//状態遷移
	void ChangeMode(MODE mode);

private:

	//モード管理
	MODE mode_;

	//状態管理(状態遷移時初期処理)
	std::map<MODE, std::function<void(void)>> stateChanges_;

	//状態管理
	std::map<MODE, std::function<void(void)>> updateFunc_;
	std::map<MODE, std::function<void(void)>> drawFunc_;

	//状態管理(更新ステップ)
	std::function<void(void)> modeUpdate_;
	std::function<void(void)> modeDraw_;

	int frame_;

	//状態遷移
	void ChangeMode3DGame(void);
	void ChangeModeMachine2D(void);
	void ChangeModeIce2D(void);
	void ChangeModeLidRack2D(void);

	//更新ステップ
	void AppearUpdate(void);
	void DisappearUpdate(void);
	void Update3DGame(void);
	void UpdateMachine2D(void);
	void UpdateIce2D(void);
	void UpdateLidRack2D(void);

	//描画ステップ
	void ProcessDraw(void);
	void Draw3DGame(void);
	void DrawMachine2D(void);
	void DrawIce2D(void);
	void DrawLidRack2D(void);
};