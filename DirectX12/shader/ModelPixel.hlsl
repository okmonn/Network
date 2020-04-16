#include "Model.hlsli"

/*����
.�x�N�g��1
.�x�N�g��2
return ����*/
float Dot(float3 vec1, float3 vec2)
{
    return (vec1.x * vec2.x) + (vec1.y * vec2.y) + (vec1.z * vec2.z);
}

/*���˃x�N�g��
.�i�s�x�N�g��
.�Փ˕��̖@���x�N�g��
return ���˃x�N�g��*/
float3 Reflect(float3 vec, float3 normal)
{
    return vec + (2.0f * dot(-vec, normal) * normal);
}

float4 main(Output output) : SV_TARGET
{
    /*�����F*/
    float3 light_color = float3(1.0f, 1.0f, 1.0f);
    /*�����x�N�g��*/
    float3 eye_vec = normalize(output.pos.xyz - float3(0.0f, 10.0f, -15.0f));
    /*�����x�N�g��*/
    float3 light_vec = normalize(float3(-1.0f, 1.0f, -1.0f));
    
    /*���˃x�N�g��*/
    float3 ref_vec = Reflect(light_vec, output.normal.xyz);
    /*�X�y�L�����̓x����*/
    float3 spec_level = pow(saturate(Dot(ref_vec, eye_vec)), supecular.w);
    /*�����x�N�g���Ɩ@���x�N�g���Ƃ̓���*/
    float bright = saturate(Dot(light_vec, output.normal.xyz));
    /*�����o�[�g*/
    float lambert = saturate(acos(bright) / acos(-1.0f));

    /*�F*/
    float3 color = saturate((diffuse.xyz * lambert) + (supecular.xyz * spec_level) + (ambient.xyz * light_color));

    return tex.Sample(smp, output.uv) * float4(color, diffuse.w);
}