#include "../../Manager/Generic/SceneManager.h"
#include "../../Manager/Generic/ResourceManager.h"
#include "GaugeUI.h"

namespace
{
	//�f�B�t���[�Y�J���[�̃f�t�H���g�l
	const COLOR_U8 DIF_COLOR = GetColorU8(255, 255, 255, 255);
	//�X�y�L�����J���[�̃f�t�H���g�l
	const COLOR_U8 SPEC_COLOR = GetColorU8(0, 0, 0, 0);
	//Z��������
    const VECTOR NORMAL_NEGATIVE_Z = { 0.0f, 0.0f, -1.0f };
	//Z��������
    const VECTOR NORMAL_POSITIVE_Z = { 0.0f, 0.0f, 1.0f };
    //�l�p�`�|���S���̒��_��
	const int VERTEX_NUM = 6;               
}

GaugeUI::GaugeUI(bool isCircle, float activeTime):
	activeTime_(activeTime), isCircle_(isCircle), currentTime_(0.0f)
{
	shadowImg_ = -1;
	circleImg_ = -1;
	cShadowImg_ = -1;
	isActive_ = false;
    alpha_ = 1.0f;

    width_ = 0.0f;
    height_ = 0.0f;
} 

void GaugeUI::Init(void)
{
    //�摜�̓ǂݍ���
	LoadImages(); 
}

void GaugeUI::Update(void)
{
	//��A�N�e�B�u�Ȃ�X�V���Ȃ�
	if (!isActive_)return;
	//�o�ߎ��Ԃ����Z
	currentTime_ += SceneManager::GetInstance().GetDeltaTime();
	//�o�ߎ��Ԃ��A�N�e�B�u���Ԃ𒴂�����A�N�e�B�u���ԂɌŒ�
	if (currentTime_ >= activeTime_) 
    {
		currentTime_ = activeTime_;
	}
}

void GaugeUI::Draw(void)
{
	//��A�N�e�B�u�Ȃ�`�悵�Ȃ�
	if (!isActive_) return;
	//�i�����v�Z�i0.0�`1.0�j
    float progress = currentTime_ / activeTime_;
	//�~�`�Q�[�W�������`�Q�[�W���ŕ`��𕪊�
    if (isCircle_) 
    {
		DrawCircleGauge(progress);  //�~�`�Q�[�W�̕`��
    }
    else 
    {
		DrawRectGauge(progress);    //�����`�Q�[�W�̕`��
    }
}

void GaugeUI::Reset(void)
{
    currentTime_ = 0.0f;      //�Q�[�W�̐i�������Z�b�g�i�[���j
    isActive_ = false;        //��\���E��A�N�e�B�u��
}

void GaugeUI::DrawCircleGauge(const float progress)
{
	const float radius = 50.0f; //�~�̔��a
    const int segmentCount = 80;//��̕�����
	const float uvCenter = 0.5f; //UV�̒��S�_
	const float angleMax = -DX_TWO_PI * progress;   //��̍ő�p�x�i�����v���j
    float baseAngle = DX_PI_F / 2.0f; //+90�x�̃I�t�Z�b�g�i�ォ��X�^�[�g�j
    std::vector<VERTEX3D> verts = {};

    // ���S�_
    VERTEX3D center;
    center.pos = pos_;
    center.dif = DIF_COLOR;
    center.norm = NORMAL_NEGATIVE_Z;
    center.u = uvCenter;
    center.v = uvCenter;
    verts.push_back(center);
	//�~�ʂ��\�����钸�_���v�Z���Ēǉ�
    for (int i = 0; i <= segmentCount * progress; ++i) 
    {
		//�J�n�p�x����ő�p�x�܂ŕ������ɉ����Ċp�x���v�Z
        float angle = baseAngle + angleMax * (float)i / segmentCount;
		float x = cosf(angle) * radius; //X���W
		float y = sinf(angle) * radius; //Y���W

        VERTEX3D v;
        v.pos = VGet(pos_.x + x, pos_.y + y, pos_.z);   //���S���W��
		v.dif = DIF_COLOR;                  //�f�B�t���[�Y�J���[
		v.norm = NORMAL_POSITIVE_Z;         //�@��
		v.spc = SPEC_COLOR;     	        //�X�y�L�����J���[
		v.u = uvCenter + x / (2 * radius);  //�~�̒��S���UV���v�Z
        v.v = uvCenter - y / (2 * radius);  //�~�̒��S���UV���v�Z
        verts.push_back(v);
    }
	const float polygonNum = 2; //�O�p�`�|���S����
    //��^��`��
    for (int i = 1; i < (int)verts.size() - 1; ++i) 
    {
		//�O�p�`�|���S�����\�����钸�_
        VERTEX3D tri[3] = { verts[i], verts[i + 1], verts[0] };
		DrawPolygon3D(tri, polygonNum, cShadowImg_, true);  //�e
		DrawPolygon3D(tri, polygonNum, circleImg_, true);   //�{��
    }
}

void GaugeUI::DrawRectGauge(const float progress)
{
	//�Q�[�W�̕��E����
	const float width = width_;
    const float height = height_;
	//�`�悷�镝
    float drawWidth = width * progress;

	//�l�p�`�|���S�����쐬���ĕ`�悷�郉���_��
    auto MakeQuad = [&](float drawWidth, int texHandle) {
        VERTEX3D verts[VERTEX_NUM] = {};//0���ߏ�����

        // �Q�[�W�̍��������̌X���ʁiZ�����ɉ����j�b�g�߂Â��邩�j
        const float zTilt = -30.0f;
		//���_�f�[�^�̐ݒ�
		VERTEX3D v0 = { //����
            VGet(-width / 2, height / 2, 0),
            NORMAL_POSITIVE_Z,
            DIF_COLOR,
            SPEC_COLOR,
            0.0f, 0.0f };

		VERTEX3D v1 = { //�E��
            VGet(-width / 2 + drawWidth, height / 2, 0),
            NORMAL_POSITIVE_Z,
            DIF_COLOR,
            SPEC_COLOR,
            drawWidth / width, 0.0f };

		VERTEX3D v2 = { //����
            VGet(-width / 2, -height / 2, zTilt),
            NORMAL_POSITIVE_Z,
            DIF_COLOR,
            SPEC_COLOR,
            0.0f, 1.0f };

		VERTEX3D v3 = { //�E��
            VGet(-width / 2 + drawWidth, -height / 2, zTilt),
            NORMAL_POSITIVE_Z,
            DIF_COLOR,
            SPEC_COLOR,
            drawWidth / width, 1.0f };

        verts[0] = v0;
        verts[1] = v1;
        verts[2] = v2;
        verts[3] = v2;
        verts[4] = v1;
        verts[5] = v3;

		//���_��
        const int vertexNum = 6;

		//�I�t�Z�b�g��������
        for (int i = 0; i < vertexNum; ++i)
        {
            verts[i].pos = VAdd(verts[i].pos, pos_);
        }
		//�|���S���`��
		SetLightDirection(NORMAL_NEGATIVE_Z); //���C�g�̌�����Z���������ɐݒ�
        DrawPolygon3D(verts, 2, texHandle, true);
		SetLightDirection(SceneManager::LIGHT_DIR); //���C�g�̌��������ɖ߂�
        };
	//�l�p�`�|���S���̍쐬�ƕ`��
	MakeQuad(width, shadowImg_);    //�e
	MakeQuad(drawWidth, uiImg_);    //�Q�[�W�{��
}

void GaugeUI::LoadImages(void)
{
	//�摜�̓ǂݍ���
	//�����`�̃o�[�Q�[�W�p
    uiImg_ = ResourceManager::GetInstance().Load(
        ResourceManager::SRC::UI_BAR).handleId_;
	//�����`�̃o�[�Q�[�W�̉e�p
    shadowImg_ = ResourceManager::GetInstance().Load(
        ResourceManager::SRC::UI_BARSHADOW).handleId_;

	//�����`�Q�[�W�̃f�t�H���g�T�C�Y��ݒ�
    width_ = UI_DEFAULT_WIDTH;    //�����`�Q�[�W�̕�
    height_ = UI_DEFAULT_HEIGHT;  //�����`�Q�[�W�̍���

	//�~�Q�[�W�p
    if (!isCircle_) return;
	//�~�`�Q�[�W�p�̉摜
    circleImg_ = ResourceManager::GetInstance().Load(
        ResourceManager::SRC::UI_CIRCLE).handleId_;
	//�~�`�Q�[�W�̉e�p�摜
    cShadowImg_ = ResourceManager::GetInstance().Load(
        ResourceManager::SRC::UI_CIRCLESHADOW).handleId_;
}