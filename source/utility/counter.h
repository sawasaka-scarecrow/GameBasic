/*!
 * @file
 * 
 * @brief �J�E���^�[�N���X
 * 
 * yaneurao �����쐬���� CRootCounter �������ς��܂���
 * 
 * @author yaneurao
 * 
 * @since 2006/02/03
 */
#pragma once

__inline int ToInt(int n){
#if 0
	if (n > 32767){
		return 32767;
	}else if (n < -32768){
		return -32768;
	}
#endif
	return n;
}

/**
 * @brief ���[�v�J�E���^
 * 
 * �����l(_start)�E�I���l(_end)�A�����ĕK�v�Ȃ�Α����l(_step)��ݒ肵�܂��B<br>
 * ���̂��ƁAinc�֐������s����ƁA1�������܂��B(_step���ݒ肳��Ă���΁A
 * ���̐����������܂�)�@�����āA���Z�������ʁA_end�ȏ�ɂȂ�Ǝ����I��
 * _start�ɖ߂�܂��B(_end�ɂ͂Ȃ�܂���)<br>
 * <br>
 * reset�֐��́A�J�E���^�l�������l_start�ɖ߂��܂��B<br>
 * ���邢�́AsetInit�֐���reset��̏����l���ݒ肳��Ă���΁A���̒l�ɂ��܂��B<br>
 * <br>
 * �܂��Aint�^�Ƃ̑��ݕϊ����\�ł��̂ŁA��������int�^�̕ϐ��ł��邩�̂悤��
 * �g���܂��B<br>
 * <br>
 * ��j<br>
 * @code
 * RootCounter r;
 * r.set (0, 256 , -5);
 * // _start == 0 , _end == 256�Ƃ����悤�ɃJ�E���^��ݒ肷��
 * r = 128;
 * @endcode
 * 
 * ���̏�ԂŁAr++; ���T����s����ƂT��ڂ� r == 129�ɂȂ�B<br>
 * r--; ���T����s����ƂT��ڂ� r == 127�ɂȂ�B<br>
 * <br>
 * �܂��A_start��_end�łȂ��ėǂ��̂ł��B
 * �܂�A���Z(inc / ++)�ł́A_end�����փC���N�������g���܂��B
 * ���Z(dec / --)�ł́A_start�����փC���N�������g���܂��B
 * 
 * �܂��A_step���}�C�i�X�ł���ꍇ�́A
 * ���̐�Βl�񐔂�inc�����o�֐��̌Ăяo���ɂ���āA1����_end�����ɐi�݂܂��B
*/
class RootCounter{
public:
	//	step�͈��̑����̐�Βl�B�}�C�i�X��1/step�̈Ӗ�
	//	start��end�łȂ��ėǂ�
	void set(int start , int end , int step = 1);
	void setStep(int step){ _step = ToInt(step); }
	void setStart(int start){ _start = ToInt(start); }
	void setEnd(int end){ _end = ToInt(end); }
	void setOffset(int offset){ _offset = ToInt(offset); }
	void setSize(int size);
	
	///	�擾
	int getStep() const{ return _step; }
	int getStart() const{ return _start; }
	int getEnd() const{ return _end; }
	int getOffset() const{ return _offset; }
	int getPos() const{ return (_rootCount-_offset); }
	int getSize() const{ return _size; }
	
	///	�J�E���^�̃��Z�b�g
	void reset();
	
	//	property..
	bool isBegin() const{ return _rootCount == _start; }
	bool isEnd() const{ return _rootCount == _end; }
	
	RootCounter();
	RootCounter(int end);
	RootCounter(int start ,int end , int step = 1);
	
	//	int�Ƃ̑��ݕϊ�
	operator int() const{ return _rootCount; }
	int operator=(int n){ _rootCount = ToInt(n); return n; }
	int get() const{ return _rootCount; }
	
	//	�J�E���^�̃C���N�������g(�I�[�܂ŒB����ƁA�����Œ�~����)
	void inc(){ _inc(true); }
	void dec(){ _inc(false); }
	
	//	���Z�i��end�����փC���N�������g�j�^���Z�i��start�����ւ̃C���N�������g�j
	RootCounter& operator++()
		{ inc(); return (*this); }
	RootCounter operator++(int)
		{ RootCounter _tmp = *this; inc(); return _tmp; }
	RootCounter& operator--()
		{ dec(); return (*this); }
	RootCounter operator--(int)
		{ RootCounter _tmp = *this; dec(); return _tmp; }
protected:
	void	_inc(bool bAdd = true);
	
	int		_rootCount;
	int		_start;
	int		_end;
	int		_step;
	int		_rate;	//	step<0�̂Ƃ��́A�����inc()��+1�����
	int		_size;
	int		_offset;
};

/**
 * @brief �O�a�J�E���^
 * 
 * RootCounter �̃o���G�[�V�����B<br>
 * �u�J�E���^��++���Ă����A�I���l�ɂȂ����Ƃ��ɁA�����ŃJ�E���^�͒�~����v�Ƃ��������������قȂ�B
 */
class SaturationCounter{
public:
	//	step�͈��̑����̐�Βl�B�}�C�i�X��1/step�̈Ӗ�
	//	start��end�łȂ��ėǂ�
	void set(int start , int end , int step = 1);
	void setStep(int step){ _step = ToInt(step); }
	void setStart(int start){ _start = ToInt(start); }
	void setEnd(int end){ _end = ToInt(end); }
	
	///	�擾
	int getStep() const{ return _step; }
	int getStart() const{ return _start; }
	int getEnd() const{ return _end; }
	
	///	�J�E���^�̃��Z�b�g
	void reset();
	
	///	property..
	bool isBegin() const{ return _rootCount == _start; }
	bool isEnd() const{ return _rootCount == _end; }

	SaturationCounter();
	SaturationCounter(int end);
	SaturationCounter(int start ,int end , int step = 1);
	
	//	int�Ƃ̑��ݕϊ�
	operator int() const{ return _rootCount; }
	int operator=(int n){ _rootCount = ToInt(n); return n; }
	int get() const{ return _rootCount; }
	
	//	�J�E���^�̃C���N�������g(�I�[�܂ŒB����ƁA�����Œ�~����)
	void inc(){ _inc(true); }
	void dec(){ _inc(false); }
	
	//	���Z�i��end�����փC���N�������g�j�^���Z�i��start�����ւ̃C���N�������g�j
	SaturationCounter& operator++()
		{ inc(); return (*this); }
	SaturationCounter operator++(int)
		{ SaturationCounter _tmp = *this; inc(); return _tmp; }
	SaturationCounter& operator--()
		{ dec(); return (*this); }
	SaturationCounter operator--(int)
		{ SaturationCounter _tmp = *this; dec(); return _tmp; }
protected:
	void	_inc(bool bAdd = true);
	
	int		_rootCount;
	int		_start;
	int		_end;
	int		_step;
	int		_rate;	//	step<0�̂Ƃ��́A�����inc()��+1�����
};

/**
 * @brief �����J�E���^��񋟂��܂��B
 * 
 * set�����o�֐��ŁA�����l(start)�E�I���l(end)�A
 * �����ĕ�����(frames)��ݒ肵�܂��B<br>
 * <br>
 * ��������΁A���̃J�E���^��start����n�܂�A
 * frame���inc�����o�֐��̌Ăяo���ɂ����end�ɓ��B����悤��
 * �����J�E���^�ƂȂ�܂��B
 * 
 * �܂��Ainc�́Aoperator++�Ƃ��Ă���`���Ă���܂��B
 * inc�̋t����ł���dec������A�����operator--�Ƃ��Ă���`���Ă���܂��B
 */
class InteriorCounter{
public:
	InteriorCounter();
	
	///	int�Ƃ̑��ݕϊ�
	operator int () const{ return _now; }
	int operator=(int n){ _now = _start = ToInt(n); _framesNow = 0; return n; }
	
	void inc();		///	���Z
	InteriorCounter& operator++()
		{ inc(); return (*this); }
	InteriorCounter operator++(int)
		{ InteriorCounter _tmp = *this; inc(); return _tmp; }
	
	void dec();		///	���Z
	InteriorCounter& operator--()
		{ dec(); return (*this); }
	InteriorCounter operator--(int)
		{ InteriorCounter _tmp = *this; dec(); return _tmp; }
	
	int get() const{ return _now; }
	/**
	 * @brief �����l(@a start)�E�I���l(@a end)�A�����ĕ�����(@a frames)��ݒ肵�܂��B
	 */
	void set(int start , int end , int frames);
	
	//!	���݂̒l���i�ꎞ�I�Ɂj�ύX����B����inc/dec�ŁA����Ȓl�ɖ߂�
//	void set(int now){ *this = now; }
	void set(int now){ _now = ToInt(now); }
	
	void reset(){ set(getStart() , getEnd() , getFrame()); }

//	bool isBegin() const{ return _now == _start; }
//	bool isEnd() const{ return _now == _end; }
	bool isBegin() const{ return _framesNow == 0; }
	bool isEnd() const{
		return _framesNow == _frames;
	}

	//	property
	int		getFrame() const{ return _frames; }
	int		getStart() const{ return _start;}
	int		getEnd() const{ return _end;}
	
	//! ���݂̃t���[����(inc��������)�̎擾�B
	int		getFrameNow() const { return _framesNow; }
	
protected:
	int		_now;			//	���݂̒l
	int		_start;			//	�����l
	int		_end;			//	�I���l
	int		_frames;		//	�t���[���������i�I���l�ɂȂ�܂ŉ���inc������΂����̂��j
	int		_framesNow;		//	���݁A���t���[���ڂ��H
};

class LoopCounter{
public:
	void set(int start , int end , int step = 1){
		_counter.set(start , end , step);
		_bUp = true;
	}
	void reset(){
		if (isUp()){
			set(_counter.getStart() , _counter.getEnd() , _counter.getStep());
		}else{
			set(_counter.getEnd() , _counter.getStart() , _counter.getStep());
			_bUp = true;
		}
	}

	//	int�Ƃ̑��ݕϊ�
	operator int() const{ return get(); }
	int get() const{ return _counter.get(); }
	int operator=(int n){ return _counter.operator=(n); }
	
	///	�擾
	int getStep() const{ return _counter.getStep(); }
	int getStart() const{ return isUp() ? _counter.getStart() : _counter.getEnd(); }
	int getEnd() const{ return isUp() ? _counter.getEnd() : _counter.getStart(); }
	
	///	property..
	bool isBegin() const{ return isUp() ? _counter.isBegin() : _counter.isEnd(); }
	bool isEnd() const{ return isBegin(); }

	///	���Z
	void inc(){
		++_counter;
		if (_counter.isEnd()){
			int s = _counter.getStart();
			int e = _counter.getEnd();
			_counter.setStart(e);
			_counter.setEnd(s);
			_bUp = !_bUp;
		}
	}
	LoopCounter& operator++()
		{ inc(); return (*this); }
	LoopCounter operator++(int)
		{ LoopCounter _tmp = *this; inc(); return _tmp; }
	
	///	���Z
	void dec(){
		--_counter;
		if (_counter.isBegin()){
			int s = _counter.getStart();
			int e = _counter.getEnd();
			_counter.setStart(e);
			_counter.setEnd(s);
			_bUp = !_bUp;
		}
	}
	LoopCounter& operator--()
		{ dec(); return (*this); }
	LoopCounter operator--(int)
		{ LoopCounter _tmp = *this; dec(); return _tmp; }

	bool isUp() const{ return _bUp; }
protected:
	SaturationCounter	_counter;
	bool				_bUp;
};
