#include "AppToP2Task.h"



AppToP2Task::AppToP2Task(BasicConnection* clientConnection, char* buffer, int from_app_len, int client_socket)
{
	this->clientConnection = clientConnection;
	int bufferLen = 1024;
	this->buffer = new char[bufferLen];
	for (int i = 0; i < bufferLen; i++) {
		this->buffer[i] = buffer[i];
	}
	this->from_app_len = from_app_len;
	this->client_socket = client_socket;
}

int AppToP2Task::Run()
{
	printf("in apptop2task run\n");
	char from_redis[1024] = { 0 };
	/*send back a redirect msg*/
	BasicConnection * redisConnection = new BasicConnection("192.168.0.200", 7000);
	redisConnection->connectToServer();
	printf("buffer content: %s", buffer);
	int len_from_redis = redisConnection->sendMsgToServer(buffer, from_app_len, from_redis, -1);

	for (int i = 0; i < len_from_redis; i++) {
		printf("%d : %d \n", i, from_redis[i]);
	}
	char* test = constructRedisReturn("redirect#111#2222222");
	int test_len = strlen(test);
	for (int i = 0; i < test_len; i++) {
		printf("%d : %d \n", i, test[i]);
	}
	clientConnection->sendMsgToServer(test, test_len, buffer, client_socket);

	return 0;
}


AppToP2Task::~AppToP2Task()
{
}

char * AppToP2Task::constructRedisReturn(string str)
{
	string res = "";
	res.push_back((char)36);
	res.append(to_string(str.length()));
	res.push_back((char)13); //carriage return 
	res.push_back((char)10); //new line
	res.append(str); 
	res.push_back((char)13); //carriage return 
	res.push_back((char)10); //carriage return 
	printf("--------------!!!%s %d", (char*)res.data());
	return (char*)res.data();
}
