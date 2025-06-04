#include "../Libs/ImGui/imgui.h"
#include "../../Common/DebugDrawFormat.h"
#include "../Common/Sphere.h"
#include "../Common/Cube.h"
#include "../../Utility/AsoUtility.h"
#include "../../Utility/StringUtility.h"
#include "StageObjectLibrary.h"
#include "StageObject.h"

StageObject::StageObject(const std::string objId, const float width,
	const float height, const float depth,Player& player):
	objId_(objId),width_(width),height_(height),
	depth_(depth),player_(player)
{
	itemState_ = ITEM_STATE::NONE;
	machineState_ = MACHINE_STATE::NONE;
	param_ = StageObjectLibrary::ObjectParams();
	rad_ = 0.0f;
	isLid_ = false;
	drink_ = Order::DRINK::NONE;
	sweets_ = Order::SWEETS::NONE;
}

StageObject::~StageObject(void)
{
}

void StageObject::Init(void)
{
	object_ = StageObjectLibrary::LoadData(objId_);

	param_ = object_.second;

	cube_ = std::make_unique<Cube>(transform_);

	sphere_ = std::make_unique<Sphere>(transform_);
	sphere_->SetLocalPos({ 0.0f, 0.0f, 0.0f });
	if(objId_ == "Table" || objId_ == "Counter")sphere_->SetLocalPos({0.0f, height_, 0.0f});

	itemState_ = ITEM_STATE::PLACED;
	machineState_ = MACHINE_STATE::INACTIVE;

	transform_.Update();

	rad_ = 30.0f;

	if (objId_ == "Coffee_Machine")rad_ = 35.0f;
	else if (objId_ == "Ice_Dispenser")rad_ = 35.0f;
	else if (objId_ == "Cup_Lid")rad_ = 5.0f;
	else if (objId_ == "Hot_Cup" || objId_ == "Ice_Cup" ||
		objId_ == "Hot_Coffee" || objId_ == "Cup_Lid_Rack")rad_ = 20.0f;

	if (objId_ == "Hot_Coffee")drink_ = Order::DRINK::HOT;
	if (objId_ == "Ice_Coffee")drink_ = Order::DRINK::ICE;

	sphere_->SetRadius(rad_);

#ifdef _DEBUG

	//�����蔻��p�̋���
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

	isActioned_ = false;

	switch (itemState_)
	{
	case StageObject::ITEM_STATE::NONE:
		break;

	case StageObject::ITEM_STATE::PLACED:
		UpdatePlaced();
		break;

	case StageObject::ITEM_STATE::HOLD:
		UpdateHold();
		break;

	default:
		break;
	}

	switch (machineState_)
	{
	case StageObject::MACHINE_STATE::NONE:
		break;

	case StageObject::MACHINE_STATE::INACTIVE:
		UpdateInActive();
		break;

	case StageObject::MACHINE_STATE::ACTIVE:
		UpdateActive();
		break;

	default:
		break;
	}

	transform_.Update();

	//UpdateDebugImGui();
}

void StageObject::Draw(void)
{
//#ifdef _DEBUG

		//�Ƃ肠�������̃��f���Ƃ��ĐF�Ⴂ��Cube�𐶐�����

	int col = 0x000000;
	COLOR_U8  retCol;

	VECTOR pos = transform_.pos;

	if (objId_ == "Coffee_Machine")col = 0x3f312b;
	else if (objId_ == "Ice_Dispenser")col = 0x4682b4, pos.y -= 10.0f;
	else if (objId_ == "Table" || objId_ == "Counter")col = 0xd2b48c;
	else if (objId_ == "Sweets_Choco")col = 0xa0522d;
	else if (objId_ == "Sweets_Strawberry")col = 0xdda0dd;
	else if (objId_ == "Hot_Cup")col = 0xcd5c5c;
	else if (objId_ == "Hot_Coffee")col = 0xffaaaa;
	else if (objId_ == "Ice_Cup")col = 0x87ceeb;
	else if (objId_ == "Cup_With_Ice")col = 0x6495ed;
	else if (objId_ == "Cup_Lid")col = 0xa9a9a9;
	else if (objId_ == "Cup_Lid_Rack")col = 0xa9a9a9;
	else if (objId_ == "Dust_Box")col = 0x2f4f4f;
	else if (objId_ == "Cup_Hot_Rack")col = 0xffaaaa;
	//int�^��COLOR_U8�֕ϊ�
	retCol.r = (col >> 16) & 0xFF;
	retCol.g = (col >> 8) & 0xFF;
	retCol.b = col & 0xFF;
	retCol.a = 255;

	if (objId_ == "Coffee_Machine")
	{
		retCol.a = 128;
		cube_->MakeBox(transform_.pos, width_, height_, depth_, retCol);
	}
	else
	{
		//Z�o�b�t�@��L���ɂ���
		SetUseZBuffer3D(true);

		//Z�o�b�t�@�ւ̏������݂�L���ɂ���
		SetWriteZBuffer3D(true);

		cube_->MakeBox(transform_.pos, width_, height_, depth_, retCol);

		//Z�o�b�t�@��L���ɂ���
		SetUseZBuffer3D(false);

		//Z�o�b�t�@�ւ̏������݂�L���ɂ���
		SetWriteZBuffer3D(false);
	}

	//DrawSphere3D(sphereTran_.pos , rad_, 8, col, col, false);
	//sphere_->Draw(col);

	int line = 3;	//�s
	int lineHeight = 30;	//�s

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
//#endif // _DEBUG

}

void StageObject::SetPos(VECTOR pos)
{
	transform_.pos = pos;
	sphereTran_.pos = pos;
}

VECTOR StageObject::GetSpherePos(void) const
{
	return sphere_->GetPos();
}

VECTOR StageObject::GetTopCenter(void) const
{
	VECTOR center = transform_.pos;
	center.y = height_;
	return center;
}

float StageObject::GetSphereRad(void) const
{
	return sphere_->GetRadius();
}

bool StageObject::IsActioned(void) const
{
	return isActioned_;
}

void StageObject::ItemCarry(void)
{
}

void StageObject::ItemPlaced(VECTOR pos)
{
}

void StageObject::Interact(const std::string& objId)
{
}

void StageObject::PickUp(std::vector<std::unique_ptr<StageObject>>& object)
{
}

void StageObject::UpdatePlaced(void)
{
}

void StageObject::UpdateHold(void)
{
}

void StageObject::UpdateInActive(void)
{
}

void StageObject::UpdateActive(void)
{
}

VECTOR StageObject::GetRotPos(const VECTOR& localPos) const
{
	VECTOR localRotPos = follow_.quaRot.PosAxis(localPos);
	return VAdd(follow_.pos, localRotPos);
}

void StageObject::UpdateDebugImGui(void)
{
	//�E�B���h�E�^�C�g��&�J�n����
	ImGui::Begin("object");

	//�ʒu
	ImGui::Text("position");
	//�\���̂̐擪�|�C���^��n���Axyz�ƘA�������������z�u�փA�N�Z�X
	ImGui::InputFloat3("Pos", &transform_.pos.x);
	ImGui::SliderFloat("PosX", &transform_.pos.x, -300.0f, 500.0f);
	ImGui::SliderFloat("PosY", &transform_.pos.y, -300.0f, 500.0f);
	ImGui::SliderFloat("PosZ", &transform_.pos.z, -300.0f, 1000.0f);	
	
	//�\���̂̐擪�|�C���^��n���Axyz�ƘA�������������z�u�փA�N�Z�X
	ImGui::InputFloat3("Scale", &width_);
	ImGui::SliderFloat("w", &width_, -300.0f, 500.0f);
	ImGui::SliderFloat("h", &height_, -300.0f, 500.0f);
	ImGui::SliderFloat("d", &depth_, -300.0f, 1000.0f);
	//�I������
	ImGui::End();
}
