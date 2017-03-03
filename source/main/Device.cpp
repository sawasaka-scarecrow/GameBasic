#include "Device.h"

#define SAFE_RELEASE(a) if (a != nullptr) { a->Release(); a = nullptr; }

Device::Device()
	: d3d_(nullptr)
	, device_(nullptr)
	, hwnd_(nullptr)
	, width_(0)
	, height_(0)
	, device_lost_(false)
	, screen_window_(true)
{
	memset(&d3dpp_, 0, sizeof(d3dpp_));
	memset(&d3dpp_window_, 0, sizeof(d3dpp_window_));
	memset(&d3dpp_full_, 0, sizeof(d3dpp_full_));
	memset(&window_rect_, 0, sizeof(window_rect_));
}

Device::~Device()
{
	Finalize();
}

/*!
 * @brief ������
 */
bool Device::Initialize(HWND hwnd, uint32_t width, uint32_t height)
{
	hwnd_ = hwnd;
	width_ = width;
	height_ = height;

	// Direct3D�I�u�W�F�N�g�̍쐬
	d3d_ = ::Direct3DCreate9(D3D_SDK_VERSION);
	if (d3d_ == nullptr) {
		return false;
	}

	// �E�C���h�E�̈ʒu�̎擾
	GetWindowRect(hwnd_, &window_rect_);

	// D3DDevice�I�u�W�F�N�g�̐ݒ�(�E�C���h�E�E���[�h�p)
	::ZeroMemory(&d3dpp_window_, sizeof(d3dpp_window_));
	d3dpp_window_.BackBufferWidth = width;
	d3dpp_window_.BackBufferHeight = height;
	d3dpp_window_.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp_window_.BackBufferCount = 1;
	d3dpp_window_.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp_window_.MultiSampleQuality = 0;
	d3dpp_window_.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp_window_.hDeviceWindow = hwnd_;
	d3dpp_window_.Windowed = TRUE;
	d3dpp_window_.EnableAutoDepthStencil = TRUE;
	d3dpp_window_.AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dpp_window_.Flags = 0;
	d3dpp_window_.FullScreen_RefreshRateInHz = 0;
	//d3dpp_window_.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
	d3dpp_window_.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	// D3DDevice�I�u�W�F�N�g�̐ݒ�(�t���X�N���[���E���[�h)
	::ZeroMemory(&d3dpp_full_, sizeof(d3dpp_full_));
	d3dpp_full_.BackBufferWidth = width;
	d3dpp_full_.BackBufferHeight = height;
	d3dpp_full_.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp_full_.BackBufferCount = 1;
	d3dpp_full_.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp_full_.MultiSampleQuality = 0;
	d3dpp_full_.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp_full_.hDeviceWindow = hwnd_;
	d3dpp_full_.Windowed = FALSE;
	d3dpp_full_.EnableAutoDepthStencil = TRUE;
	d3dpp_full_.AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dpp_full_.Flags = 0;
	d3dpp_full_.FullScreen_RefreshRateInHz = 60;
	//d3dpp_full_.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
	d3dpp_full_.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	// D3DDevice�I�u�W�F�N�g�̍쐬
	d3dpp_ = d3dpp_window_;

	// �\���p�̃f�o�C�X���쐬
	HRESULT hr;

	// �f�o�C�X�쐬
	bool ret = false;
	hr = d3d_->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd_, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp_, &device_);
	if (!SUCCEEDED(hr)) {
		hr = d3d_->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd_, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp_, &device_);
		if (!SUCCEEDED(hr)) {
			hr = d3d_->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hwnd_, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp_, &device_);
			if (!SUCCEEDED(hr)) {
				return false;
			}
		}
	}

	// �X�e�[�g�̏�����
	InitRenderState_();

	return true;
}

void Device::Finalize()
{
	SAFE_RELEASE(device_);
	SAFE_RELEASE(d3d_);
}

bool Device::CheckIdle()
{
	if (d3d_ == nullptr || device_ == nullptr) {
		PostMessage(hwnd_, WM_DESTROY, 0, 0);
		return false;
	}

	if (device_lost_) {
		// �f�o�C�X��Ԃ̃`�F�b�N
		HRESULT hr  = device_->TestCooperativeLevel();
		if (FAILED(hr)) {
			if (hr == D3DERR_DEVICELOST) {
				return false;  // �f�o�C�X�͂܂������Ă���
			}
			if (hr != D3DERR_DEVICENOTRESET) {
				//GpLog::SetString("%s(%d)�FAppIdle��D3DERR_DEVICENOTRESET�ɂȂ�܂����B\n", std::strrchr(__FILE__, '\\') + 1, __LINE__);
				PostMessage(hwnd_, WM_DESTROY, 0, 0);
				return false;	// �\�����ʃG���[
			}

			OnLostDevice_();
			hr = device_->Reset(&d3dpp_); // ���������݂�
			if (FAILED(hr)) {
				if (hr == D3DERR_DEVICELOST) {
					return false; // �f�o�C�X�͂܂������Ă���
				}
				//GpLog::SetString("%s(%d)�FAppIdle�̃��Z�b�g�Ɏ��s���܂����B\n", std::strrchr(__FILE__, '\\') + 1, __LINE__);
				PostMessage(hwnd_, WM_DESTROY, 0, 0);
				return false;
			}
			// �������Direct3D�ŊǗ����Ă��Ȃ����\�[�X���Ď擾
			InitRenderState_();
			if (FAILED(hr)) {
				return false;
			}
		}
		// �f�o�C�X����������
		device_lost_ = false;
	}

	return true;
}

void Device::CheckDevice()
{
	if (device_ == nullptr)  return ;

	HRESULT hr  = device_->TestCooperativeLevel();
	if (hr == D3DERR_DEVICELOST) {
		device_lost_ = true;
	}
}

/*!
 * @brief �X�N���[���̐؂�ւ�
 */
void Device::ChangeScreen(bool window)
{
	if (device_ == nullptr)  return ;

	static bool last_window;

	screen_window_ = window;
	if (screen_window_) {
		d3dpp_ = d3dpp_window_;
	} else {
		d3dpp_ = d3dpp_full_;
		GetWindowRect(hwnd_, &window_rect_);
	}
	OnLostDevice_();

	HRESULT hr = device_->Reset(&d3dpp_);
	if (FAILED(hr)) {
		if (hr == D3DERR_DEVICELOST) {
			device_lost_ = true;
			return ;
		} else {
			//GpLog::SetString("%s(%d)�FChangeScreen�̃��Z�b�g�Ɏ��s���܂����B\n", std::strrchr(__FILE__, '\\') + 1, __LINE__);
			//PostMessage(hwnd_, WM_DESTROY, 0, 0);
			return ;
		}
	}
	
	InitRenderState_();

	if (screen_window_) {
		SetWindowLong(hwnd_, GWL_STYLE, WS_OVERLAPPEDWINDOW);
		ShowWindow(hwnd_, SW_SHOWNORMAL);
		UpdateWindow(hwnd_);
		if (!last_window) {
			SetWindowPos(hwnd_, HWND_NOTOPMOST,
						 window_rect_.left, window_rect_.top,
						 window_rect_.right - window_rect_.left,
						 window_rect_.bottom - window_rect_.top,
						 SWP_SHOWWINDOW);
		}
	} else {
		SetWindowLong(hwnd_, GWL_STYLE, WS_POPUP | WS_VISIBLE);
		ShowWindow(hwnd_, SW_SHOWNORMAL);
		UpdateWindow(hwnd_);
	}
	Sleep(100);

	if (screen_window_) {
		last_window = true;
	} else {
		last_window = false;
	}
}

void Device::SetBackBufferSize(uint32_t width, uint32_t height)
{
	if (device_ == nullptr)  return ;

	d3dpp_window_.BackBufferWidth = width;
	d3dpp_window_.BackBufferHeight = height;
	d3dpp_ = d3dpp_window_;

	OnLostDevice_();
	HRESULT hr = device_->Reset(&d3dpp_);
	if (FAILED(hr)) {
		if (hr == D3DERR_DEVICELOST) {
			device_lost_ = true;
			return;
		}
	}

	InitRenderState_();
}

void Device::AddDeviceLostListener(DeviceLostListener* listener)
{
	device_lost_listeners_.insert(listener);
}

void Device::RemoveDeviceLostListener(DeviceLostListener* listener)
{
	device_lost_listeners_.erase(listener);
}

/*!
 * @brief �I�u�W�F�N�g�̏�������
 */
void Device::InitRenderState_()
{
	if (device_ == nullptr)  return ;

	float	fFogStart = 0.0f, fFogEnd = 1.0f,  fFogDensity = 1.0f,  PointSize = 1.0f,
			PointSizeMin = 1.0f, PointScaleA = 1.0f, PointScaleB = 0.0f, PointScaleC = 0.0f,
			PointSizeMax = 64.0f, TweenFactor = 0.0f;

	// 3D�f�o�C�X����
	device_->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);											// �ʂ�h��Ԃ�
	device_->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);										// �O���[�V���[�f�B���O���[�h
	device_->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);												// �[�x�o�b�t�@�ւ̏������݂�L��
	device_->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);											// �s�N�Z�����Ƃ̃A���t�@ �e�X�g��L��
	device_->SetRenderState(D3DRS_LASTPIXEL, TRUE);													// ���ōŌ�̃s�N�Z���̕`���L��
	device_->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);											// �J��������̋�������Ƃ���s�N�Z���̎󂯎���󂯎�苑�ۂ����� 
	device_->SetRenderState(D3DRS_ALPHAREF, 0x00000000);											// �s�N�Z�����e�X�g���邽�߂̊�A���t�@�l���w��
	device_->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);										// �A�v���P�[�V�����ɃA���t�@�l����Ƃ���s�N�Z���̎󂯎���󂯎�苑�ۂ�����
	device_->SetRenderState(D3DRS_DITHERENABLE, FALSE);												// ���\�[�X�̎�ނ��`����B
	device_->SetRenderState(D3DRS_FOGENABLE, FALSE);												// �t�H�O �u�����f�B���O�̗L��
	device_->SetRenderState(D3DRS_SPECULARENABLE, FALSE);											// �X�y�L���� �n�C���C�g�̗L��
	device_->SetRenderState(D3DRS_FOGCOLOR, 0);														// MicrosoftR Direct3DR �̊�{�I�ȃJ���[ �^�C�v���`����B
	device_->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_NONE);										// �s�N�Z�� �t�H�O�Ɏg����t�H�O��
	device_->SetRenderState(D3DRS_FOGSTART, *(reinterpret_cast<DWORD*>(&fFogStart)));				// �s�N�Z���܂��͒��_�t�H�O �G�t�F�N�g���J�n����[�x
	device_->SetRenderState(D3DRS_FOGEND, *(reinterpret_cast<DWORD*>(&fFogEnd)));					// �s�N�Z���܂��͒��_�t�H�O �G�t�F�N�g���I������[�x
	device_->SetRenderState(D3DRS_FOGDENSITY, *(reinterpret_cast<DWORD*>(&fFogDensity)));			// �s�N�Z���܂��͒��_�t�H�O�̃t�H�O���x
	device_->SetRenderState(D3DRS_RANGEFOGENABLE, FALSE);											// �͈̓x�[�X�̒��_�t�H�O�̗L��
	device_->SetRenderState(D3DRS_STENCILENABLE, FALSE);											// �X�e���V�������̗L��
	device_->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);									// �X�e���V�� �e�X�g�Ɏ��s�����Ƃ��Ɏ��s����X�e���V������
	device_->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);									// �[�x�e�X�g (Z �e�X�g) �Ɏ��s�����ꍇ�Ɏ��s����X�e���V������
	device_->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);									// �X�e���V�� �e�X�g����ѐ[�x (Z) �e�X�g�̗����Ƀp�X�����ꍇ�Ɏ��s����X�e���V������
	device_->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);										// �X�e���V�� �e�X�g�̂��߂̔�r�֐�
	device_->SetRenderState(D3DRS_STENCILREF, 0);													// �X�e���V�� �e�X�g�̂��߂� int ��l
	device_->SetRenderState(D3DRS_STENCILMASK, 0xFFFFFFFF);											// �X�e���V�� �e�X�g�̂��߂̗L���r�b�g�����肷��}�X�N
	device_->SetRenderState(D3DRS_STENCILWRITEMASK, 0xFFFFFFFF);									// �X�e���V�� �o�b�t�@�ɏ������ޒl�ɓK�p���鏑�����݃}�X�N
	device_->SetRenderState(D3DRS_TEXTUREFACTOR, 0xFFFFFFFF);										// �u�����f�B���O�����ɂ��}���`�e�N�X�`�� �u�����f�B���O�Ŏg����F
	device_->SetRenderState(D3DRS_WRAP0, 0);														// �����̃e�N�X�`�����W�Z�b�g�ɑ΂���e�N�X�`�����b�s���O����
	device_->SetRenderState(D3DRS_WRAP1, 0);														// �����̃e�N�X�`�����W�Z�b�g�ɑ΂���e�N�X�`�����b�s���O����
	device_->SetRenderState(D3DRS_WRAP2, 0);														// �����̃e�N�X�`�����W�Z�b�g�ɑ΂���e�N�X�`�����b�s���O����
	device_->SetRenderState(D3DRS_WRAP3, 0);														// �����̃e�N�X�`�����W�Z�b�g�ɑ΂���e�N�X�`�����b�s���O����
	device_->SetRenderState(D3DRS_WRAP4, 0);														// �����̃e�N�X�`�����W�Z�b�g�ɑ΂���e�N�X�`�����b�s���O����
	device_->SetRenderState(D3DRS_WRAP5, 0);														// �����̃e�N�X�`�����W�Z�b�g�ɑ΂���e�N�X�`�����b�s���O����
	device_->SetRenderState(D3DRS_WRAP6, 0);														// �����̃e�N�X�`�����W�Z�b�g�ɑ΂���e�N�X�`�����b�s���O����
	device_->SetRenderState(D3DRS_WRAP7, 0);														// �����̃e�N�X�`�����W�Z�b�g�ɑ΂���e�N�X�`�����b�s���O����
	device_->SetRenderState(D3DRS_WRAP8, 0);														// �����̃e�N�X�`�����W�Z�b�g�ɑ΂���e�N�X�`�����b�s���O����
	device_->SetRenderState(D3DRS_WRAP9, 0);														// �����̃e�N�X�`�����W�Z�b�g�ɑ΂���e�N�X�`�����b�s���O����
	device_->SetRenderState(D3DRS_WRAP10, 0);														// �����̃e�N�X�`�����W�Z�b�g�ɑ΂���e�N�X�`�����b�s���O����
	device_->SetRenderState(D3DRS_WRAP11, 0);														// �����̃e�N�X�`�����W�Z�b�g�ɑ΂���e�N�X�`�����b�s���O����
	device_->SetRenderState(D3DRS_WRAP12, 0);														// �����̃e�N�X�`�����W�Z�b�g�ɑ΂���e�N�X�`�����b�s���O����
	device_->SetRenderState(D3DRS_WRAP13, 0);														// �����̃e�N�X�`�����W�Z�b�g�ɑ΂���e�N�X�`�����b�s���O����
	device_->SetRenderState(D3DRS_WRAP14, 0);														// �����̃e�N�X�`�����W�Z�b�g�ɑ΂���e�N�X�`�����b�s���O����
	device_->SetRenderState(D3DRS_WRAP15, 0);														// �����̃e�N�X�`�����W�Z�b�g�ɑ΂���e�N�X�`�����b�s���O����
	device_->SetRenderState(D3DRS_CLIPPING, TRUE);													// MicrosoftR Direct3DR �ɂ��v���~�e�B�u�̃N���b�s���O�̗L��
	device_->SetRenderState(D3DRS_AMBIENT, 0);														// �A���r�G���g ���C�g�̐F
	device_->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_NONE);										// ���_�t�H�O�Ŏg����t�H�O�� 
	device_->SetRenderState(D3DRS_COLORVERTEX, TRUE);												// ���_�P�ʂ̐F�̗L��
	device_->SetRenderState(D3DRS_LOCALVIEWER, TRUE);												// �J�����Ƃ̑��Ίp�x�Ɉˑ������X�y�L���� �n�C���C�g�̗L��
	device_->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);											// ���_�@���̎������K���̗L��
	device_->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1);							// ���C�e�B���O�v�Z�Ɏg����f�B�t���[�Y�F�̃\�[�X
	device_->SetRenderState(D3DRS_SPECULARMATERIALSOURCE, D3DMCS_COLOR2);							// ���C�e�B���O�v�Z�Ɏg����X�y�L�����F�̃\�[�X
	device_->SetRenderState(D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_MATERIAL);							// ���C�e�B���O�v�Z�Ɏg����A���r�G���g�F�̃\�[�X
	device_->SetRenderState(D3DRS_EMISSIVEMATERIALSOURCE, D3DMCS_MATERIAL);							// ���C�e�B���O�v�Z�Ɏg����G�~�b�V�����F�̃\�[�X
	device_->SetRenderState(D3DRS_VERTEXBLEND, D3DVBF_DISABLE);										// �W�I���g��������ꍇ�A�W�I���g�� �u�����f�B���O�����s���邽�߂Ɏg���s��̌�
	device_->SetRenderState(D3DRS_CLIPPLANEENABLE, 0);												// ���[�U�[��`�̃N���b�v�ʂ�L���܂��͖����ɂ���
	device_->SetRenderState(D3DRS_POINTSIZE,	*(reinterpret_cast<uint32_t*>(&PointSize)));		// �e���_�ɑ΂��ă|�C���g �T�C�Y���w�肳��Ă��Ȃ��ꍇ�ɁA�|�C���g �T�C�Y�v�Z�Ŏg���T�C�Y���w�肷�� float�l
	device_->SetRenderState(D3DRS_POINTSIZE_MIN, *(reinterpret_cast<uint32_t*>(&PointSizeMin)));	// �|�C���g �v���~�e�B�u�̍ŏ��T�C�Y���w�肷�� float �l
	device_->SetRenderState(D3DRS_POINTSPRITEENABLE, FALSE);										// BOOL �l
	device_->SetRenderState(D3DRS_POINTSCALEENABLE, FALSE);											// �|�C���g �v���~�e�B�u�ɑ΂���T�C�Y�̌v�Z�𐧌䂷�� BOOL �l
	device_->SetRenderState(D3DRS_POINTSCALE_A, *(reinterpret_cast<uint32_t*>(&PointScaleA)));		// �|�C���g �v���~�e�B�u�ɑ΂��鋗���x�[�X�̃T�C�Y�̌����𐧌䂷�� float �l
	device_->SetRenderState(D3DRS_POINTSCALE_B, *(reinterpret_cast<uint32_t*>(&PointScaleB)));		// �|�C���g �v���~�e�B�u�ɑ΂��鋗���x�[�X�̃T�C�Y�̌����𐧌䂷�� float �l
	device_->SetRenderState(D3DRS_POINTSCALE_C, *(reinterpret_cast<uint32_t*>(&PointScaleC)));		// �|�C���g �v���~�e�B�u�ɑ΂��鋗���x�[�X�̃T�C�Y�̌����𐧌䂷�� float �l
	device_->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, TRUE);										// �}���`�T���v�� �����_�����O�̃^�[�Q�b�g �o�b�t�@���g���Ƃ��́A�X�̃T���v���̌v�Z���@�����肷�� 
	device_->SetRenderState(D3DRS_MULTISAMPLEMASK, 0xFFFFFFFF);										// �}���`�T���v�� �o�b�t�@��~�σo�b�t�@�Ƃ��Ďg���A�e�p�X���T���v���̃T�u�Z�b�g���X�V����悤�ȃW�I���g���̃}���`�p�X �����_�����O���s�����Ƃ��ł���B
	device_->SetRenderState(D3DRS_PATCHEDGESTYLE, D3DPATCHEDGE_DISCRETE);							// �p�b�` �G�b�W�����������_���X�^�C���̃e�Z���[�V�������g�����ǂ�����ݒ肷��
	device_->SetRenderState(D3DRS_DEBUGMONITORTOKEN, D3DDMT_ENABLE);								// ���j�^���f�o�b�O����ꍇ�ɂ̂ݐݒ肷��
	device_->SetRenderState(D3DRS_POINTSIZE_MAX, *(reinterpret_cast<uint32_t*>(&PointSizeMax)));	// �|�C���g �X�v���C�g�����������ő�T�C�Y���w�肷�� float �l
	device_->SetRenderState(D3DRS_INDEXEDVERTEXBLENDENABLE, FALSE);									// �C���f�b�N�X�t���̒��_�u�����f�B���O��L���܂��͖����ɂ��� BOOL �l
	device_->SetRenderState(D3DRS_COLORWRITEENABLE, 0x0000000F);									// �����_�����O �^�[�Q�b�g�̃J���[ �o�b�t�@�ɑ΂���`�����l�����Ƃ̏������݂�L���ɂ��� UINT �l
	device_->SetRenderState(D3DRS_TWEENFACTOR, *(reinterpret_cast<uint32_t*>(&TweenFactor)));		// �g�D�C�[���W���𐧌䂷�� float �l
	device_->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);											// �A���t�@ �u�����f�B���O �����_�����O �X�e�[�g��TRUE�ɐݒ肳��Ă���ꍇ�ɓK�p����Z�p���Z�̑I���Ɏg����l
	device_->SetRenderState(D3DRS_POSITIONDEGREE, D3DDEGREE_CUBIC);									// N �p�b�`�ʒu��Ԏ���
	device_->SetRenderState(D3DRS_NORMALDEGREE,  D3DDEGREE_LINEAR);									// N �p�b�`�@����Ԏ���
	device_->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);										// �V�U�[�e�X�g�̗L��
	device_->SetRenderState(D3DRS_SLOPESCALEDEPTHBIAS, 0);											// z �t�@�C�e�B���O�����炷���߂ɁA���ꕽ�ʏ�̃v���~�e�B�u�ɂǂꂾ���o�C�A�X��K�p���邩�����肷��̂Ɏg��
	device_->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, FALSE);									// ���̃A���`�G�C���A�V���O�̗L��
	device_->SetRenderState(D3DRS_TWOSIDEDSTENCILMODE, FALSE);										// 2 �ʂ̃X�e���V���̗L��
	device_->SetRenderState(D3DRS_CCW_STENCILFAIL, 0x00000001);										// �����v���̃X�e���V�� �e�X�g�Ɏ��s�����Ƃ��Ɏ��s����X�e���V������
	device_->SetRenderState(D3DRS_CCW_STENCILZFAIL, 0x00000001);									// �����v���̃X�e���V�� �e�X�g�Ƀp�X���AZ �e�X�g�Ɏ��s�����ꍇ�Ɏ��s����X�e���V������
	device_->SetRenderState(D3DRS_CCW_STENCILPASS, 0x00000001);										// �����v���̃X�e���V�� �e�X�g����� Z �e�X�g�̗����Ƀp�X�����ꍇ�Ɏ��s����X�e���V������
	device_->SetRenderState(D3DRS_CCW_STENCILFUNC, 0x00000008);										// ��r�֐��B�����v���̃X�e���V�� �e�X�g�Ƀp�X����̂�((ref & mask)�X�e���V���֐�(stencil & mask))��TRUE�̏ꍇ
	device_->SetRenderState(D3DRS_COLORWRITEENABLE1, 0x0000000f);									// �f�o�C�X�ɑ΂���ǉ��� ColorWriteEnable �̒l
	device_->SetRenderState(D3DRS_COLORWRITEENABLE2, 0x0000000f);									// �f�o�C�X�ɑ΂���ǉ��� ColorWriteEnable �̒l
	device_->SetRenderState(D3DRS_COLORWRITEENABLE3, 0x0000000f);									// �f�o�C�X�ɑ΂���ǉ��� ColorWriteEnable �̒l
	device_->SetRenderState(D3DRS_BLENDFACTOR, 0xffffffff);											// �A���t�@ �u�����f�B���O�����̊Ԃɒ萔�u�����f�B���O�W���Ɏg�� D3DCOLOR
	device_->SetRenderState(D3DRS_SRGBWRITEENABLE, 0);												// �����_�����O �^�[�Q�b�g�ւ̏������݂ŁAsRGB �ւ̃K���}�␳��L���ɂ���
	device_->SetRenderState(D3DRS_DEPTHBIAS, 0);													// �[�x�l�̔�r�Ɏg�����������_�l
	device_->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, FALSE);									// �A���t�@ �`�����l���ɑ΂���ʂ̃u�����f�B���O ���[�h�̗L��
	device_->SetRenderState(D3DRS_SRCBLENDALPHA, D3DBLEND_ONE);										// D3DBLEND �񋓌^�̃����o�̂����ꂩ�ł���
	device_->SetRenderState(D3DRS_DESTBLENDALPHA, D3DBLEND_ZERO);									// D3DBLEND �񋓌^�̃����o�̂����ꂩ�ł���
	device_->SetRenderState(D3DRS_BLENDOPALPHA, D3DBLENDOP_ADD);									// �����_�����O �X�e�[�g�� D3DRS_ALPHABLENDENABLE �� TRUE �ɐݒ肳��Ă���ꍇ�ɁA�ʂ̃A���t�@ �u�����f�B���O�ɓK�p����Z�p���Z�̑I���Ɏg���l�B 
	device_->SetRenderState(D3DRS_ZENABLE, D3DZB_USEW);												// Z�o�b�t�@��L��
	device_->SetRenderState(D3DRS_AMBIENT, 0xffffffff);												// �A���r�G���g���C�g
	device_->SetRenderState(D3DRS_LIGHTING, TRUE);													// ���C�g��L��
	device_->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);											// �J�����O�̐ݒ�
	device_->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);											// �A���t�@�u�����h��L��

	// �t�B���^�ݒ�
	device_->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);			// �~�b�v�}�b�v
	device_->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);			// �g��
	device_->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);			// �k��
	device_->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);		// �e�N�X�`���̐F���g�p
	device_->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);		// ���_�̐F���g�p
	device_->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);		// �����_�����O���̐F�̌v�Z���@�̐ݒ�
	device_->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);		// �e�N�X�`���̐F���g�p
	device_->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);		// ���_�̐F���g�p

	// �r���[�|�[�g
	D3DVIEWPORT9 view = { 0, 0, width_, height_, 0.0f, 1.0f };
	device_->SetViewport(&view);

	// ���C�g
	D3DLIGHT9 light;
	ZeroMemory(&light, sizeof(D3DLIGHT9));
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Diffuse.r = 1.0f;
	light.Diffuse.g = 1.0f;
	light.Diffuse.b = 1.0f;
	light.Direction = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	device_->SetLight(0, &light);
	device_->LightEnable(0, TRUE);

	// �f�o�C�X���Z�b�g
	OnResetDevice_();
}

void Device::OnLostDevice_()
{
	for (auto it = device_lost_listeners_.begin(); it != device_lost_listeners_.end(); ++it) {
		(*it)->OnLostDevice();
	}
}

void Device::OnResetDevice_()
{
	for (auto it = device_lost_listeners_.begin(); it != device_lost_listeners_.end(); ++it) {
		(*it)->OnResetDevice();
	}
}
