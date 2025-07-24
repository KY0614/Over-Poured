// VS/PS����
#include "../Common/VertexToPixelHeader.hlsli"

// IN
#define PS_INPUT VertexToPixelLit

//PS
#include "../Common/Pixel/PixelShader3DHeader.hlsli"

// �萔�o�b�t�@�F�X���b�g4�Ԗ�(b4�Ə���)
cbuffer cbParam : register(b4)
{
    float4 g_outline_color; //�萔�o�b�t�@�̐F
}

float4 main(PS_INPUT PSInput) : SV_TARGET
{
    return float4(g_outline_color); // ���A�E�g���C��
}