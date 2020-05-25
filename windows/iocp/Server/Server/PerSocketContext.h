#pragma once
#include "stdafx.h"


#define MAX_BUFFER_LEN (1024*8)

//����ɶ˿���Ͷ�ݵ�I/O�������ͣ���I/O���������ʲô�¼����Ͳ��
enum class PostType
{
	UNKNOW,		//��ʼ��״̬
	ACCEPT,		//Ͷ��accept����
	SEND,		//Ͷ��send����
	RECV,		//Ͷ��recv����
};

struct IoContext
{
	OVERLAPPED   m_Overlapped;            //�ص����ݽṹ���о��͸��ò��Ķ���������Ҫ�ŵ�һλ���м�
	PostType     m_post_type;
	SOCKET       m_accept_socket;         //��ǰ׼����socket
	WSABUF       m_WSABuf;
	char         m_buff[MAX_BUFFER_LEN];
	DWORD        m_total_bytes;           //���ֽ���
	DWORD        m_sent_bytes;            //�ѷ����ֽ���

	IoContext()
	{
		m_post_type = PostType::UNKNOW;
		ZeroMemory(&m_Overlapped, sizeof(m_Overlapped));
		ZeroMemory(&m_buff, MAX_BUFFER_LEN);
		m_accept_socket = INVALID_SOCKET;
		m_WSABuf.len = MAX_BUFFER_LEN;
		m_WSABuf.buf = m_buff;
		m_total_bytes = 0;
		m_sent_bytes = 0;
	}

	~IoContext()
	{
		if (m_accept_socket != INVALID_SOCKET)
			m_accept_socket = INVALID_SOCKET;

		//closesocket(m_accept_socket);

		cout << "one socket close" << endl;
	}

	void ResetBuffer()
	{
		ZeroMemory(m_buff, MAX_BUFFER_LEN);
		m_WSABuf.len = MAX_BUFFER_LEN;
		m_WSABuf.buf = m_buff;
	}
};

//��������ݽṹ
//�ͷ�������session���ĸо�������socket�����״̬
//���ڴ��ݸ�GetQueuedCompletionStatus�Ĳ���֮һ
struct SocketContext
{
	SOCKET                            m_socket;			//�ͻ���socket
	SOCKADDR_IN                       m_client_addr;    //�ͻ��˵�ַ

	set<IoContext*>                   m_arrayIoContext;

	SocketContext()
	{
		m_socket = INVALID_SOCKET;
		memset(&m_client_addr, 0, sizeof(m_client_addr));
	}

	~SocketContext()
	{
		if (m_socket != INVALID_SOCKET)
		{
			closesocket(m_socket);
			m_socket = INVALID_SOCKET;
		}

		auto it = m_arrayIoContext.begin();
		while (it != m_arrayIoContext.end())
		{
			delete *it;
			++it;
		}

		if (m_arrayIoContext.size() > 0)
			cout << "error size" << endl;

		m_arrayIoContext.clear();
	}

	IoContext* GetNewIoContext()
	{
		IoContext* sp = new IoContext;
		m_arrayIoContext.insert(sp);

		return sp;
	}

	void RemoveContext(IoContext* spContext)
	{
		if (spContext == nullptr)
			return;

		auto it = m_arrayIoContext.find(spContext);
		if (it != m_arrayIoContext.end())
		{
			delete* it;
			spContext = nullptr;
			m_arrayIoContext.erase(it);
		}
	}
};
