#pragma once
#include <iostream>
#include <memory>

class Sock;

// �N���X�̐���
void Create(void);

// �ڑ�
void Accept(void);

// ��M
void Recv(void);

// ���M
void Send(void);

std::shared_ptr<Sock>sock;
