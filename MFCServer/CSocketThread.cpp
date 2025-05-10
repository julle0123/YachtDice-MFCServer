// CSocketThread.cpp: 구현 파일
//

#include "pch.h"
#include "MFCServer.h"
#include "CSocketThread.h"


// CSocketThread

IMPLEMENT_DYNCREATE(CSocketThread, CWinThread)

CSocketThread::CSocketThread()//CSocketThread 클래스는 CWinThread를 상속받아 새로운 스레드를 만들기 위한 클래스
{
}

CSocketThread::~CSocketThread()
{
}

void CSocketThread::SetWnd(HWND hWnd)
{
	m_hWnd = hWnd;
}

BOOL CSocketThread::InitInstance()
{
	// TODO:  여기에서 각 스레드에 대한 초기화를 수행합니다.
	// 리슨 소켓으로부터 넘겨받은 소켓을 자신의 멤버 소켓에 붙인다.
	m_ClientSocket.Attach(m_hSocket); //attach : 데이터를 경로에 추가하여 번수명으로 바로 접근 할 수있게함
	m_ClientSocket.m_pThread = this; //m_ClientSocket이라는 멤버 변수에 스레드가 속한 소켓을 붙입니다.이 소켓은 리슨 소켓으로부터 받은 클라이언트 소켓을 나타냄
	m_ClientSocket.SetWnd(m_hWnd); // SetWnd : Sendmessage 활용을 위한 메인의 핸들을 받는 함수

	//ServerSocket에서 활용하던 Sendmessage 이곳으로 이동
	SendMessage(m_hWnd, WM_ACCEPT_SOCKET, 0, (LPARAM)&m_ClientSocket);//SendMessage 함수를 사용하여 메인 윈도우 (m_hWnd)로 WM_ACCEPT_SOCKET 메시지를 보냄
	return TRUE;
}

int CSocketThread::ExitInstance()
{
	// TODO:  여기에서 각 스레드에 대한 정리를 수행합니다.
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CSocketThread, CWinThread)
END_MESSAGE_MAP()


// CSocketThread 메시지 처리기
