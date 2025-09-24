#include "../../../Manager/Generic/ResourceManager.h"
#include "../../../Utility/CommonUtility.h"
#include "../../../Renderer/ModelMaterial.h"
#include "../../../Renderer/ModelRenderer.h"
#include "Furnitures.h"

Furnitures::Furnitures(void)
{
	material_ = nullptr;
	renderer_ = nullptr;
}

Furnitures::~Furnitures(void)
{
}

void Furnitures::Init(void)
{
	//床の基本設定
	SetupFurniture(floor_, ResourceManager::SRC::FLOOR, CommonUtility::VECTOR_ZERO);
	floor_.MakeCollider(Collider::TYPE::STAGE);

	//床用のマテリアル初期化
	InitMaterial();
	
	//その他家具の基本設定
	SetupFurniture(deskL_, ResourceManager::SRC::DESK, DESK_L_POS);	//左の机
	SetupFurniture(deskR_, ResourceManager::SRC::DESK, DESK_R_POS);	//右の机
	SetupFurniture(sidePlant_, ResourceManager::SRC::PLANT, SIDE_PLANT_POS);	//横の観葉植物
	SetupFurniture(frontPlant_, ResourceManager::SRC::PLANT, FRONT_PLANT_POS);	//前の観葉植物
	SetupFurniture(sideShelves_, ResourceManager::SRC::SHELVES, SIDE_SHELVES_POS);	//横の棚
	SetupFurniture(frontLShelves_, ResourceManager::SRC::SHELVES_FRONT,			//前の左の棚
		{ FRONT_SHELVES_R_POS_X,FRONT_SHELVES_POS.y,FRONT_SHELVES_POS.z });
	SetupFurniture(frontRShelves_, ResourceManager::SRC::SHELVES,		//前の右の棚
		FRONT_SHELVES_POS, Quaternion::Euler({ 0.0f, CommonUtility::Deg2RadF(90.0f), 0.0f }));
}

void Furnitures::Draw(void)
{
	//床の描画
	renderer_->Draw();
	//その他家具の描画
	MV1DrawModel(deskL_.modelId);			//左の机
	MV1DrawModel(deskR_.modelId);			//右の机
	MV1DrawModel(sidePlant_.modelId);		//横の観葉植物
	MV1DrawModel(frontPlant_.modelId);		//前の観葉植物
	MV1DrawModel(sideShelves_.modelId);		//横の棚
	MV1DrawModel(frontLShelves_.modelId);	//前の左の棚
	MV1DrawModel(frontRShelves_.modelId);	//前の右の棚
}

void Furnitures::SetupFurniture(Transform& transform,
	ResourceManager::SRC modelSrc, const VECTOR& pos, const Quaternion& quaRotLocal)
{
	//モデルの読み込みと基本設定
	transform.SetModel(ResourceManager::GetInstance().LoadModelDuplicate(modelSrc));
	transform.scl = CommonUtility::VECTOR_ONE;
	transform.pos = pos;
	transform.quaRot = Quaternion();
	transform.quaRotLocal = quaRotLocal;
	transform.Update();
}

void Furnitures::InitMaterial(void)
{
	//定数バッファ用の定数
	const int VSbufferSize = 1;	
	const int PSbufferSize = 3;	
	//モデル描画用
	material_ = std::make_unique<ModelMaterial>(
		"StdModelVS.cso", VSbufferSize,
		"StdModelPS.cso", PSbufferSize
	);
	//タイリングするのでテクスチャアドレスをWRAPに
	material_->SetTextureAddress(ModelMaterial::TEXADDRESS::WRAP);

	//uvに渡すスケール値
	material_->AddConstBufVS({ TILLING_SIZE ,TILLING_SIZE,TILLING_SIZE,TILLING_SIZE });

	//色の影響度
	const float colorPow = 1.0f;
	material_->AddConstBufPS({ colorPow,colorPow,colorPow,colorPow });

	//ライトの方向
	VECTOR light = GetLightDirection();
	material_->AddConstBufPS({ light.x,light.y,light.z,0.0f });	//4つ目はダミー

	//環境光
	material_->AddConstBufPS({ AMBIENT_COLOR,AMBIENT_COLOR,AMBIENT_COLOR,AMBIENT_COLOR });

	//レンダラーの生成
	renderer_ = std::make_unique<ModelRenderer>(floor_.modelId, *material_);
}