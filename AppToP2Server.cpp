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
		char buffer[1024] = { 0 };
		int client_socket = clientConnection->acceptContent();
		int from_app_len = read(client_socket, buffer, 1024);

		// network is busy
		printf("%d - %s\n", from_app_len, buffer);
		bool isRnicBusy = true;
		Pool->AddTask(new AppToP2Task(clientConnection, buffer, from_app_len, client_socket));
	}
	return 0;
}


AppToP2Server::~AppToP2Server()
{
}
