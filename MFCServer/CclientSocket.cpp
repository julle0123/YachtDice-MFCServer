// CclientSocket.cpp: 구현 파일
//

#include "pch.h"
#include "MFCServer.h"
#include "CclientSocket.h"


// CclientSocket

CclientSocket::CclientSocket()
{
}

CclientSocket::~CclientSocket()
{
}
void CclientSocket::SetWnd(HWND hWnd)
{
	m_hWnd = hWnd;
}
void CclientSocket::SetPlayerName(const CString& playerName)
{
	m_playerName = playerName;
}
CString CclientSocket::GetPlayerName() 
{
	return m_playerName;
}


void CclientSocket::OnReceive(int nErrorCode)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	CString strTmp = _T(""), strIPAddress = _T("");
	UINT uPortNumber = 0;
	TCHAR strBuffer[1024];
	CString keybuffer = _T("");
	ZeroMemory(strBuffer, sizeof(strBuffer));
	GetPeerName(strIPAddress, uPortNumber);
	keybuffer.Format(_T("%s"), strBuffer);
	if (Receive(strBuffer, sizeof(strBuffer)) > 0) { // 전달된 데이터(문자열)가 있을 경우
		keybuffer.Format(_T("%s"), strBuffer);
		if (keybuffer.Find(L"C|") + 1) {
			strTmp.Format(_T("C|[%s]: %s"), m_playerName, keybuffer.Mid(2));

		}
		else {
			if (m_playerName.Find(L"1")+1) {
				strTmp.Format(_T("%s%s"), L"P1|", strBuffer);
			}else{ strTmp.Format(_T("%s%s"), L"P2|", strBuffer);}
		}
	}


	SendMessage(m_hWnd, WM_CLEINT_MSG_RECV, 0, (LPARAM)((LPCTSTR)strTmp));
	CSocket::OnReceive(nErrorCode);
}

void CclientSocket::OnClose(int nErrorCode)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	m_pThread->PostThreadMessage(WM_QUIT, 0, 0);

	SendMessage(m_hWnd, WM_CLIENT_CLOSE, 0, (LPARAM)this);

	CSocket::OnClose(nErrorCode);
}

// CclientSocket 멤버 함수
