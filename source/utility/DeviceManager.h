/*!
 * @file
 *
 * @brief �f�o�C�X�Ǘ��N���X��񋟂��܂��B
 *
 * @copyright
 */
#pragma once

#include <d3dx9.h>

class AppBase;

/*!
 * @brief �f�o�C�X�Ǘ��N���X��񋟂��܂��B
 */
class DeviceManager
{
public :
	/*!
	 * @brief ������
	 */
	void Initialize(AppBase* app); 

	/*!
	 * @brief �E�C���h�E�̕��擾
	 */
	int GetWidth() const;

	/*!
	 * @brief �E�C���h�E�̍����擾
	 */
	int GetHeight() const;

	/*!
	 * @brief FPS�̐ݒ�
	 */
	void SetFPS(int fps);

	/*!
	 * @brief �I��
	 */
	void Exit();

	/*!
	 * @brief �f�o�C�X�擾
	 */
	LPDIRECT3DDEVICE9 GetDevice();

	static DeviceManager* GetInstance()	{
		static DeviceManager v;
		return &v;
	}
private :
	DeviceManager() : app_(nullptr) {}

	AppBase* app_;
};
