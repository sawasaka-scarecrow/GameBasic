/*!
 * @file
 *
 * @brief �f�o�C�X�Ǘ��N���X��񋟂��܂��B
 *
 * @copyright
 */
#pragma once

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

	static DeviceManager* GetInstance()
	{
		static DeviceManager v;
		return &v;
	}
private :
	DeviceManager() : app_(nullptr) {}

	AppBase* app_;
};
