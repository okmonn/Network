#ifndef DIRECTX_DEVICE
#define DIRECTX_DEVICE

#include "BaseClass.h"

struct Device : 
	public BaseClass<ID3D12Device6> {
public:
	/*�R���X�g���N�^*/
	Device();

private:
	/*�f�o�C�X�̐���*/
	void CreateDevice(void);
};

#endif
