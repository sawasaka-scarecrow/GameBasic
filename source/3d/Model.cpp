#include "Model.h"
#include "utility/DeviceManager.h"
#include "utility/Utility.hpp"
#include "utility/KeyManager.h"

#define SAFE_RELEASE(a) if (a != nullptr) { a->Release(); a = nullptr; }
static const float RotateBase = 6.28318530718f;

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

bool Model::LoadFile(const char* path)
{
	auto device = DeviceManager::GetInstance()->GetDevice();
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
		materials_[i].Ambient = materials_[i].Diffuse;
		diffuses_[i] = materials_[i].Diffuse;

		//�e�N�X�`�������[�h
		textures_[i] = nullptr;
		if (d3dxmatrs[i].pTextureFilename == nullptr)
			continue ;

		auto dir = Utility::GetDirectoryName(path);
		auto name = Utility::GetFileName(d3dxmatrs[i].pTextureFilename);
		auto tex_path = Utility::StringFormat("%s/%s", dir.c_str(), name.c_str());

		if (S_OK != D3DXCreateTextureFromFileA(device, tex_path.c_str(), &textures_[i])) {
			textures_[i] = nullptr;
		}
	}
	buffer->Release();

	return true;
}

void Model::Release()
{
	for (size_t i = 0; i < textures_.size(); ++i) {
		SAFE_RELEASE(textures_[i]);
	}
	std::vector<LPDIRECT3DTEXTURE9>().swap(textures_);
	std::vector<D3DMATERIAL9>().swap(materials_);
	SAFE_RELEASE(mesh_);
}

void Model::Update(float df)
{
	auto device = DeviceManager::GetInstance()->GetDevice();
	if (device == nullptr || mesh_ == nullptr) return;
}

void Model::Render()
{
	auto device = DeviceManager::GetInstance()->GetDevice();
	if (device == nullptr || mesh_ == nullptr) return ;

	D3DXMATRIX mat, matt;
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
		q *= *D3DXQuaternionRotationAxis(&tq, &D3DXVECTOR3(0, 0, 1), rotate_.z * RotateBase);
		rotate = true;
	}
	// X��]
	if (rotate_.x != 0) {
		D3DXQUATERNION tq = D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f);
		q *= *D3DXQuaternionRotationAxis(&tq, &D3DXVECTOR3(1, 0, 0), rotate_.x * RotateBase);
		rotate = true;
	}
	// Y��]
	if (rotate_.y != 0) {
		D3DXQUATERNION tq = D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f);
		q *= *D3DXQuaternionRotationAxis(&tq, &D3DXVECTOR3(0, 1, 0), rotate_.y * RotateBase);
		rotate = true;
	}
	if (rotate) {
		mat *= *D3DXMatrixRotationQuaternion(&matt, &q);
	}

	// �ړ�
	mat.m[3][0] += position_.x;
	mat.m[3][1] += position_.y;
	mat.m[3][2] += position_.z;
	device->SetTransform(D3DTS_WORLD, &mat);

	// �A���t�@�u�����h
	Renderer::GetInstance()->SetBlend(Renderer::BLEND_ALPHA);

	device->SetVertexShader(nullptr);
	device->SetFVF(mesh_->GetFVF());

	/*
	static bool hoge = false;
	if (KeyManager::GetInstance()->IsTrg('L')) {
		hoge = !hoge;
	}
	*/

	float r = color_.GetR() / 255.0f;
	float g = color_.GetG() / 255.0f;
	float b = color_.GetB() / 255.0f;
	float a = color_.GetA() / 255.0f;
	if (a < 1.0f) {
		// ����������
		device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		for (size_t i = 0; i < materials_.size(); ++i) {
			materials_[i].Diffuse.a = 0;
			device->SetMaterial(&materials_[i]);
			device->SetTexture(0, nullptr);
			mesh_->DrawSubset(i);
		}
		for (size_t i = 0; i < materials_.size(); ++i) {
			auto& dst_diff = materials_[i].Diffuse;
			auto& src_diff = diffuses_[i];
			dst_diff.r = src_diff.r * r;
			dst_diff.g = src_diff.g * g;
			dst_diff.b = src_diff.b * b;
			dst_diff.a = src_diff.a * a;

			device->SetMaterial(&materials_[i]);
			device->SetTexture(0, textures_[i]);
			mesh_->DrawSubset(i);
		}
	} else {
		// �������Ȃ�
		for (size_t i = 0; i < materials_.size(); ++i) {
			auto& dst_diff = materials_[i].Diffuse;
			auto& src_diff = diffuses_[i];
			dst_diff.r = src_diff.r * r;
			dst_diff.g = src_diff.g * g;
			dst_diff.b = src_diff.b * b;
			dst_diff.a = src_diff.a * a;

			device->SetMaterial(&materials_[i]);
			device->SetTexture(0, textures_[i]);
			mesh_->DrawSubset(i);
		}
	}
}
