#pragma once

#include <vector>
#include <string>
#include <d3dx9.h>
#include <functional>
#include <memory>
#include "render/Texture.h"
#include "render/Color.h"

/*!
 * @brief ���f��
 */
class Model
{
public :
	Model();
	~Model();

	virtual bool LoadFile(const char* path, std::function<std::shared_ptr<Texture>(const char* name)> texture_func = nullptr);
	void Release();

	virtual void Update(float df);
	void Render();

	void SetPotision(const D3DXVECTOR3& position) { position_ = position; }
	void SetPotision(float x, float y, float z) { position_ = D3DXVECTOR3(x, y, z); }
	void SetRotate(const D3DXVECTOR3& rotate) { rotate_ = rotate; }
	void SetRotate(float x, float y, float z) { rotate_ = D3DXVECTOR3(x, y, z); }
	void SetScale(const D3DXVECTOR3& scale) { scale_ = scale; }
	void SetScale(float x, float y, float z) { scale_ = D3DXVECTOR3(x, y, z); }
	void SetColor(const Color& color) { color_ = color; }

	const D3DXVECTOR3& GetPotision() const { return position_; }
	const D3DXVECTOR3& GetRotate() const { return rotate_; }
	const D3DXVECTOR3& GetScale() const { return scale_; }
	const Color& GetColor() const { return color_; }

private :
	LPD3DXMESH mesh_;
	std::vector<D3DMATERIAL9> materials_;
	std::vector<D3DCOLORVALUE> diffuses_;
	std::vector<std::shared_ptr<Texture>> textures_;
	D3DXVECTOR3 position_;
	D3DXVECTOR3 rotate_;
	D3DXVECTOR3 scale_;
	Color color_;
};
