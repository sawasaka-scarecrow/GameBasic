#include "Counter.h"

#if		defined(WIN32)
typedef	signed __int64 INT64;
#elif	defined(PLATFORM_PSP)
typedef	signed long long INT64;
#elif	defined(PLATFORM_PS3)
typedef	signed long long INT64;
#endif

RootCounter::RootCounter() {
	set(0 , 0x7fffffff , 1);
	reset();
}

RootCounter::RootCounter(int end) {
	set(0 , end , 1);
	reset();
}

RootCounter::RootCounter(int start ,int end , int step) {
	set(start , end , step);
	reset();
}

void RootCounter::set(int start , int end , int step) {
	_start = ToInt(start);
	_end = _size = ToInt(end);
	_step = ToInt(step);
	reset();
}

void RootCounter::setSize(int size)
{
	_size = ToInt(size);
	if ( _size > _end ) _size = _end;
	_offset = _rootCount;
	if ( _offset > (_end - _size) ) _offset = _end - _size;
}

void RootCounter::reset() {
	_rootCount = _start;
	_rate = 0;
	_offset = 0;
}

void RootCounter::_inc(bool bAdd) {
	bool bInc = (_start > _end) ^ bAdd; // �t�����J�E���^�H
	if (bInc) {
	//	�C���N�������g
		if (_step > 0) {
		//	�����C���N�������g
			_rootCount += _step;
		} else {
		//	�����C���N�������g
			_rate++; if (_rate >= -_step) { _rate = 0; _rootCount++; }
		}
		//	�T�`�����[�g�����̂��H
		int max = _start < _end ? _end : _start;
		if (_rootCount >= max) {
			int min = _start < _end ? _start : _end;
			_rootCount = min;
			//		CRootCounter�Ƃ͂��������Ⴄ����
		}
		// �I�t�Z�b�g
		if ( (_rootCount-_offset) >= _size ) _offset++;
		if ( _rootCount == 0 ) _offset = 0;
	} else {
	//	�f�N�������g
		if (_step > 0) {
		//	�����f�N�������g
			_rootCount -= _step;
		} else {
		//	�����f�N�������g
			_rate--; if (_rate <= _step) { _rate = 0; _rootCount--; }
			//	�ˁ@_rate++�łȂ����Ƃɒ��ӁB
			//	++�̂���--���āA���̐��������Ƃ�Ȃ��Ă͂Ȃ�Ȃ�
			//	���AnStep<0�̂Ƃ��ŏ��̂P��ڂ�--��RootCounter��
			//	1�����Ă͂����Ȃ��B����Ă������������ɂȂ�
		}
		//	�T�`�����[�g�����̂��H
		int min = _start < _end ? _start : _end;
		if (_rootCount < min) {
			int max = _start < _end ? _end : _start;
			if (_step > 0) {
				_rootCount = max - _step;
			} else {
				_rootCount = max - 1;
			}
			//		CRootCounter�Ƃ͂��������Ⴄ����
		}
		// �I�t�Z�b�g
		if ( (_rootCount-_offset) < 0 ) _offset--;
		if ( _rootCount == (_end-1) ) _offset = _end - _size;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

SaturationCounter::SaturationCounter() {
	set(0 , 0x7fffffff , 1);
	reset();
}

SaturationCounter::SaturationCounter(int end) {
	set(0 , end , 1);
	reset();
}

SaturationCounter::SaturationCounter(int start ,int end , int step) {
	set(start , end , step);
	reset();
}

void SaturationCounter::set(int start , int end , int step) {
	_start = ToInt(start);
	_end = ToInt(end);
	_step = ToInt(step);
	reset();
}

void SaturationCounter::reset() {
	_rootCount = _start;
	_rate = 0;
}

void SaturationCounter::_inc(bool bAdd) {
	bool bInc = (_start > _end) ^ bAdd; // �t�����J�E���^�H
	if (bInc) {
	//	�C���N�������g
		if (_step > 0) {
		//	�����C���N�������g
			_rootCount += _step;
		} else {
		//	�����C���N�������g
			_rate++; if (_rate >= -_step) { _rate = 0; _rootCount++; }
		}
		//	�T�`�����[�g�����̂��H
		int max = _start < _end ? _end : _start;
		if (_rootCount > max) _rootCount = max;
			//		CRootCounter�Ƃ͂��������Ⴄ����
	} else {
	//	�f�N�������g
		if (_step > 0) {
		//	�����f�N�������g
			_rootCount -= _step;
		} else {
		//	�����f�N�������g
			_rate--; if (_rate <= _step) { _rate = 0; _rootCount--; }
			//	�ˁ@_rate++�łȂ����Ƃɒ��ӁB
			//	++�̂���--���āA���̐��������Ƃ�Ȃ��Ă͂Ȃ�Ȃ�
			//	���AnStep<0�̂Ƃ��ŏ��̂P��ڂ�--��RootCounter��
			//	1�����Ă͂����Ȃ��B����Ă������������ɂȂ�
		}
		//	�T�`�����[�g�����̂��H
		int min = _start < _end ? _start : _end;
		if (_rootCount < min) _rootCount = min;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

InteriorCounter::InteriorCounter() {
	_start		= 0;
	_end		= 0;
	_now		= 0;
	_frames		= 0;
	_framesNow	= 0;
}

void InteriorCounter::set(int start , int end , int frames) {
//	WARNING(nFrames == 0,"CInteriorCounter::Set��nFrames == 0");
	// �[�����Z���������
	if (frames == 0) frames = 1;
	
	_start		= ToInt(start);
	_end		= ToInt(end);
	_now		= ToInt(start);
	_frames		= ToInt(frames);
	_framesNow	= 0;
}

void InteriorCounter::inc() {
	//	�J�E���^�͏I���l���H
	if (_framesNow >= _frames) {
		_now = _end;
		return;
	}
	_framesNow++;
	//	��������
	_now =  _start + static_cast<int>(static_cast<INT64>(_framesNow) * (_end - _start) / _frames);
}

void InteriorCounter::dec() {
	//	�J�E���^�͏����l���H
	if (_framesNow == 0) return;
	_framesNow--;
	//	��������
	_now =  _start + static_cast<int>(static_cast<INT64>(_framesNow) * (_end - _start) / _frames);
}
