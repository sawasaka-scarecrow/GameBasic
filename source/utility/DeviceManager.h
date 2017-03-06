/*!
 * @file
 *
 * @brief �f�o�C�X�Ǘ��N���X��񋟂��܂��B
 *
 * @copyright
 */
#pragma once

#include "main/Device.h"

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
	Device* GetDevice();

	static DeviceManager* GetInstance()	{
		static DeviceManager v;
		return &v;
	}
private :
	DeviceManager() : app_(nullptr) {}

	AppBase* app_;
};
