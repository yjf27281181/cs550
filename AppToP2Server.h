#pragma once

#include "Thread.h"
#include "AppToP2Task.h"
class AppToP2Server : public CTask
{
public:
	AppToP2Server();
	int Run();
	~AppToP2Server();
private:
	CThreadPool* Pool;
};

