#include <DxLib.h>
#include "../Libs/ImGui/imgui.h"
#include "../Common/DebugDrawFormat.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/GameSystem/OrderManager.h"
#include "../Manager/Generic/InputManager.h"
#include "Order.h"

Order::Order(void)
{
	timer_ = -1.0f;
}

Order::~Order(void)
{
}

void Order::Init(void)
{
	timer_ = 0.0f;
}

void Order::Update(void)
{
	//制限時間から経過時間を引いていく
	timer_ -= SceneManager::GetInstance().GetDeltaTime();

	//ImGuiの操作を行う
	//UpdateDebugImGui();
}

void Order::Draw(void)
{
#ifdef _DEBUG
	//注文内容取得用
	OrderManager::Order order = OrderManager::GetInstance().GetOrder();

	//注文に合わせて四角の色を変える
	int startX = DebugDrawFormat::GetFormatSize("注文 : %d,%d", order.drink_, order.sweets_);
	startX = startX * 1.5;//フォントサイズが1.5倍なので
	int scale = 25;
	int endX = startX + scale;
	int startY = 60;
	int endY = startY + scale;
	int drinkCol = GetColor(0,0,0);

	if (order.drink_ == OrderManager::DRINK::HOT)
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
	switch (order.sweets_)
	{
	case OrderManager::SWEETS::NONE:
		foodCol = GetColor(0, 0, 0);
		break;

	case OrderManager::SWEETS::CHOCO:
		foodCol = GetColor(132, 98, 68);
		break;

	case OrderManager::SWEETS::STRAWBERRY:
		foodCol = GetColor(255, 198, 244);
		break;
	default:
		break;
	}
	//食べ物用
	DrawBox(endX + scale, startY, endX + (scale * 2), endY, foodCol, true);
#endif // _DEBUG
}

//void Order::UpdateImgGUI(void)
//{
//	//ウィンドウタイトル&開始処理
//	ImGui::Begin("Box");
//	//大きさ
//	ImGui::Text("scale");
//	ImGui::InputFloat("SclX", &transform_.scl.x);
//	ImGui::InputFloat("SclY", &transform_.scl.y);
//	ImGui::InputFloat("SclZ", &transform_.scl.z);
//	//角度
//	VECTOR rotDeg = VECTOR();
//	rotDeg.x = AsoUtility::Rad2DegF(transform_.rot.x);
//	rotDeg.y = AsoUtility::Rad2DegF(transform_.rot.y);
//	rotDeg.z = AsoUtility::Rad2DegF(transform_.rot.z);
//	ImGui::Text("angle(deg)");
//	ImGui::SliderFloat("RotX", &rotDeg.x, 0.0f, 360.0f);
//	ImGui::SliderFloat("RotY", &rotDeg.y, 0.0f, 360.0f);
//	ImGui::SliderFloat("RotZ", &rotDeg.z, 0.0f, 360.0f);
//	transform_.rot.x = AsoUtility::Deg2RadF(rotDeg.x);
//	transform_.rot.y = AsoUtility::Deg2RadF(rotDeg.y);
//	transform_.rot.z = AsoUtility::Deg2RadF(rotDeg.z);
//	//位置
//	ImGui::Text("position");
//	//構造体の先頭ポインタを渡し、xyzと連続したメモリ配置へアクセス
//	ImGui::InputFloat3("Pos", &transform_.pos.x);
//	//終了処理
//	ImGui::End();
//}
