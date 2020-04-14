#include "P2RedirectTask.h"
#include "BasicConnection.h"


P2RedirectTask::P2RedirectTask(BasicConnection* clientConnection, char* buffer, int from_app_len, int client_socket)
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

int P2RedirectTask::Run()
{
	//get data from rmda
	printf("in p2 redirect task: %s \n", this->buffer);
	string redirectCommand(buffer);
	string delimiter = "#";

	size_t pos = 0;
	string token;
	while ((pos = redirectCommand.find(delimiter)) != std::string::npos) {
		token = redirectCommand.substr(0, pos);
		cout << token << endl;
		redirectCommand.erase(0, pos + delimiter.length());
	}
	cout << redirectCommand << endl;
	return 0;
}


P2RedirectTask::~P2RedirectTask()
{
}
