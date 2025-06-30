#include "../../Utility/AsoUtility.h"
#include "../../Manager/Generic/ResourceManager.h"
#include "GaugeUI.h"

GaugeUI::GaugeUI(bool isCircle, float activeTime):
	activeTime_(activeTime), isCircle_(isCircle), currentTime_(0.0f)
{
}

void GaugeUI::Init(void)
{
	
	uiImg_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::PUSH_SPACE).handleId_;
}

void GaugeUI::Update(void)
{
}

void GaugeUI::Draw(void)
{
	if (!isActive_) return;
}

void GaugeUI::SetPos(VECTOR pos)
{
}

void GaugeUI::SetActiveTime(float time)
{
}

void GaugeUI::SetIsCircle(bool flag)
{
}

void GaugeUI::Reset(void)
{
}