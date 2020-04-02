//#include <unistd.h>
//#include "BasicConnection.h"
//#include <stdio.h>
//#include <sys/socket.h>
//#include <stdlib.h>
//#include <netinet/in.h>
//#include <string.h>
//#include <arpa/inet.h>
#include "AppToP2Server.h";
#include "P2RedirectServer.h";
#include "Thread.h";
#include <stdio.h>

int main(int argc, char const *argv[]) {
	CThreadPool Pool(5);
	CTask* appToP2Server = new AppToP2Server();
	Pool.AddTask(appToP2Server);
	CTask* p2RedirectServer = new P2RedirectServer();
	Pool.AddTask(p2RedirectServer);
	getchar();
}