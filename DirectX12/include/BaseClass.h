#ifndef DIRECTX_BASE_CLASS
#define DIRECTX_BASE_CLASS

#include "ComPtr.h"

template <typename T>
struct BaseClass {
public:
	/*�R���X�g���N�^*/
	BaseClass() {}
	/*�f�X�g���N�^*/
	virtual ~BaseClass() {}

public:
	/*�I�u�W�F�N�g�̎擾
	return �I�u�W�F�N�g*/
	T* Get(void) const {
		return obj.Get();
	}
	/*�I�u�W�F�N�g�A�h���X�̎擾
	return �I�u�W�F�N�g�A�h���X*/
	T** GetAddress(void) {
		return &obj;
	}

private:
	/*�I�u�W�F�N�g*/
	ComPtr<T>obj;
};

#endif
