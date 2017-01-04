/*!
 * @file
 *
 * @brief ��ԊǗ��N���X��񋟂��܂��B
 *
 * @copyright
 */
#pragma once

/*!
 * @brief ��ԊǗ��N���X
 */
template <class State>
class StateManager
{
public :
	StateManager() : wait_time_(0) {}

	/*!
	 * @brief ��Ԃ��m�F����
	 *
	 * @param df ���ԍ������w��
	 */
	bool Check(float df) {
		wait_time_ -= df;
		if (wait_time_ <= 0) {
			wait_time_ = 0;
			if ((int)next_state_ != -1) {
				state_ = next_state_;
				next_state_ = (State)-1;
				return true;
			}
		}

		return false;
	}

	/*!
	 * @brief ��Ԃ�؂�ւ���
	 *
	 * @param state ���̏�Ԃ��w�肷��
	 * @param immediate �����ؑւ̗L�����w�肷��
	 */
	void Change(State state, bool immediate = false) {
		if (immediate) {
			state_ = state;
		} else {
			next_state_ = state;
		}
	}

	/*!
	 * @brief ��Ԃ�؂�ւ���
	 *
	 * @param state ���̏�Ԃ��w�肷��
	 * @param time ���ɐ؂�ւ��܂ł̑҂����Ԃ��w�肷��
	 */
	void ChangeDelay(State state, float time) {
		next_state_ = state;
		wait_time_ = time;
	}

	/*!
	 * @brief ��Ԃ��͈͓��ɂ��邩�m�F����
	 *
	 * @param start �͈͂̊J�n
	 * @param end �͈͂̏I��
	 *
	 * @return �͈͓��ɂ���ꍇ�� true ��Ԃ�
	 */
	bool IsRange(State start, State end) {
		return start <= state_ && state_ <= end;
	}

	operator State() const { return state_; }
	State operator=(State state){ state_ = state; return state; }
	State Get() const{ return state_; }

private :
	State state_;
	State next_state_;
	float wait_time_;
};
