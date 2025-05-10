#pragma once

// CServerSocket 명령 대상
#include "CclientSocket.h"
#include <list>
#define WM_ACCEPT_SOCKET WM_USER+1
class CServerSocket : public CSocket
{
public:
	CServerSocket();
	virtual ~CServerSocket();
	void SetWnd(HWND hWnd);
	HWND m_hWnd;
	int m_MaxClients;       // 허용할 최대 클라이언트 수
	int m_CurrentClients;   // 현재 연결된 클라이언트 수
	void SetMaxClients(int maxClients);  // 최대 클라이언트 수를 설정하는 함수
	virtual void OnAccept(int nErrorCode);
	void downclients();
};


