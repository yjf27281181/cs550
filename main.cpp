#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <thread>
#include <iostream>

using namespace std;
#define PORT 1234
#define QUEUE 20//�����������
int conn;
void thread_task()
{
}

int main()
{
	//printf("%d\n",AF_INET);//IPv4Э��
	printf("%d\n", SOCK_STREAM);//�ֽ����׽���
	int ss = socket(AF_INET, SOCK_STREAM, 0);//���ɹ��򷵻�һ��sockfd���׽�����������
	//printf("%d\n",ss);
	struct sockaddr_in server_sockaddr;//һ���Ǵ����ַ�Ͷ˿ڵġ�������Ϣ����ʾ���洢ʹ��
	/*���� sockaddr_in �ṹ������ز���*/
	server_sockaddr.sin_family = AF_INET;
	server_sockaddr.sin_port = htons(PORT);//��һ���޷��Ŷ�������ֵת��Ϊ�����ֽ��򣬼����ģʽ(big-endian)��
	//printf("%d\n",INADDR_ANY);
	//INADDR_ANY����ָ����ַΪ0.0.0.0�ĵ�ַ�������ַ��ʵ�ϱ�ʾ��ȷ����ַ�������е�ַ�����������ַ����
	//һ����˵���ڸ���ϵͳ�о������Ϊ0ֵ��
	server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);//���������޷��ų�������ת���������ֽ�˳�򡣡�
	if (bind(ss, (struct sockaddr*) &server_sockaddr, sizeof(server_sockaddr)) == -1)
	{
		perror("bind");
		exit(1);
	}
	if (listen(ss, QUEUE) == -1)
	{
		perror("listen");
		exit(1);
	}

	struct sockaddr_in client_addr;
	socklen_t length = sizeof(client_addr);
	///�ɹ����طǸ������֣�������-1
	conn = accept(ss, (struct sockaddr*)&client_addr, &length);
	//���accpet�ɹ�����ô�䷵��ֵ�����ں��Զ����ɵ�һ��ȫ���������������������ؿͻ���TCP���ӡ�
	//accpet֮��ͻ����µ��׽���conn
	if (conn < 0)
	{
		perror("connect");
		exit(1);
	}

	char buffer[1024];
	char msg[1024];
	int msgLen = 0;
	cout << msgLen << endl;
	//��������һ���߳�
	//std::thread t(thread_task);
	//t.join();
	//char buf[1024];
	//���߳�
	while (1)
	{
		//�����sendע�͵��ˣ��������������serverֻ���ǽ���client�˵����ݲ��ܸ�client�������ݣ���ʹ��ע�͵�Ҳû�ã���Ϊû�ж��Ƿ������ݴ���ʹ���
		//�����ж����԰�������Ĵ�������д��ÿ�ζ�Ҫ����server�����������client�����������ݣ�����server������������޷������߾�û��client���͹����������
		//����ÿ����ʾҲֻ����һ�У�������ʾ��ȫ�Ǵ���ˣ����Ծ���Ҫselect��FD_ISSET���ж���
		// memset(buf, 0 ,sizeof(buf));
		// if(fgets(buf, sizeof(buf),stdin) != NULL) {
		// send(conn, buf, sizeof(buf), 0); 
		// }

		memset(buffer, 0, sizeof(buffer));
		int len = recv(conn, buffer, sizeof(buffer), 0);//��TCP���ӵ���һ�˽������ݡ�
		/*�ú����ĵ�һ������ָ�����ն��׽�����������
		�ڶ�������ָ��һ�����������û������������recv�������յ������ݣ�
		����������ָ��buf�ĳ��ȣ�
		���ĸ�����һ����0*/
		if (strcmp(buffer, "exit\n") == 0)//���û���յ�TCP��һ�˷���������������ѭ�������
		{
			break;
		}
		
		string receivedMsg;
		if (len != 0) {
			
			
			printf("msg from java is:");//������յ��������������
			for (int i = 0; i < len; i++) {
				if (i < 2) continue;
				cout << buffer[i];
				receivedMsg += buffer[i];
			}
			cout << endl;
			cout << "receivedMsg: " << receivedMsg << endl;
			string big_data = "bigData";
			cout << big_data.length() << " length " << receivedMsg.length() << endl;
			if (receivedMsg == big_data) {
				strcpy(msg, "  1 [L1][address] [L2][datalength]");
			}
			else {
				strcpy(msg, "  0 databody");
			}
			msgLen = strlen(msg);
			msg[0] = buffer[0];
			msg[1] = msgLen - 2;
		}
		
		
		//����Ҫ�з������ݣ� ��������һ������������
		send(conn, msg, msgLen, 0);//��TCP���ӵ���һ�˷������ݡ�
	}
	close(conn);//��Ϊaccpet�������ӳɹ��󻹻�����һ���µ��׽�����������������Ҳ��Ҫ�ر�
	close(ss);//�ر�socket�׽���������
	return 0;
}