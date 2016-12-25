/*!
 * @file
 *
 * @brief �x�W�F���[�e�B���e�B��񋟂��܂��B
 *
 * @copyright
 */
#pragma once

namespace Bezier
{
	struct ControlPoint
	{
		float x1, y1, x2, y2;
		ControlPoint() : x1(0), y1(0), x2(0), y2(0){}
		ControlPoint(float x1, float y1, float x2, float y2) {
			this->x1 = x1;
			this->y1 = y1;
			this->x2 = x2;
			this->y2 = y2;
		}
	};

	// �x�W�F�̃e���v���[�g(�Q�l�Fhttp://matthewlein.com/ceaser/)
	static const ControlPoint Linear		= ControlPoint(0.250f,  0.250f, 0.750f, 0.750f);	// ����
	static const ControlPoint EaseIn		= ControlPoint(0.750f,  0.000f, 0.750f, 0.900f);	// �������x���A�I��肪����
	static const ControlPoint EaseOut		= ControlPoint(0.250f,  0.100f, 0.250f, 1.000f);	// �����������A�I��肪�x��
	static const ControlPoint EaseInOut		= ControlPoint(0.420f,  0.000f, 0.580f, 1.000f);	// �����ƏI��肪�x���A���Ԃ�����
	static const ControlPoint EaseInBack	= ControlPoint(0.600f, -0.280f, 0.735f, 0.045f);	// �������x���o�b�N���A�I��肪����
	static const ControlPoint EaseOutBack	= ControlPoint(0.175f,  0.885f, 0.320f, 1.275f);	// �����������A�I��肪�x���I�[�o�[���Ė߂�
	static const ControlPoint EaseInOutBack	= ControlPoint(0.680f, -0.550f, 0.265f, 1.550f);	// �����ƏI��肪�x���A���Ԃ������B�ŏ��Ƀo�b�N���A�I���ŃI�[�o�[���Ė߂�
	static const ControlPoint EaseInCirc	= ControlPoint(0.600f,  0.040f, 0.980f, 0.335f);	// ���������Ȃ�x���A�I��肪���Ȃ葬��
	/*!
	 * @brief �x�W�F�̌v�Z���s���܂��B
	 */
	float Calc(float x, float x1, float y1, float x2, float y2);

	/*!
	 * @brief �x�W�F�J�E���^�[�N���X��񋟂��܂��B
	 */
	template<class T>
	class Counter
	{
	public :
		Counter()
			: now_(0)
			, start_(0)
			, end_(0)
			, frames_(0)
			, frames_now_(0)
			, return_(false)
		{}
		Counter(T start, T end, int frame, const ControlPoint& point){
			Set(start, end, frame, point);
		}

		void Set(T start, T end, int frame, const ControlPoint& point, bool re = false) {
			now_ = start;
			start_ = start;
			end_ = end;
			frames_ = frame;
			frames_now_ = 0;
			point_ = point;
			return_ = re;
		}
		void SetReverse(const ControlPoint& point){
			T temp = start_;
			now_ = end_;
			start_ = end_;
			end_ = temp;
			frames_now_ = 0;
			point_ = point;
		}
		void SetReverse(){
			T temp = start_;
			start_ = end_;
			end_ = temp;
			now_ = start_;
			frames_now_ = 0;
		}

		void SetFrameNow(int frame) {
			frames_now_ = frame;
			if (frames_now_ < 0) frames_now_ = 0;
			if (frames_now_ > frames_) frames_ = frames_;
			if (frames_now_ == 0) {
				now_ = start_;
				return;
			}
			if (frames_now_ >= frames_) {
				now_ = end_;
				return;
			}

			//	��������
			now_ =  static_cast<T>(start_ + (end_ - start_) * Calc(static_cast<float>(frames_now_) / frames_, point_.x1, point_.y1, point_.x2, point_.y2));
		}

		bool IsBegin() const { return now_ == start_; }
		bool IsEnd() const { return now_ == end_ || frames_now_ == frames_; }

		operator T() const{ return now_; }
		T operator=(T n){ now_ = n; return n; }
		T Get() const { return now_; }

		T GetStart() const { return start_; }
		T GetEnd() const { return end_; }
		int GetFrame() const { return frames_; }
		int GetFrameNow() const { return frames_now_; }

		// ���Z
		bool Inc() {
			if (++frames_now_ >= frames_) {
				now_ = end_;
				frames_now_ = frames_;
				if(return_){
					return true;
				}
				return false;
			}

			//	��������
			now_ =  static_cast<T>(start_ + (end_ - start_) * Calc(static_cast<float>(frames_now_) / frames_, point_.x1, point_.y1, point_.x2, point_.y2));
			return false;
		}

		Counter& operator++()
			{ Inc(); return (*this); }
		Counter operator++(int)
			{ Counter tmp = *this; Inc(); return tmp; }

		// ���Z
		void Dec() {
			if (--frames_now_ <= 0) {
				now_ = start_;
				frames_now_ = 0;
				return;
			}

			//	��������
			now_ =  static_cast<T>(start_ + (end_ - start_) * Calc(static_cast<float>(frames_now_) / frames_, point_.x1, point_.y1, point_.x2, point_.y2));
		}

		Counter& operator--()
			{ Dec(); return (*this); }
		Counter operator--(int)
			{ Counter tmp = *this; Dec(); return tmp; }

	private :
		T now_;
		T start_;
		T end_;
		int frames_;
		int frames_now_;
		bool return_;
		ControlPoint point_;
	};

	/*!
	 * @brief �x�W�F�^�C�}�[�N���X��񋟂��܂��B
	 */
	template<class T>
	class Timer
	{
	public :
		Timer()
			: now_()
			, start_()
			, end_()
			, time_start_()
			, times_()
			, times_now_()
		{}

		void Set(T start, T end, unsigned __int64 time, const ControlPoint& point) {
			now_ = start;
			start_ = start;
			end_ = end;
			time_start_ = 0;
			times_now_ = 0;
			times_ = time;
			point_ = point;
			if (time == 0) {
				now_ = end;
			}
		}
		void Set(T start, T end, float time, const ControlPoint& point) {
			Set(start, end, static_cast<unsigned __int64>(time * 1000), point);
		}
		void Update(int time) {
			times_now_ += time;
			if (times_now_ >= times_) {
				now_ = end_;
				times_now_ = times_;
				return ;
			}

			now_ =  static_cast<T>(start_ + (end_ - start_) * Calc(static_cast<float>(times_now_) / times_, point_.x1, point_.y1, point_.x2, point_.y2));
		}
		void Update(float time) {
			Update(static_cast<int>(time * 1000));
		}

		bool IsEnd() const { return now_ == end_; }

		operator T() const{ return now_; }
		T operator=(T n){ Set(n, n, 0.0f, Linear); return	n; }
		T operator+=(T n){ Set(now_ + n, now_ + n, 0.0f, Linear); return	n; }
		T operator-=(T n){ Set(now_ - n, now_ - n, 0.0f, Linear); return	n; }
		T operator*=(T n){ Set(now_ * n, now_ * n, 0.0f, Linear); return	n; }
		T operator/=(T n){ Set(now_ / n, now_ / n, 0.0f, Linear); return	n; }
		T Get() const { return now_; }

		T GetStart() const { return start_; }
		T GetEnd() const { return end_; }

		unsigned int GetPassTime() const { return static_cast<unsigned int>(times_now_); }
	private :
		T now_;
		T start_;
		T end_;
		unsigned __int64 time_start_;
		unsigned __int64 times_;
		unsigned __int64 times_now_;
		ControlPoint point_;
	};
}
