#include "BlurShader.h"
#include "utility/DeviceManager.h"
#include <string>

#define SAFE_RELEASE(a) if (a != nullptr) { a->Release(); a = nullptr; }

static const char* fx = {
	"// -------------------------------------------------------------\n"
	"// �K�E�X�t�B���^\n"
	"// \n"
	"// Copyright (c) 2003 IMAGIRE Takashi. All rights reserved.\n"
	"// -------------------------------------------------------------\n"
	"\n"
	"// -------------------------------------------------------------\n"
	"// �O���[�o���ϐ�\n"
	"// -------------------------------------------------------------\n"
	"\n"
	"float MAP_WIDTH;\n"
	"float MAP_HEIGHT;\n"
	"float weight[8];\n"
	"\n"
	"// -------------------------------------------------------------\n"
	"// �e�N�X�`��\n"
	"// -------------------------------------------------------------\n"
	"texture SrcMap;\n"
	"sampler SrcSamp = sampler_state\n"
	"{\n"
	"    Texture = <SrcMap>;\n"
	"    MinFilter = LINEAR;\n"
	"    MagFilter = LINEAR;\n"
	"    MipFilter = NONE;\n"
	"\n"
	"    AddressU = Clamp;\n"
	"    AddressV = Clamp;\n"
	"};\n"
	"// -------------------------------------------------------------\n"
	"// ���_�V�F�[�_����s�N�Z���V�F�[�_�ɓn���f�[�^\n"
	"// -------------------------------------------------------------\n"
	"struct VS_OUTPUT\n"
	"{\n"
	"    float4 Pos          : POSITION;\n"
	"    float2 Tex          : TEXCOORD0;\n"
	"};\n"
	"\n"
	"// -------------------------------------------------------------\n"
	"// X�ڂ���\n"
	"// -------------------------------------------------------------\n"
	"\n"
	"// -------------------------------------------------------------\n"
	"// ���_�V�F�[�_�v���O����\n"
	"// -------------------------------------------------------------\n"
	"VS_OUTPUT VS_pass1 (\n"
	"      float4 Pos    : POSITION,          // ���f���̒��_\n"
	"      float4 Tex    : TEXCOORD0          // �e�N�X�`�����W\n"
	"){\n"
	"    VS_OUTPUT Out = (VS_OUTPUT)0;        // �o�̓f�[�^\n"
	"    \n"
	"    // �ʒu���W\n"
	"    Out.Pos = Pos;\n"
	"    \n"
	"    Out.Tex = Tex + float2(0, 0.5f / MAP_HEIGHT);\n"
	"    \n"
	"    return Out;\n"
	"}\n"
	"\n"
	"// -------------------------------------------------------------\n"
	"// �s�N�Z���V�F�[�_�v���O����\n"
	"// -------------------------------------------------------------\n"
	"float4 PS_pass1(VS_OUTPUT In) : COLOR\n"
	"{   \n"
	"    float4 Color;\n"
	"    \n"
	"    Color  = weight[0] *  tex2D(SrcSamp, In.Tex);\n"
	"    Color += weight[1]\n"
	"     * (tex2D(SrcSamp, In.Tex + float2(+ 2.0f / MAP_WIDTH, 0))\n"
	"     +  tex2D(SrcSamp, In.Tex + float2(- 2.0f / MAP_WIDTH, 0)));\n"
	"    Color += weight[2]\n"
	"     * (tex2D(SrcSamp, In.Tex + float2(+ 4.0f / MAP_WIDTH, 0))\n"
	"     +  tex2D(SrcSamp, In.Tex + float2(- 4.0f / MAP_WIDTH, 0)));\n"
	"    Color += weight[3]\n"
	"     * (tex2D(SrcSamp, In.Tex + float2(+ 6.0f / MAP_WIDTH, 0))\n"
	"     +  tex2D(SrcSamp, In.Tex + float2(- 6.0f / MAP_WIDTH, 0)));\n"
	"    Color += weight[4]\n"
	"     * (tex2D(SrcSamp, In.Tex + float2(+ 8.0f / MAP_WIDTH, 0))\n"
	"     +  tex2D(SrcSamp, In.Tex + float2(- 8.0f / MAP_WIDTH, 0)));\n"
	"    Color += weight[5]\n"
	"     * (tex2D(SrcSamp, In.Tex + float2(+10.0f / MAP_WIDTH, 0))\n"
	"     +  tex2D(SrcSamp, In.Tex + float2(-10.0f / MAP_WIDTH, 0)));\n"
	"    Color += weight[6]\n"
	"     * (tex2D(SrcSamp, In.Tex + float2(+12.0f / MAP_WIDTH, 0))\n"
	"     +  tex2D(SrcSamp, In.Tex + float2(-12.0f / MAP_WIDTH, 0)));\n"
	"    Color += weight[7]\n"
	"     * (tex2D(SrcSamp, In.Tex + float2(+14.0f / MAP_WIDTH, 0))\n"
	"     +  tex2D(SrcSamp, In.Tex + float2(-14.0f / MAP_WIDTH, 0)));\n"
	"    \n"
	"    return Color;\n"
	"}\n"
	"// -------------------------------------------------------------\n"
	"// Y�ڂ���\n"
	"// -------------------------------------------------------------\n"
	"\n"
	"// -------------------------------------------------------------\n"
	"// ���_�V�F�[�_�v���O����\n"
	"// -------------------------------------------------------------\n"
	"VS_OUTPUT VS_pass2 (\n"
	"      float4 Pos    : POSITION,          // ���f���̒��_\n"
	"      float4 Tex    : TEXCOORD0          // �e�N�X�`�����W\n"
	"){\n"
	"    VS_OUTPUT Out = (VS_OUTPUT)0;        // �o�̓f�[�^\n"
	"    \n"
	"    // �ʒu���W\n"
	"    Out.Pos = Pos;\n"
	"    \n"
	"    Out.Tex = Tex + float2(0.5f / MAP_WIDTH, 0);\n"
	"    \n"
	"    return Out;\n"
	"}\n"
	"\n"
	"// -------------------------------------------------------------\n"
	"// �s�N�Z���V�F�[�_�v���O����\n"
	"// -------------------------------------------------------------\n"
	"float4 PS_pass2(VS_OUTPUT In) : COLOR\n"
	"{   \n"
	"    float4 Color;\n"
	"    \n"
	"    Color  = weight[0] *  tex2D(SrcSamp, In.Tex);\n"
	"    Color += weight[1]\n"
	"     * (tex2D(SrcSamp, In.Tex + float2(0, + 2.0f / MAP_HEIGHT))\n"
	"     +  tex2D(SrcSamp, In.Tex + float2(0, - 2.0f / MAP_HEIGHT)));\n"
	"    Color += weight[2]\n"
	"     * (tex2D(SrcSamp, In.Tex + float2(0, + 4.0f / MAP_HEIGHT))\n"
	"     +  tex2D(SrcSamp, In.Tex + float2(0, - 4.0f / MAP_HEIGHT)));\n"
	"    Color += weight[3]\n"
	"     * (tex2D(SrcSamp, In.Tex + float2(0, + 6.0f / MAP_HEIGHT))\n"
	"     +  tex2D(SrcSamp, In.Tex + float2(0, - 6.0f / MAP_HEIGHT)));\n"
	"    Color += weight[4]\n"
	"     * (tex2D(SrcSamp, In.Tex + float2(0, + 8.0f / MAP_HEIGHT))\n"
	"     +  tex2D(SrcSamp, In.Tex + float2(0, - 8.0f / MAP_HEIGHT)));\n"
	"    Color += weight[5]\n"
	"     * (tex2D(SrcSamp, In.Tex + float2(0, +10.0f / MAP_HEIGHT))\n"
	"     +  tex2D(SrcSamp, In.Tex + float2(0, -10.0f / MAP_HEIGHT)));\n"
	"    Color += weight[6]\n"
	"     * (tex2D(SrcSamp, In.Tex + float2(0, +12.0f / MAP_HEIGHT))\n"
	"     +  tex2D(SrcSamp, In.Tex + float2(0, -12.0f / MAP_HEIGHT)));\n"
	"    Color += weight[7]\n"
	"     * (tex2D(SrcSamp, In.Tex + float2(0, +14.0f / MAP_HEIGHT))\n"
	"     +  tex2D(SrcSamp, In.Tex + float2(0, -14.0f / MAP_HEIGHT)));\n"
	"    \n"
	"    return Color;\n"
	"}\n"
	"\n"
	"// -------------------------------------------------------------\n"
	"// X��Y���ڂ���\n"
	"// -------------------------------------------------------------\n"
	"\n"
	"// -------------------------------------------------------------\n"
	"// ���_�V�F�[�_�v���O����\n"
	"// -------------------------------------------------------------\n"
	"VS_OUTPUT VS_pass3 (\n"
	"      float4 Pos    : POSITION,          // ���f���̒��_\n"
	"      float4 Tex    : TEXCOORD0          // �e�N�X�`�����W\n"
	"){\n"
	"    VS_OUTPUT Out = (VS_OUTPUT)0;        // �o�̓f�[�^\n"
	"    \n"
	"    // �ʒu���W\n"
	"    Out.Pos = Pos;\n"
	"    \n"
	"    Out.Tex = Tex + float2(0.5f / MAP_WIDTH, 0.5f / MAP_HEIGHT);\n"
	"    \n"
	"    return Out;\n"
	"}\n"
	"\n"
	"// -------------------------------------------------------------\n"
	"// �s�N�Z���V�F�[�_�v���O����\n"
	"// -------------------------------------------------------------\n"
	"float4 PS_pass3(VS_OUTPUT In) : COLOR\n"
	"{   \n"
	"    float4 Color;\n"
	"    \n"
	"    Color  = weight[0] *  tex2D(SrcSamp, In.Tex);\n"
	"    Color += weight[1]\n"
	"     * (tex2D(SrcSamp, In.Tex + float2(+ 2.0f / MAP_WIDTH, + 2.0f / MAP_HEIGHT))\n"
	"     +  tex2D(SrcSamp, In.Tex + float2(- 2.0f / MAP_WIDTH, - 2.0f / MAP_HEIGHT)));\n"
	"    Color += weight[2]                 \n"
	"     * (tex2D(SrcSamp, In.Tex + float2(+ 4.0f / MAP_WIDTH, + 4.0f / MAP_HEIGHT))\n"
	"     +  tex2D(SrcSamp, In.Tex + float2(- 4.0f / MAP_WIDTH, - 4.0f / MAP_HEIGHT)));\n"
	"    Color += weight[3]                 \n"
	"     * (tex2D(SrcSamp, In.Tex + float2(+ 6.0f / MAP_WIDTH, + 6.0f / MAP_HEIGHT))\n"
	"     +  tex2D(SrcSamp, In.Tex + float2(- 6.0f / MAP_WIDTH, - 6.0f / MAP_HEIGHT)));\n"
	"    Color += weight[4]                 \n"
	"     * (tex2D(SrcSamp, In.Tex + float2(+ 8.0f / MAP_WIDTH, + 8.0f / MAP_HEIGHT))\n"
	"     +  tex2D(SrcSamp, In.Tex + float2(- 8.0f / MAP_WIDTH, - 8.0f / MAP_HEIGHT)));\n"
	"    Color += weight[5]                 \n"
	"     * (tex2D(SrcSamp, In.Tex + float2(+10.0f / MAP_WIDTH, +10.0f / MAP_HEIGHT))\n"
	"     +  tex2D(SrcSamp, In.Tex + float2(-10.0f / MAP_WIDTH, -10.0f / MAP_HEIGHT)));\n"
	"    Color += weight[6]                 \n"
	"     * (tex2D(SrcSamp, In.Tex + float2(+12.0f / MAP_WIDTH, +12.0f / MAP_HEIGHT))\n"
	"     +  tex2D(SrcSamp, In.Tex + float2(-12.0f / MAP_WIDTH, -12.0f / MAP_HEIGHT)));\n"
	"    Color += weight[7]                 \n"
	"     * (tex2D(SrcSamp, In.Tex + float2(+14.0f / MAP_WIDTH, +14.0f / MAP_HEIGHT))\n"
	"     +  tex2D(SrcSamp, In.Tex + float2(-14.0f / MAP_WIDTH, -14.0f / MAP_HEIGHT)));\n"
	"    \n"
	"    return Color;\n"
	"}\n"
	"\n"
	"// -------------------------------------------------------------\n"
	"// �e�N�j�b�N\n"
	"// -------------------------------------------------------------\n"
	"technique TShader\n"
	"{\n"
	"    pass P0\n"
	"    {\n"
	"        // �V�F�[�_\n"
	"        VertexShader = compile vs_1_1 VS_pass1();\n"
	"        PixelShader  = compile ps_2_0 PS_pass1();\n"
	"        \n"
	"        Sampler[0] = (SrcSamp);\n"
	"    }\n"
	"    pass P1\n"
	"    {\n"
	"        // �V�F�[�_\n"
	"        VertexShader = compile vs_1_1 VS_pass2();\n"
	"        PixelShader  = compile ps_2_0 PS_pass2();\n"
	"        \n"
	"        Sampler[0] = (SrcSamp);\n"
	"    }\n"
	"    pass P2\n"
	"    {\n"
	"        // �V�F�[�_\n"
	"        VertexShader = compile vs_1_1 VS_pass3();\n"
	"        PixelShader  = compile ps_2_0 PS_pass3();\n"
	"        \n"
	"        Sampler[0] = (SrcSamp);\n"
	"    }\n"
	"}\n"
};

BlurShader::BlurShader()
	: dispersion_(2.0f)
{
	auto device = DeviceManager::GetInstance()->GetDevice()->GetDevice();
	if (device == nullptr) return ;

	//�t�@�C���̓ǂݍ���
	LPD3DXBUFFER message;
	HRESULT hr = D3DXCreateEffect(device, fx, strlen(fx), nullptr, nullptr, 0, nullptr, &effect_, &message);
	if (FAILED(hr)) {
		std::string s = static_cast<const char*>(message->GetBufferPointer());
		SAFE_RELEASE(message);
		return;
	}
	SAFE_RELEASE(message);
	techniqe_ = effect_->GetTechniqueByName("TShader");
	weight_ = effect_->GetParameterByName(nullptr, "weight");
	effect_->SetFloat("MAP_WIDTH", static_cast<float>(DeviceManager::GetInstance()->GetWidth()));
	effect_->SetFloat("MAP_HEIGHT", static_cast<float>(DeviceManager::GetInstance()->GetHeight()));
	UpdateWeight_(dispersion_ * dispersion_);
}

BlurShader::~BlurShader()
{
	Release();
}

void BlurShader::SetDispersion(float dispersion)
{
	dispersion_ = dispersion;
	UpdateWeight_(dispersion_ * dispersion_);
}

float BlurShader::GetDispersion() const
{
	return dispersion_;
}

void BlurShader::UpdateWeight_(float dispersion)
{
	float total = 0;
	for (int i = 0; i < WEIGHT_MUN; ++i) {
		weight_table_[i] = expf(-0.5f * static_cast<float>(i * i) / dispersion);
		if (i == 0) {
			total += weight_table_[i];
		} else {
			// ���S�ȊO�́A�Q�񓯂��W�����g���̂łQ�{
			total += weight_table_[i] * 2.0f;
		}
	}

	// �K�i��
	for (int i = 0; i < WEIGHT_MUN; ++i) {
		weight_table_[i] /= total;
	}
	effect_->SetFloatArray(weight_, weight_table_, WEIGHT_MUN);
}

