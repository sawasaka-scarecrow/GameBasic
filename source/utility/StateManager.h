#pragma once

template <class State>
class StateManager
{
public :
	StateManager() : wait_time_(0) {}

	/*!
	 * @brief 状態を確認する
	 *
	 * @param df 時間差分を指定
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
	 * @brief 状態を切り替える
	 *
	 * @param state 次の状態を指定する
	 * @param immediate 即時切替の有無を指定する
	 */
	void Change(State state, bool immediate = false) {
		if (immediate) {
			state_ = state;
		} else {
			next_state_ = state;
		}
	}

	/*!
	 * @brief 状態を切り替える
	 *
	 * @param state 次の状態を指定する
	 * @param time 次に切り替わるまでの待ち時間を指定する
	 */
	void ChangeDelay(State state, float time) {
		next_state_ = state;
		wait_time_ = time;
	}

	operator State() const { return state_; }
	State operator=(State state){ state_ = state; return state; }
	State Get() const{ return state_; }

private :
	State state_;
	State next_state_;
	float wait_time_;
};
