/*!
 * @file
 *
 * @brief InOut�A�j���[�V�����Ǘ��N���X��񋟂��܂��B
 *
 * @copyright
 */
#pragma once

#include "bezier.h"
#include "DeviceManager.h"
#include <vector>
#include <string>
#include <map>

/*!
 * @brief InOut�A�j���[�V�����Ǘ��N���X
 */
template <class Data, class Animation>
class InOutAnimation
{
public :
	/*!
	 * @brief �J�n�A�j���[�V�������Z�b�g���܂��B
	 */
	template <class UI>
	void SetIn(UI& ui, const std::map<std::string, Animation>& inout) {
		for (auto it = ui.begin(); it != ui.end(); ++it) {
			auto it2 = inout.find(it->name);
			if (it2 == inout.end()) continue;

			auto& anim = it2->second;
			Info info;
			info.data = &(*it);
			info.type = anim.type;
			info.wait_time = anim.delay;
			if (info.type == 0) {
				// �ړ�
				int xvalue = (anim.param1 % 3 - 1) * DeviceManager::GetInstance()->GetWidth();
				int yvalue = (anim.param1 / 3 - 1) * DeviceManager::GetInstance()->GetHeight();

				info.bezier_.resize(2);
				info.bezier_[0].Set(info.data->x + xvalue, info.data->x, anim.time, Bezier::EaseOut);
				info.bezier_[1].Set(info.data->y + yvalue, info.data->y, anim.time, Bezier::EaseOut);
				info.data->x = info.bezier_[0];
				info.data->y = info.bezier_[1];
			} else if (info.type == 1) {
				// �A���t�@
				info.bezier_.resize(1);
				info.bezier_[0].Set(0, info.data->a, anim.time, Bezier::EaseOut);
				info.data->a = 0;
			}
			infos_.push_back(info);
		}
	}

	/*!
	* @brief �I���A�j���[�V�������Z�b�g���܂��B
	*/
	template <class UI>
	void SetOut(UI& ui, const std::map<std::string, Animation>& inout)
	{
		for (auto it = ui.begin(); it != ui.end(); ++it) {
			auto it2 = inout.find(it->name);
			if (it2 == inout.end()) continue;

			auto& anim = it2->second;
			Info info;
			info.data = &(*it);
			info.type = anim.type;
			info.wait_time = anim.delay;
			if (info.type == 0) {
				// �ړ�
				int xvalue = (anim.param1 % 3 - 1) * DeviceManager::GetInstance()->GetWidth();
				int yvalue = (anim.param1 / 3 - 1) * DeviceManager::GetInstance()->GetHeight();

				info.bezier_.resize(2);
				info.bezier_[0].Set(info.data->x, info.data->x - xvalue, anim.time, Bezier::EaseOut);
				info.bezier_[1].Set(info.data->y, info.data->y - yvalue, anim.time, Bezier::EaseOut);
			}
			else if (info.type == 1) {
				// �A���t�@
				info.bezier_.resize(1);
				info.bezier_[0].Set(info.data->a, 0, anim.time, Bezier::EaseOut);
			}
			infos_.push_back(info);
		}
	}

	/*!
	 * @brief �X�V
	 *
	 * @param df �O�̃t���[���Ƃ̍�������
	 */
	void Update(float df) {
		for (size_t i = 0; i < infos_.size(); ++i) {
			auto& info = infos_[i];
			info.wait_time -= df;
			if (info.wait_time > 0) continue;

			if (info.type == 0) {
				// �ړ�
				info.data->x = info.bezier_[0];
				info.data->y = info.bezier_[1];
			} else if (info.type == 1) {
				// �A���t�@
				info.data->a = info.bezier_[0];
			}
			for (size_t j = 0; j < info.bezier_.size(); ++j) {
				info.bezier_[j].Update(df);
			}
		}
	}

	/*!
	* @brief �I���̗L�����擾���܂��B
	*
	* @param df �O�̃t���[���Ƃ̍�������
	*/
	bool IsEnd() const
	{
		for (size_t i = 0; i < infos_.size(); ++i) {
			const auto& info = infos_[i];
			if (info.wait_time > 0) return false;

			for (size_t j = 0; j < info.bezier_.size(); ++j) {
				if (!info.bezier_[j].IsEnd())
					return false;
			}
		}
		return true;
	}

private :
	struct Info {
		Data* data;
		int type;
		float wait_time;
		std::vector<Bezier::Timer<int>> bezier_;
	};

	std::vector<Info> infos_;
};
