#include "Model.h"
#include "render/Renderer.h"
#include "utility/DeviceManager.h"
#include "utility/PathUtility.hpp"
#include "utility/KeyUtility.hpp"

#define SAFE_RELEASE(a) if (a != nullptr) { a->Release(); a = nullptr; }
static const float PI2 = 6.28318530718f;

Model::Model()
	: mesh_(nullptr)
	, color_(Color::White)
	, position_(0.0f, 0.0f, 0.0f)
	, rotate_(0.0f, 0.0f, 0.0f)
	, scale_(1.0f, 1.0f, 1.0f)
{
}

Model::~Model()
{
	Release();
}

bool Model::LoadFile(const char* path, std::function<std::shared_ptr<Texture>(const char* name)> texture_func)
{
	auto device = DeviceManager::GetInstance()->GetDevice()->GetDevice();
	if (device == nullptr || path == nullptr) return false;

	Release();

	//X�t�@�C�����[�h
	DWORD material_num;
	LPD3DXBUFFER buffer;
	auto ret = D3DXLoadMeshFromXA(path, D3DXMESH_MANAGED, device, nullptr, &buffer, nullptr, &material_num, &mesh_);
	if (ret != D3D_OK) {
		return false;
	}

	//X�t�@�C���ɖ@�����Ȃ��ꍇ�́A�@������������
	if ((mesh_->GetFVF() & D3DFVF_NORMAL) == 0) {
		ID3DXMesh* mesh = NULL;
		mesh_->CloneMeshFVF(mesh_->GetOptions(), mesh_->GetFVF() | D3DFVF_NORMAL, device, &mesh );
		D3DXComputeNormals(mesh, NULL);
		mesh_->Release();
		mesh_ = mesh;
	}
	
	//�}�e���A���A�e�N�X�`���̏���
	materials_.resize(material_num);
	textures_.resize(material_num);
	diffuses_.resize(material_num);
	D3DXMATERIAL* d3dxmatrs = static_cast<D3DXMATERIAL*>(buffer->GetBufferPointer());

	for (DWORD i = 0; i < material_num; ++i) {
		//�}�e���A������
		materials_[i] = d3dxmatrs[i].MatD3D;
		diffuses_[i] = materials_[i].Diffuse;

		//�e�N�X�`�������[�h
		textures_[i].reset(new Texture());
		if (d3dxmatrs[i].pTextureFilename == nullptr)
			continue;

		auto name = PathUtility::GetFileName(d3dxmatrs[i].pTextureFilename);
		if (texture_func) {
			textures_[i] = texture_func(name.c_str());
		} else {
			auto dir = PathUtility::GetDirectoryName(path);
			auto tex_path = PathUtility::Combine(dir, name);

			textures_[i].reset(new Texture());
			textures_[i]->CreateFromFile(tex_path.c_str());
		}
	}
	buffer->Release();

	return true;
}

void Model::Release()
{
	std::vector<std::shared_ptr<Texture>>().swap(textures_);
	std::vector<D3DMATERIAL9>().swap(materials_);
	SAFE_RELEASE(mesh_);
}

void Model::Update(float df)
{
}

void Model::Render()
{
	auto device = DeviceManager::GetInstance()->GetDevice()->GetDevice();
	if (device == nullptr || mesh_ == nullptr || color_.GetA() == 0) return ;

	// ���[�h���}�g���N�X�ݒ�
	D3DXMATRIX mat;
	CreateWorldMatrix_(mat);
	device->SetTransform(D3DTS_WORLD, &mat);

	// ���_
	device->SetVertexShader(nullptr);
	device->SetFVF(mesh_->GetFVF());

	// �A���t�@�u�����h
	Renderer::GetInstance()->SetBlend(Renderer::BLEND_ALPHA);
	float r = color_.GetR() / 255.0f;
	float g = color_.GetG() / 255.0f;
	float b = color_.GetB() / 255.0f;
	float a = color_.GetA() / 255.0f;
	for (size_t i = 0; i < materials_.size(); ++i) {
		auto& dst_diff = materials_[i].Diffuse;
		auto& src_diff = diffuses_[i];
		dst_diff.r = src_diff.r * r;
		dst_diff.g = src_diff.g * g;
		dst_diff.b = src_diff.b * b;
		dst_diff.a = src_diff.a * a;
	}

	static bool check = false;
#ifdef _DEBUG
	check = KeyUtility::TrgBoolean(KeyCode::SPACE, check);
#endif

	// �`��
	if (a < 1.0f && !check) {
		// ����������
		Renderer::GetInstance()->SetBlend(Renderer::BLEND_DEST);
		device->SetTexture(0, nullptr);
		for (size_t i = 0; i < materials_.size(); ++i) {
			mesh_->DrawSubset(i);
		}
		Renderer::GetInstance()->SetBlend(Renderer::BLEND_ALPHA);
		for (size_t i = 0; i < materials_.size(); ++i) {
			device->SetMaterial(&materials_[i]);
			textures_[i]->Apply();
			mesh_->DrawSubset(i);
		}
	} else {
		// �������Ȃ�
		for (size_t i = 0; i < materials_.size(); ++i) {
			device->SetMaterial(&materials_[i]);
			textures_[i]->Apply();
			mesh_->DrawSubset(i);
		}
	}
}

void Model::CreateWorldMatrix_(D3DXMATRIX& mat)
{
	auto device = DeviceManager::GetInstance()->GetDevice()->GetDevice();
	if (device == nullptr) return ;

	D3DXMATRIX matt;
	D3DXMatrixIdentity(&mat);

	// �g�k
	if (scale_ != D3DXVECTOR3(1.0f, 1.0f, 1.0f)) {
		mat *= *D3DXMatrixScaling(&matt, scale_.x, scale_.y, scale_.z);
	}
	bool rotate = false;
	D3DXQUATERNION q = D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f);
	// Z��]
	if (rotate_.z != 0) {
		D3DXQUATERNION tq = D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f);
		q *= *D3DXQuaternionRotationAxis(&tq, &D3DXVECTOR3(0, 0, 1), rotate_.z * PI2);
		rotate = true;
	}
	// X��]
	if (rotate_.x != 0) {
		D3DXQUATERNION tq = D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f);
		q *= *D3DXQuaternionRotationAxis(&tq, &D3DXVECTOR3(1, 0, 0), rotate_.x * PI2);
		rotate = true;
	}
	// Y��]
	if (rotate_.y != 0) {
		D3DXQUATERNION tq = D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f);
		q *= *D3DXQuaternionRotationAxis(&tq, &D3DXVECTOR3(0, 1, 0), rotate_.y * PI2);
		rotate = true;
	}
	if (rotate) {
		mat *= *D3DXMatrixRotationQuaternion(&matt, &q);
	}

	// �ړ�
	mat.m[3][0] += position_.x;
	mat.m[3][1] += position_.y;
	mat.m[3][2] += position_.z;
}
