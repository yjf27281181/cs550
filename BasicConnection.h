#pragma once
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <iostream>
class BasicConnection
{
public:
	BasicConnection(int port);
	//����p2 - p2���Ͷ� ���� p2 �� redis�ķ��Ͷ�
	BasicConnection(int port, int ip);
	//����p2 - p2���ܶ� ���߽���p2��application�Ľ��ն�
	BasicConnection(char* ip, int port);

	//�󶨽ӿڲ����м��� 
	void bindAndListen();
	//׼��������Ϣ
	int acceptContent(); //return client_socket

	//ͨ��ip����Զ��server��p2����reids��
	void connectToServer();
	//����һ�˷������ݣ�socket == -1 ��ʾ��p2��reids��������socket��ֵ��ʾp2���ѽ������ӵ�socket��������
	int sendMsgToServer(char* buffer, int buffer_len, char* buffer_from_server, int socket); //return length of msg
	~BasicConnection();

private:
	struct sockaddr_in address;
	int opt = 1;
	int addrlen;
	int server_fd;
	char* ip;
};

