#pragma once
#include <string>
#include "Thread.h"
#include "BasicConnection.h"

using namespace std;
class AppToP2Task : public CTask
{
public:
	AppToP2Task(BasicConnection* clientConnection, char* buffer, int from_app_len, int client_socket);
	int Run();
	~AppToP2Task();

private:
	BasicConnection* clientConnection;
	char* buffer;
	int from_app_len;
	int client_socket;
	char* constructRedisReturn(string str);
};

