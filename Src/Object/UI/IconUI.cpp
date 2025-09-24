#include "IconUI.h"

IconUI::IconUI(VECTOR localPos,VECTOR parentPos,ResourceManager::SRC src) :
	localPos_(localPos), parentPos_(parentPos), imgSrc_(src), size_(0.0f)
{
}

void IconUI::Init(void)
{
	//UI�摜�̓ǂݍ���
	uiImg_ = ResourceManager::GetInstance().Load(
		imgSrc_).handleId_;

	//�Ǐ]��̍��W�Ƀx�N�g�����Z
	pos_ = VAdd(parentPos_, localPos_);
}

void IconUI::Update(void)
{
}

void IconUI::Draw(void)
{
	//��\���Ȃ�`�悵�Ȃ�
	if (!isActive_) return;
	//UI�̉摜�̒����l
	const float imageCenter = 0.5f;
	//UI�̊p�x
	const float imageAngle = 70.0f;
	//UI�̕`��
	DrawBillboard3D(pos_,
		imageCenter, imageCenter, imageAngle,
		0.0f, uiImg_, true);
}
