#pragma once



// CSocketThread
#include "CclientSocket.h"  //ClientSocket 헤더 추가

#define WM_ACCEPT_SOCKET WM_USER+1  //ServerSocket에 있던 메시지 쓰레드로 이동
class CSocketThread : public CWinThread
{
	DECLARE_DYNCREATE(CSocketThread)
public:
	CclientSocket m_ClientSocket;
	SOCKET m_hSocket;

	HWND m_hWnd;
	void SetWnd(HWND hWnd);
protected:
	CSocketThread();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CSocketThread();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
};


