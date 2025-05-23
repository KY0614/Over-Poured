#include "../Libs/ImGui/imgui.h"
#include "../../Common/DebugDrawFormat.h"
#include "../Common/Cube.h"
#include "../../Utility/AsoUtility.h"
#include "../../Utility/StringUtility.h"
#include "StageObjectLibrary.h"
#include "StageObject.h"

StageObject::StageObject(const std::string objId, const float width,
	const float height, const float depth):
	objId_(objId),param_(StageObjectLibrary::ObjectParams()),width_(width),
	height_(height),depth_(depth)
{
	followPos_ = AsoUtility::VECTOR_ZERO;
	state_ = STATE::NONE;
}

StageObject::~StageObject(void)
{
}

void StageObject::Init(void)
{
	object_ = StageObjectLibrary::LoadData(objId_);

	param_ = object_.second;

	cube_ = std::make_unique<Cube>(transform_);

	state_ = STATE::PLACED;

	transform_.Update();

#ifdef _DEBUG

	//当たり判定用の球体
	sphereTran_.Update();

	sphereTran_.scl = AsoUtility::VECTOR_ONE;
	sphereTran_.pos = { 221.0f, 0.0f, 139.0f };
	sphereTran_.quaRot = Quaternion();
	sphereTran_.quaRotLocal =
		Quaternion::Euler({ 0.0f, AsoUtility::Deg2RadF(180.0f), 0.0f });

#endif // _DEBUG
}

void StageObject::Update(void)
{
	switch (state_)
	{
	case StageObject::STATE::NONE:
		break;
	case StageObject::STATE::PLACED:
		UpdatePlaced();
		break;
	case StageObject::STATE::HOLD:
		UpdateHold();
		break;
	default:
		break;
	}

	transform_.Update();

	//UpdateDebugImGui();
}

void StageObject::Draw(void)
{
#ifdef _DEBUG

	//とりあえず仮のモデルとして色違いのCubeを生成する

	int col = 0x000000;
	COLOR_U8  retCol;

	float rad = 30.0f;
	VECTOR pos = transform_.pos;

	if (objId_ == "Coffee_Machine")col = 0x3f312b,rad = 35.0f;
	else if (objId_ == "Ice_Dispenser")col = 0x4682b4,rad = 35.0f, pos.y -= 10.0f;
	else if (objId_ == "Table")col = 0xd2b48c;
	else if (objId_ == "Sweets_Choco")col = 0xa0522d;
	else if (objId_ == "Sweets_Strawberry")col = 0xdda0dd;
	else if (objId_ == "Cup_Hot")col = 0xcd5c5c,rad = 20.0f;
	else if (objId_ == "Cup_Ice")col = 0x87ceeb,rad = 20.0f;
	else if (objId_ == "Cup_With_Ice")col = 0x6495ed;
	else if (objId_ == "Lids")col = 0xa9a9a9;
	else if (objId_ == "Dust_Box")col = 0x2f4f4f;
	//int型をCOLOR_U8へ変換
	retCol.r = (col >> 16) & 0xFF;
	retCol.g = (col >> 8) & 0xFF;
	retCol.b = col & 0xFF;
	retCol.a = 255;

	cube_->MakeBox(transform_.pos, width_, height_, depth_, retCol);

	DrawSphere3D(sphereTran_.pos , rad, 8, col, col, false);
	//cube_->Draw();

	VECTOR screenPos = ConvWorldPosToScreenPos(transform_.pos);
	// 変換成功
	DrawString(static_cast<int>(screenPos.x) - 25, static_cast<int>(screenPos.y) - 50,
		StringUtility::StringToWstring(objId_.c_str()).c_str(), GetColor(255, 255, 255));

	int line = 3;	//行
	int lineHeight = 30;	//行
	//DebugDrawFormat::FormatStringRight(L"param", 0, line,lineHeight);
	//DebugDrawFormat::FormatStringRight(L"id : %ws", StringUtility::StringToWstring(param_.id_).c_str(), line, lineHeight);
	//DebugDrawFormat::FormatStringRight(L"name : %s", StringUtility::StringToWstring(param_.name_).c_str(), line, lineHeight);
	//DebugDrawFormat::FormatStringRight(L"modelFile_ : %s", StringUtility::StringToWstring(param_.modelFile_).c_str(), line, lineHeight);
	//DebugDrawFormat::FormatStringRight(L"placeable_ : %d", param_.placeable_, line, lineHeight);
	//DebugDrawFormat::FormatStringRight(L"carryable_ : %d", param_.carryable_, line, lineHeight);
	//DebugDrawFormat::FormatStringRight(L"interactable_ : %d", param_.interactable_, line, lineHeight);
	//DebugDrawFormat::FormatStringRight(L"interactTime %2.f", param_.interactTime, line, lineHeight);
	//DebugDrawFormat::FormatStringRight(L"type : %s", StringUtility::StringToWstring(param_.category_).c_str(), line, lineHeight);
	//
	//for (auto& item : param_.acceptedItems_)
	//{
	//	DebugDrawFormat::FormatStringRight(L"acceptedItem : %s", StringUtility::StringToWstring(item).c_str(), line, lineHeight);
	//}
	//for (auto& produce : param_.produces_)
	//{
	//	DebugDrawFormat::FormatStringRight(L"produce : %s", StringUtility::StringToWstring(produce).c_str(), line, lineHeight);
	//}
#endif // _DEBUG

}

void StageObject::SetPos(VECTOR pos)
{
	transform_.pos = pos;
	sphereTran_.pos = pos;
}

void StageObject::UpdatePlaced(void)
{
}

void StageObject::UpdateHold(void)
{
	transform_.pos = followPos_;
}

void StageObject::UpdateDebugImGui(void)
{
	//ウィンドウタイトル&開始処理
	ImGui::Begin("object");

	//位置
	ImGui::Text("position");
	//構造体の先頭ポインタを渡し、xyzと連続したメモリ配置へアクセス
	ImGui::InputFloat3("Pos", &transform_.pos.x);
	ImGui::SliderFloat("PosX", &transform_.pos.x, -300.0f, 500.0f);
	ImGui::SliderFloat("PosY", &transform_.pos.y, -300.0f, 500.0f);
	ImGui::SliderFloat("PosZ", &transform_.pos.z, -300.0f, 1000.0f);
	//終了処理
	ImGui::End();
}
