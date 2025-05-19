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
}

void StageObject::Update(void)
{
}

void StageObject::Draw(void)
{
	DrawSphere3D(transform_.pos, 30.0f, 8, 0xffffff, 0xffffff, true);
	VECTOR screenPos = ConvWorldPosToScreenPos(transform_.pos);
	// ïœä∑ê¨å˜
	DrawString(static_cast<int>(screenPos.x), static_cast<int>(screenPos.y),
		StringUtility::StringToWstring(objId_.c_str()).c_str(), GetColor(255, 255, 255));

#ifdef _DEBUG
	int line = 0;	//çs
	int lineHeight = 30;	//çs
	DebugDrawFormat::FormatStringRight(L"param", 0, line,lineHeight);
	DebugDrawFormat::FormatStringRight(L"id : %s", param_.id_, line, lineHeight);
	DebugDrawFormat::FormatStringRight(L"name : %s", param_.name_, line, lineHeight);
	DebugDrawFormat::FormatStringRight(L"modelFile_ : %s", param_.modelFile_, line, lineHeight);
	DebugDrawFormat::FormatStringRight(L"placeable_ : %d", param_.placeable_, line, lineHeight);
	DebugDrawFormat::FormatStringRight(L"carryable_ : %d", param_.carryable_, line, lineHeight);
	DebugDrawFormat::FormatStringRight(L"interactable_ : %d", param_.interactable_, line, lineHeight);
	DebugDrawFormat::FormatStringRight(L"interactTime %2.f", param_.interactTime, line, lineHeight);
	DebugDrawFormat::FormatStringRight(L"type : %s", param_.category_, line, lineHeight);

#endif // _DEBUG

}

bool StageObject::Interact(Player& player)
{
	return false;
}