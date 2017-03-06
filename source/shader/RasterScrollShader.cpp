#include "RasterScrollShader.h"
#include "utility/DeviceManager.h"
#include <string>

#define SAFE_RELEASE(a) if (a != nullptr) { a->Release(); a = nullptr; }

static const char* fx = {
	"// -------------------------------------------------------------\n"
	"// �e�N�X�`��\n"
	"// -------------------------------------------------------------\n"
	"texture SrcMap;\n"
	"float Offset = 0.0f;\n"
	"float Period = 0.0f;\n"
	"float Amplitude = 0.24f;\n"
	"float4 Diffuse = float4(1.0f, 1.0f, 1.0f, 1.0f);\n"
	"\n"
	"sampler SrcSamp = sampler_state\n"
	"{\n"
	"    Texture = <SrcMap>;\n"
	"    MinFilter = LINEAR;\n"
	"    MagFilter = LINEAR;\n"
	"    MipFilter = NONE;\n"
	"\n"
	"    AddressU = BORDER;\n"
	"    AddressV = BORDER;\n"
	"};\n"
	"\n"
	"// -------------------------------------------------------------\n"
	"// �s�N�Z���V�F�[�_�v���O����\n"
	"// -------------------------------------------------------------\n"
	"float4 PS_pass1(float2 uv : TEXCOORD0) : COLOR\n"
	"{\n"
	"    float2 dst;\n"
	"    dst.x = uv.x + sin((uv.y + Offset) * Period) * Amplitude;\n"
	"    dst.y = uv.y;\n"
	"\n"
	"    float4 Color = tex2D(SrcSamp, dst) * Diffuse;\n"
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
	"        PixelShader  = compile ps_2_0 PS_pass1();\n"
	"        \n"
	"        Sampler[0] = (SrcSamp);\n"
	"    }\n"
	"}\n"
};

RasterScrollShader::RasterScrollShader()
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
	src_map_ = effect_->GetParameterByName(nullptr, "SrcMap");
	offset_handle_ = effect_->GetParameterByName(nullptr, "Offset");
	period_handle_ = effect_->GetParameterByName(nullptr, "Period");
	amplitude_handle_ = effect_->GetParameterByName(nullptr, "Amplitude");
	diffuse_handle_ = effect_->GetParameterByName(nullptr, "Diffuse");

	effect_->GetFloat(offset_handle_, &offset_);
	effect_->GetFloat(period_handle_, &period_);
	effect_->GetFloat(amplitude_handle_, &amplitude_);
}

RasterScrollShader::~RasterScrollShader()
{
	Release();
}

void RasterScrollShader::SetOffset(float value)
{
	offset_ = value;
	effect_->SetFloat(offset_handle_, value);
}

void RasterScrollShader::SetPeriod(float value)
{
	period_ = value;
	effect_->SetFloat(period_handle_, value);
}

void RasterScrollShader::SetAmplitude(float value)
{
	amplitude_ = value;
	effect_->SetFloat(amplitude_handle_, value);
}

void RasterScrollShader::SetColor(const Color& color)
{
	const float col[4] = {
		color.GetR() / 255.0f,
		color.GetG() / 255.0f,
		color.GetB() / 255.0f,
		color.GetA() / 255.0f,
	};
	effect_->SetFloatArray(diffuse_handle_, col, 4);
}
