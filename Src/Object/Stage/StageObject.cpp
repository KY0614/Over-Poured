#include "../Libs/ImGui/imgui.h"
#include "../../Common/DebugDrawFormat.h"
#include "../../Utility/StringUtility.h"
#include "StageObjectLibrary.h"
#include "StageObject.h"

StageObject::StageObject(const std::string objId):objId_(objId),param_(StageObjectLibrary::ObjectParams())
{
}

StageObject::~StageObject(void)
{
}

void StageObject::Init(void)
{
	object_ = StageObjectLibrary::LoadData(objId_);

	param_ = object_.second;
	transform_.Update();
}

void StageObject::Update(void)
{
	transform_.Update();

	UpdateDebugImGui();
}

void StageObject::Draw(void)
{
	int col = 0x000000;
	if (objId_ == "Coffee_Machine")col = 0x3f312b;
	else if (objId_ == "Ice_Dispenser")col = 0x4682b4;
	else if (objId_ == "Table")col = 0xd2b48c;
	else if (objId_ == "Sweets_Choco")col = 0xa0522d;
	else if (objId_ == "Sweets_Strawberry")col = 0xdda0dd;
	else if (objId_ == "Cup_Hot")col = 0xcd5c5c;
	else if (objId_ == "Cup_Ice")col = 0x87ceeb;
	else if (objId_ == "Cup_With_Ice")col = 0x6495ed;
	else if (objId_ == "Lids")col = 0xa9a9a9;
	else if (objId_ == "Dust_Box")col = 0x2f4f4f;
	DrawSphere3D(transform_.pos, 30.0f, 8, col, 0xffffff, true);
	VECTOR screenPos = ConvWorldPosToScreenPos(transform_.pos);
	// 変換成功
	DrawString(static_cast<int>(screenPos.x), static_cast<int>(screenPos.y) - 50,
		StringUtility::StringToWstring(objId_.c_str()).c_str(), GetColor(255, 255, 255));

#ifdef _DEBUG
	int line = 3;	//行
	int lineHeight = 30;	//行
	DebugDrawFormat::FormatStringRight(L"param", 0, line,lineHeight);
	DebugDrawFormat::FormatStringRight(L"id : %ws", StringUtility::StringToWstring(param_.id_).c_str(), line, lineHeight);
	DebugDrawFormat::FormatStringRight(L"name : %s", StringUtility::StringToWstring(param_.name_).c_str(), line, lineHeight);
	DebugDrawFormat::FormatStringRight(L"modelFile_ : %s", StringUtility::StringToWstring(param_.modelFile_).c_str(), line, lineHeight);
	DebugDrawFormat::FormatStringRight(L"placeable_ : %d", param_.placeable_, line, lineHeight);
	DebugDrawFormat::FormatStringRight(L"carryable_ : %d", param_.carryable_, line, lineHeight);
	DebugDrawFormat::FormatStringRight(L"interactable_ : %d", param_.interactable_, line, lineHeight);
	DebugDrawFormat::FormatStringRight(L"interactTime %2.f", param_.interactTime, line, lineHeight);
	DebugDrawFormat::FormatStringRight(L"type : %s", StringUtility::StringToWstring(param_.category_).c_str(), line, lineHeight);
	
	for (auto& item : param_.acceptedItems_)
	{
		DebugDrawFormat::FormatStringRight(L"acceptedItem : %s", StringUtility::StringToWstring(item).c_str(), line, lineHeight);
	}
	for (auto& produce : param_.produces_)
	{
		DebugDrawFormat::FormatStringRight(L"produce : %s", StringUtility::StringToWstring(produce).c_str(), line, lineHeight);
	}
#endif // _DEBUG

}

bool StageObject::Interact(Player& player)
{
	return false;
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
