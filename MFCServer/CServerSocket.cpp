// CServerSocket.cpp: 구현 파일
//

#include "pch.h"
#include "MFCServer.h"
#include "CServerSocket.h"
#include "CSocketThread.h"  //스레드 클래스 헤더 추가
#define MAX_CLIENTS 2
// CServerSocket

CServerSocket::CServerSocket()

	: m_MaxClients(MAX_CLIENTS), m_CurrentClients(0) {
}

CServerSocket::~CServerSocket()
{
}

void CServerSocket::SetWnd(HWND hWnd)
{
	m_hWnd = hWnd;
}
void CServerSocket::SetMaxClients(int maxClients) {
	m_MaxClients = maxClients;
}
void CServerSocket::downclients() {
	m_CurrentClients--;
}
void CServerSocket::OnAccept(int nErrorCode)
{
    if (m_CurrentClients < m_MaxClients) {
        CSocket soc;
        // 아직 허용 가능한 클라이언트 수에 도달하지 않았으면 클라이언트 연결 허용
        //CServerSocket* pClientSocket = new CServerSocket;
        if (Accept(soc)) {
            // 클라이언트 연결 성공
            m_CurrentClients++;

            // 새로운 스레드 생성
            CSocketThread* pThread = (CSocketThread*)AfxBeginThread(RUNTIME_CLASS(CSocketThread),
                THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED); //AfxBeginThread : mfc에서 제공하는 스레드 생성 함수.
            pThread->SetWnd(m_hWnd);

			if (!pThread)
			{
				soc.Close();
				AfxMessageBox(_T("ERROR :스레드가 만들어지지 않음."));
				return;
			}

			//연결되면 클라이언트쪽에 메시지 보냄
			soc.Send(_T("서버에 연결됨!"), 43);//생성된 스레드에 대한 핸들이 클라이언트 소켓을 분리하고, 클라이언트에게 "서버에 연결됨!" 메시지를 보냄

			// 접속한 소켓을 분리해서 소켓의 핸들을 스레드에게 넘겨준다
			pThread->m_hSocket = soc.Detach();
			// 생성한 스레드를 실행시킨다
			pThread->ResumeThread();
        }
    }
    else {
        // 허용 가능한 클라이언트 수를 초과한 경우 연결 거부
        CSocket clientSocket;
        Accept(clientSocket);
        clientSocket.Close();
    }

    CSocket::OnAccept(nErrorCode);
	/*// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	CSocket soc;
	if (Accept(soc))
	{
		CSocketThread* pThread = (CSocketThread*)AfxBeginThread(RUNTIME_CLASS(CSocketThread), 
			THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED); //AfxBeginThread : mfc에서 제공하는 스레드 생성 함수.
		pThread->SetWnd(m_hWnd);

		if (!pThread)
		{
			soc.Close();
			AfxMessageBox(_T("ERROR :스레드가 만들어지지 않음."));
			return;
		}

		//연결되면 클라이언트쪽에 메시지 보냄
		soc.Send(_T("서버에 연결됨!"), 43);//생성된 스레드에 대한 핸들이 클라이언트 소켓을 분리하고, 클라이언트에게 "서버에 연결됨!" 메시지를 보냄

		// 접속한 소켓을 분리해서 소켓의 핸들을 스레드에게 넘겨준다
		pThread->m_hSocket = soc.Detach();
		// 생성한 스레드를 실행시킨다
		pThread->ResumeThread();
	}

	CSocket::OnAccept(nErrorCode);*/
}
// CServerSocket 멤버 함수
