// VS/PS����
#include "../Common/VertexToPixelHeader.hlsli"

// IN
#define PS_INPUT VertexToPixelLit

//PS
#include "../Common/Pixel/PixelShader3DHeader.hlsli"

// �萔�o�b�t�@�F�X���b�g4�Ԗ�(b4�Ə���)
cbuffer cbParam : register(b4)
{
    float4 g_color;
    
    float3 g_light_dir;
    float dummy; // float4�̃T�C�Y�ɍ��킹�邽�߂̃_�~�[�ϐ�
    
    float4 g_ambient_color; // �����̐F
    
    float3 g_camera_pos; // �J�����̈ʒu
    float dummy2; // float4�̃T�C�Y�ɍ��킹�邽�߂̃_�~�[�ϐ�
    
    float4 g_specular_color; // �X�y�L�����̐F
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
    
    float lihgt = dot(PSInput.normal, -g_light_dir);
 
    float3 refDir = normalize(g_light_dir + (PSInput.normal * 2.0f));
    float3 refEye = g_camera_pos - PSInput.worldPos;
    float3 refEyeDir = normalize(refEye); // �J�����ʒu����̃x�N�g���𐳋K��
    float refDot = abs(dot(refDir, refEyeDir));
    float g_specular_pow = 40.0f;
    float refDotEx = pow(refDot, g_specular_pow);
    
    // �F�̍���
    float3 rgb = (color.rgb * g_color.rgb * lihgt)
    + (refDotEx * g_specular_color.rgb) + g_ambient_color.rgb;
    
    return float4(rgb, color.a);

}