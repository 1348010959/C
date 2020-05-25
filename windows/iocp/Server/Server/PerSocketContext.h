#pragma once
#include "stdafx.h"


#define MAX_BUFFER_LEN (1024*8)

//在完成端口上投递的I/O操作类型，和I/O复用里监听什么事件类型差不多
enum class PostType
{
	UNKNOW,		//初始化状态
	ACCEPT,		//投递accept操作
	SEND,		//投递send操作
	RECV,		//投递recv操作
};

struct IoContext
{
	OVERLAPPED   m_Overlapped;            //重叠数据结构，感觉和复用差不多的对象，这玩意要放第一位，切记
	PostType     m_post_type;
	SOCKET       m_accept_socket;         //提前准备的socket
	WSABUF       m_WSABuf;
	char         m_buff[MAX_BUFFER_LEN];
	DWORD        m_total_bytes;           //总字节数
	DWORD        m_sent_bytes;            //已发送字节数

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

//单句柄数据结构
//和服务器里session差不多的感觉，保存socket的相关状态
//用于传递给GetQueuedCompletionStatus的参数之一
struct SocketContext
{
	SOCKET                            m_socket;			//客户端socket
	SOCKADDR_IN                       m_client_addr;    //客户端地址

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
