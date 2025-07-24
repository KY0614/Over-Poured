// VS/PS����
#include "../Common/VertexToPixelHeader.hlsli"

// IN
#define PS_INPUT VertexToPixelLit

//PS
#include "../Common/Pixel/PixelShader3DHeader.hlsli"

// �萔�o�b�t�@�F�X���b�g4�Ԗ�(b4�Ə���)
cbuffer cbParam : register(b4)
{
    float4 g_color; //�萔�o�b�t�@�̐F
    
    float3 g_light_dir; //���C�g�̕���
    float dummy; // float4�̃T�C�Y�ɍ��킹�邽�߂̃_�~�[�ϐ�
    
    float4 g_ambient_color; // �����̐F
}

float4 main(PS_INPUT PSInput) : SV_TARGET
{
    
    float4 color;
    color = diffuseMapTexture.Sample(diffuseMapSampler, PSInput.uv);
    if (color.a < 0.01f)
    {
        discard; // �A���t�@�l��0�̃s�N�Z���͔j��)
    }
    
    color *= g_color; // �萔�o�b�t�@�̐F����Z
    
    float3 normal = PSInput.normal;
    float lihgt = dot(normal, -g_light_dir);

    float3 lightDir = normalize(g_light_dir);
    float NdotL = dot(normalize(PSInput.normal), -lightDir);

    // ���邳��i�K�I�Ƀx�^�h��
    float brightness;
    if (NdotL > 0.75)
    {
        brightness = 1.0;
    }
    else if (NdotL > 0.7)
    {
        brightness = 0.9;
    }
    else
    {
        brightness = 0.8;
    }

    float3 baseColor = color.rgb;
    
    return float4(baseColor * brightness * lihgt, 1.0);
    
}