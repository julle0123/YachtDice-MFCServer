#pragma once

// CclientSocket 명령 대상
#define MAX_CLIENT_COUNT 2 
#define WM_CLEINT_MSG_RECV WM_USER+2
#define WM_CLIENT_CLOSE WM_USER+3
class CclientSocket : public CSocket
{
public:
	CclientSocket();
	virtual ~CclientSocket();
	//Thread
	CWinThread* m_pThread;
	void SetWnd(HWND hWnd);
	HWND m_hWnd;
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	void SetPlayerName(const CString& playerName);
	CString GetPlayerName();

private:
	CString m_playerName;
};

