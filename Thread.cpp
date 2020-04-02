/******************
** Fliename: Thread.cpp
** Dscribe: �̳߳�ʵ���ļ�
** Date: 2018.7.18
** @author: xionglei
***/


#include "Thread.h"  
#include <iostream>  
#include <stdio.h> 
#include <deque>


void CTask::SetConnFd(int data)
{
	connfd = data;
}

int CTask::GetConnFd()
{
	return connfd;
}
/**
* ��ʼ������
*/
deque<CTask*> CThreadPool::m_deqTaskList;         //�����б�  
bool CThreadPool::shutdown = false;

pthread_mutex_t CThreadPool::m_pthreadMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t CThreadPool::m_pthreadCond = PTHREAD_COND_INITIALIZER;

/**
 * �̳߳ع����๹�캯��
 */
CThreadPool::CThreadPool(int threadNum)
{
	this->m_iThreadNum = threadNum;
	cout << "I will create " << threadNum << " threads" << endl;
	Create();       //*��������ʱ�㴴���̡߳�
}


/**
 * �̻߳ص�����
 */
void* CThreadPool::ThreadFunc(void* threadData)
{
	pthread_t tid = pthread_self();
	while (1)
	{

		//* �߳̿���ʱ������ */
		pthread_mutex_lock(&m_pthreadMutex);
		while (m_deqTaskList.size() == 0 && !shutdown)
		{
			//* û������ʱ���̵߳ȴ�״̬������������*/
			pthread_cond_wait(&m_pthreadCond, &m_pthreadMutex);
		}

		if (shutdown)
		{
			pthread_mutex_unlock(&m_pthreadMutex);
			printf("thread %lu will exit\n", pthread_self());
			pthread_exit(NULL);
		}

		printf("tid %lu run\n", tid);

		/**
		* ȡ������в�����֮
		*/

		//deque<CTask*>::iterator iter = m_deqTaskList.front();
		CTask* task = m_deqTaskList.front();
		m_deqTaskList.pop_front();

		//* ȡ��������ͷ���*/
		pthread_mutex_unlock(&m_pthreadMutex);

		task->Run(); /** ִ������ */

	}
	return (void*)0;
}





/**
 * ������������������񲢷����߳�ͬ���ź�
 */
int CThreadPool::AddTask(CTask *task)
{
	pthread_mutex_lock(&m_pthreadMutex);
	this->m_deqTaskList.push_back(task);
	pthread_mutex_unlock(&m_pthreadMutex);

	// * ������� �����������źţ�������  */
	pthread_cond_signal(&m_pthreadCond);
	return 0;
}

/**
 * �����߳�
 */
int CThreadPool::Create()
{
	pthread_id = (pthread_t*)malloc(sizeof(pthread_t) * m_iThreadNum);
	for (int i = 0; i < m_iThreadNum; i++)
	{
		pthread_create(&pthread_id[i], NULL, ThreadFunc, NULL);
	}
	return 0;
}





/**
 * ֹͣ�����߳�
 */
int CThreadPool::StopAll()
{
	/** �����ظ����� */
	if (shutdown)
	{
		return -1;
	}
	printf("Now I will end all threads!!\n");
	/** �������еȴ��߳� */
	shutdown = true;
	pthread_cond_broadcast(&m_pthreadCond);

	/** �����ȴ��߳��˳� */
	for (int i = 0; i < m_iThreadNum; i++)
	{
		pthread_join(pthread_id[i], NULL);
	}

	free(pthread_id);
	pthread_id = NULL;

	/** �������������ͻ����� */
	pthread_mutex_destroy(&m_pthreadMutex);
	pthread_cond_destroy(&m_pthreadCond);

	return 0;
}




/**
 * ��ȡ��ǰ������������
 */
int CThreadPool::getTaskSize()
{
	return m_deqTaskList.size();
}






