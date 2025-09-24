#include "IconUI.h"

IconUI::IconUI(VECTOR localPos,VECTOR parentPos,ResourceManager::SRC src) :
	localPos_(localPos), parentPos_(parentPos), imgSrc_(src)
{
}

void IconUI::Init(void)
{
	uiImg_ = ResourceManager::GetInstance().Load(
		imgSrc_).handleId_;

	//í«è]êÊÇÃç¿ïWÇ…ÉxÉNÉgÉãâ¡éZ
	pos_ = VAdd(parentPos_, localPos_);
}

void IconUI::Update(void)
{
}

void IconUI::Draw(void)
{
	if (!isActive_) return;
	const float imageCenter = 0.5f;
	const float imageAngle = 70.0f;
	//UIÇÃï`âÊ
	DrawBillboard3D(pos_,
		imageCenter, imageCenter, imageAngle,
		0.0f, uiImg_, true);
}
