#include "IconUI.h"

IconUI::IconUI(VECTOR localPos,VECTOR parentPos,ResourceManager::SRC src) :
	localPos_(localPos), parentPos_(parentPos), imgSrc_(src), size_(0.0f)
{
}

void IconUI::Init(void)
{
	//UI画像の読み込み
	uiImg_ = ResourceManager::GetInstance().Load(
		imgSrc_).handleId_;

	//追従先の座標にベクトル加算
	pos_ = VAdd(parentPos_, localPos_);
}

void IconUI::Update(void)
{
}

void IconUI::Draw(void)
{
	//非表示なら描画しない
	if (!isActive_) return;
	//UIの画像の中央値
	const float imageCenter = 0.5f;
	//UIの角度
	const float imageAngle = 70.0f;
	//UIの描画
	DrawBillboard3D(pos_,
		imageCenter, imageCenter, imageAngle,
		0.0f, uiImg_, true);
}
