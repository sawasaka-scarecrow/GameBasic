#pragma once

#include "Camera.h"

/*!
 * @brief �f�o�b�O�J����
 */
class DebugCamera : public Camera
{
public :
	DebugCamera();
	virtual ~DebugCamera();

	virtual void Update(float df);

private :
	float move_[5];
};
