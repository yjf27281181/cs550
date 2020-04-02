#pragma once

#include "Thread.h"
class AppToP2Server : public CTask
{
public:
	AppToP2Server();
	int Run();
	~AppToP2Server();
};

