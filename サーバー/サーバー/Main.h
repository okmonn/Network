#pragma once
#include <iostream>
#include <memory>

class Sock;

// クラスの生成
void Create(void);

// 接続
void Accept(void);

// 受信
void Recv(void);

// 送信
void Send(void);

std::shared_ptr<Sock>sock;
