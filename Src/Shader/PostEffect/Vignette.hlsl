#include "../Common/Pixel/PixelShader2DHeader.hlsli"

// �萔�o�b�t�@�F�X���b�g4�Ԗ�(b4�Ə���)
cbuffer cbParam : register(b4)
{
	float g_vine_pow;// �r�l�b�g�̋��x
}

float4 main(PS_INPUT PSInput) : SV_TARGET
{

	float2 uv = PSInput.uv;
	float x = abs(uv.x - 0.5f);	// 0.0�`1.0f �� -0.5�`0.5 �� 0.0�`0.5
	float y = abs(uv.y - 0.5f);	// 0.0�`1.0f �� -0.5�`0.5 �� 0.0�`0.5
	float dis = (x * x + y * y);

	// �r�l�b�g�̋��x���v�Z
	float vignette = 1.0f - saturate(dis * g_vine_pow);

	// UV���W�ƃe�N�X�`�����Q�Ƃ��āA�œK�ȐF���擾����
	float4 ret = tex.Sample(texSampler, PSInput.uv);
	ret.rgb *= vignette;

	return ret;

}
