#pragma once

#include "Thread.h"
class P2RedirectServer : public CTask
{
public:
	P2RedirectServer();
	int Run();
	~P2RedirectServer();
};