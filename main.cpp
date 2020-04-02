#include <unistd.h>
#include "BasicConnection.h"
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT 8080
int main(int argc, char const *argv[]) {

	char buffer[1024] = { 0 }, from_redis[1024] = { 0 };
	BasicConnection * clientConnection = new BasicConnection(8080);
	clientConnection->bindAndListen();
	BasicConnection * redisConnection = new BasicConnection("192.168.0.200",7000);
	redisConnection->connectToServer();

	while (true) {
		
		int client_socket = clientConnection->acceptContent();
		int from_app_len = read(client_socket, buffer, 1024);
		
		int len_from_redis = redisConnection->sendMsgToServer(buffer, from_app_len, from_redis, -1);
		clientConnection->sendMsgToServer(from_redis, len_from_redis, buffer, client_socket);

	}
	return 0;
}