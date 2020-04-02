#include "P2RedirectServer.h"
#include "BasicConnection.h"


P2RedirectServer::P2RedirectServer()
{
}

int P2RedirectServer::Run()
{
	char buffer[1024] = { 0 }, from_redis[1024] = { 0 };
	BasicConnection * clientConnection = new BasicConnection(8081);
	clientConnection->bindAndListen();
	BasicConnection * redisConnection = new BasicConnection("192.168.0.200", 7000);
	redisConnection->connectToServer();

	while (true) {

		int client_socket = clientConnection->acceptContent();
		int from_app_len = read(client_socket, buffer, 1024);

		int len_from_redis = redisConnection->sendMsgToServer(buffer, from_app_len, from_redis, -1);
		clientConnection->sendMsgToServer(from_redis, len_from_redis, buffer, client_socket);

	}
	return 0;
}


P2RedirectServer::~P2RedirectServer()
{
}
