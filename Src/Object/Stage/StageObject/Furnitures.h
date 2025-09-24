#pragma once
#include <memory>
#include "../../ActorBase.h"

class ModelRenderer;
class ModelMaterial;

class Furnitures : public ActorBase
{
public:
	//座標関連
	static constexpr VECTOR SIDE_PLANT_POS = { -255.0f,0.0f,-280.0f };
	static constexpr VECTOR FRONT_PLANT_POS = { 170.0f,0.0f,465.0f };
	static constexpr VECTOR DESK_R_POS = { -190.0f,0.0f,635.0f };
	static constexpr VECTOR DESK_L_POS = { 405.0f,0.0f,635.0f };
	static constexpr VECTOR SIDE_SHELVES_POS = { -380.0f,0.0f,-35.0f };
	static constexpr VECTOR FRONT_SHELVES_POS = { -25.0f,0.0f,465.0f };
	static constexpr float FRONT_SHELVES_R_POS_X = 350.0f;

	//UVスケール
	static constexpr float TILLING_SIZE = 35.0f;

	//環境光
	static constexpr float AMBIENT_COLOR = 0.2f;

	//コンストラクタ
	Furnitures(void);

	//デストラクタ
	~Furnitures(void);

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init(void) override;

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name=""></param>
	void Draw(void) override;

	/// <summary>
	/// 床のモデル情報を取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>床のTransform情報</returns>
	const Transform GetFloorTran(void)const { return floor_; }

private:
	//マテリアル
	std::unique_ptr<ModelMaterial> material_;
	//レンダラー
	std::unique_ptr<ModelRenderer> renderer_;

	//モデル関連
	Transform floor_;	//床
	Transform deskL_;	//机左
	Transform deskR_;	//机右
	Transform sidePlant_;	//横観葉植物
	Transform frontPlant_;	//前観葉植物
	Transform sideShelves_;	//横本棚
	Transform frontLShelves_;	//前本棚左
	Transform frontRShelves_;	//前本棚右

	/// <summary>
	/// モデルセットアップ
	/// </summary>
	/// <param name="transform">モデルのTransform</param>
	/// <param name="modelSrc">モデルのソースタイプ</param>
	/// <param name="pos">初期座標</param>
	/// <param name="quaRotLocal">回転</param>
	void SetupFurniture(Transform& transform,
		ResourceManager::SRC modelSrc, const VECTOR& pos,
		const Quaternion& quaRotLocal = Quaternion());

	/// <summary>
	/// マテリアル初期化
	/// </summary>
	void InitMaterial(void);
};

