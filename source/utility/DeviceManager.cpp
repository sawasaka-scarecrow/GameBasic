#include "DeviceManager.h"
#include "main/WinMain.h"

/*!
 * @brief ������
 */
void DeviceManager::Initialize(AppBase* app)
{
	app_ = app;
}

/*!
 * @brief �E�C���h�E�̕��擾
 */
int DeviceManager::GetWidth() const
{
	if (app_ == nullptr) return 0;
	return app_->GetWidth();
}

/*!
 * @brief �E�C���h�E�̍����擾
 */
int DeviceManager::GetHeight() const
{
	if (app_ == nullptr) return 0;
	return app_->GetHeight();
}

/*!
 * @brief FPS�̐ݒ�
 */
void DeviceManager::SetFPS(int fps)
{
	if (app_ == nullptr) return;
	app_->SetFPS(fps);
}
