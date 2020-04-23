#include "AppToP2Server.h"
#include "BasicConnection.h"


AppToP2Server::AppToP2Server()
{
	this->Pool = new CThreadPool(10);
}

int AppToP2Server::Run()
{
	BasicConnection * clientConnection = new BasicConnection(8080);
	clientConnection->bindAndListen();
	while (true) {
		int client_socket = clientConnection->acceptContent();
		bool isRnicBusy = true;
		Pool->AddTask(new AppToP2Task(clientConnection, client_socket));
	}
	return 0;
}


AppToP2Server::~AppToP2Server()
{
}
