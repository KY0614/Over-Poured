#include "../../Manager/Generic/SceneManager.h"
#include "../../Manager/Generic/ResourceManager.h"
#include "GaugeUI.h"

namespace
{
	//ディフューズカラーのデフォルト値
	const COLOR_U8 DIF_COLOR = GetColorU8(255, 255, 255, 255);
	//スペキュラカラーのデフォルト値
	const COLOR_U8 SPEC_COLOR = GetColorU8(0, 0, 0, 0);
	//Z軸負方向
    const VECTOR NORMAL_NEGATIVE_Z = { 0.0f, 0.0f, -1.0f };
	//Z軸正方向
    const VECTOR NORMAL_POSITIVE_Z = { 0.0f, 0.0f, 1.0f };
    //四角形ポリゴンの頂点数
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
    //画像の読み込み
	LoadImages(); 
}

void GaugeUI::Update(void)
{
	//非アクティブなら更新しない
	if (!isActive_)return;
	//経過時間を加算
	currentTime_ += SceneManager::GetInstance().GetDeltaTime();
	//経過時間がアクティブ時間を超えたらアクティブ時間に固定
	if (currentTime_ >= activeTime_) 
    {
		currentTime_ = activeTime_;
	}
}

void GaugeUI::Draw(void)
{
	//非アクティブなら描画しない
	if (!isActive_) return;
	//進捗を計算（0.0～1.0）
    float progress = currentTime_ / activeTime_;
	//円形ゲージか長方形ゲージかで描画を分岐
    if (isCircle_) 
    {
		DrawCircleGauge(progress);  //円形ゲージの描画
    }
    else 
    {
		DrawRectGauge(progress);    //長方形ゲージの描画
    }
}

void GaugeUI::Reset(void)
{
    currentTime_ = 0.0f;      //ゲージの進捗をリセット（ゼロ）
    isActive_ = false;        //非表示・非アクティブに
}

void GaugeUI::DrawCircleGauge(const float progress)
{
	const float radius = 50.0f; //円の半径
    const int segmentCount = 80;//扇の分割数
	const float uvCenter = 0.5f; //UVの中心点
	const float angleMax = -DX_TWO_PI * progress;   //扇の最大角度（反時計回り）
    float baseAngle = DX_PI_F / 2.0f; //+90度のオフセット（上からスタート）
    std::vector<VERTEX3D> verts = {};

    // 中心点
    VERTEX3D center;
    center.pos = pos_;
    center.dif = DIF_COLOR;
    center.norm = NORMAL_NEGATIVE_Z;
    center.u = uvCenter;
    center.v = uvCenter;
    verts.push_back(center);
	//円弧を構成する頂点を計算して追加
    for (int i = 0; i <= segmentCount * progress; ++i) 
    {
		//開始角度から最大角度まで分割数に応じて角度を計算
        float angle = baseAngle + angleMax * (float)i / segmentCount;
		float x = cosf(angle) * radius; //X座標
		float y = sinf(angle) * radius; //Y座標

        VERTEX3D v;
        v.pos = VGet(pos_.x + x, pos_.y + y, pos_.z);   //中心座標に
		v.dif = DIF_COLOR;                  //ディフューズカラー
		v.norm = NORMAL_POSITIVE_Z;         //法線
		v.spc = SPEC_COLOR;     	        //スペキュラカラー
		v.u = uvCenter + x / (2 * radius);  //円の中心基準でUVを計算
        v.v = uvCenter - y / (2 * radius);  //円の中心基準でUVを計算
        verts.push_back(v);
    }
	const float polygonNum = 2; //三角形ポリゴン数
    //扇型を描画
    for (int i = 1; i < (int)verts.size() - 1; ++i) 
    {
		//三角形ポリゴンを構成する頂点
        VERTEX3D tri[3] = { verts[i], verts[i + 1], verts[0] };
		DrawPolygon3D(tri, polygonNum, cShadowImg_, true);  //影
		DrawPolygon3D(tri, polygonNum, circleImg_, true);   //本体
    }
}

void GaugeUI::DrawRectGauge(const float progress)
{
	//ゲージの幅・高さ
	const float width = width_;
    const float height = height_;
	//描画する幅
    float drawWidth = width * progress;

	//四角形ポリゴンを作成して描画するラムダ式
    auto MakeQuad = [&](float drawWidth, int texHandle) {
        VERTEX3D verts[VERTEX_NUM] = {};//0埋め初期化

        // ゲージの高さ方向の傾き量（Z方向に何ユニット近づけるか）
        const float zTilt = -30.0f;
		//頂点データの設定
		VERTEX3D v0 = { //左上
            VGet(-width / 2, height / 2, 0),
            NORMAL_POSITIVE_Z,
            DIF_COLOR,
            SPEC_COLOR,
            0.0f, 0.0f };

		VERTEX3D v1 = { //右上
            VGet(-width / 2 + drawWidth, height / 2, 0),
            NORMAL_POSITIVE_Z,
            DIF_COLOR,
            SPEC_COLOR,
            drawWidth / width, 0.0f };

		VERTEX3D v2 = { //左下
            VGet(-width / 2, -height / 2, zTilt),
            NORMAL_POSITIVE_Z,
            DIF_COLOR,
            SPEC_COLOR,
            0.0f, 1.0f };

		VERTEX3D v3 = { //右下
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

		//頂点数
        const int vertexNum = 6;

		//オフセットを加える
        for (int i = 0; i < vertexNum; ++i)
        {
            verts[i].pos = VAdd(verts[i].pos, pos_);
        }
		//ポリゴン描画
		SetLightDirection(NORMAL_NEGATIVE_Z); //ライトの向きをZ軸負方向に設定
        DrawPolygon3D(verts, 2, texHandle, true);
		SetLightDirection(SceneManager::LIGHT_DIR); //ライトの向きを元に戻す
        };
	//四角形ポリゴンの作成と描画
	MakeQuad(width, shadowImg_);    //影
	MakeQuad(drawWidth, uiImg_);    //ゲージ本体
}

void GaugeUI::LoadImages(void)
{
	//画像の読み込み
	//長方形のバーゲージ用
    uiImg_ = ResourceManager::GetInstance().Load(
        ResourceManager::SRC::UI_BAR).handleId_;
	//長方形のバーゲージの影用
    shadowImg_ = ResourceManager::GetInstance().Load(
        ResourceManager::SRC::UI_BARSHADOW).handleId_;

	//長方形ゲージのデフォルトサイズを設定
    width_ = UI_DEFAULT_WIDTH;    //長方形ゲージの幅
    height_ = UI_DEFAULT_HEIGHT;  //長方形ゲージの高さ

	//円ゲージ用
    if (!isCircle_) return;
	//円形ゲージ用の画像
    circleImg_ = ResourceManager::GetInstance().Load(
        ResourceManager::SRC::UI_CIRCLE).handleId_;
	//円形ゲージの影用画像
    cShadowImg_ = ResourceManager::GetInstance().Load(
        ResourceManager::SRC::UI_CIRCLESHADOW).handleId_;
}