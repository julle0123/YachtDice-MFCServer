
// MFCServerDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "MFCServer.h"
#include "MFCServerDlg.h"
#include "afxdialogex.h"
#include <cstring>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define PORT 9999

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCServerDlg 대화 상자



CMFCServerDlg::CMFCServerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCSERVER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_Client, m_list_client);
	DDX_Control(pDX, IDC_LIST_MSG, m_list_msg);
}

BEGIN_MESSAGE_MAP(CMFCServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_ACCEPT_SOCKET, &CMFCServerDlg::OnAcceptSocket)
	ON_MESSAGE(WM_CLEINT_MSG_RECV, &CMFCServerDlg::OnCleintMsgRecv)
	ON_MESSAGE(WM_CLIENT_CLOSE, &CMFCServerDlg::OnClientClose)
END_MESSAGE_MAP()


// CMFCServerDlg 메시지 처리기

BOOL CMFCServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.
	m_pServerSocket = new CServerSocket;
	m_pServerSocket->SetWnd(this->m_hWnd);
	if (!m_pServerSocket->Create(PORT)) {
		AfxMessageBox(_T("Failed to create server socket"));
		EndDialog(IDCANCEL);
		return FALSE;
	}

	if (m_pServerSocket->Bind(PORT) != 0) {
		AfxMessageBox(_T("Failed to bind server socket"));
		EndDialog(IDCANCEL);
		return FALSE;
	}
	if (!m_pServerSocket->Listen())
	{
		AfxMessageBox(_T("Failed to listen on server socket"));
		EndDialog(IDCANCEL);
		return FALSE;
	}

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CMFCServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CMFCServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CMFCServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



afx_msg LRESULT CMFCServerDlg::OnAcceptSocket(WPARAM wParam, LPARAM lParam)
{
	CString str;
	m_pClientSocket = (CclientSocket*)lParam;
	m_ptrClientSocketList.AddTail(m_pClientSocket);
	POSITION pos = m_ptrClientSocketList.GetHeadPosition();
	CString playerName;
	playerName.Format(_T("Player%d"), (int)m_ptrClientSocketList.GetCount()); // player뜨게 하기
	m_pClientSocket->SetPlayerName(playerName);
	m_list_client.InsertString(-1, playerName);
	Sleep(500);
	playerName.Format(_T("%s접속"), playerName);
	senddata(playerName);
	str.Format(_T("Client (%d)"), (int)(m_pClientSocket));
	m_list_client.InsertString(-1, str);
	m_pClientSocket = NULL;
	delete m_pClientSocket;

	return 0;
}


afx_msg LRESULT CMFCServerDlg::OnCleintMsgRecv(WPARAM wParam, LPARAM lParam)
{
	CString lpszStr = (LPCTSTR)lParam;

	//BroadCasting
	POSITION pos = m_ptrClientSocketList.GetHeadPosition();
	m_list_msg.InsertString(-1, lpszStr);
	m_list_msg.SetCurSel(m_list_msg.GetCount() - 1);
	if (lpszStr.Find(_T("C|"))+1) {//key가 채팅일때 브로드캐스팅하기
		
		while (pos != NULL)
		{
			CclientSocket* pClient = (CclientSocket*)m_ptrClientSocketList.GetNext(pos);
			
			if (pClient != NULL)
			{
				//UNICODE 사용하면 기존 메모리크기 *2 해야함
				//Client에 Broadcasting하기
				pClient->Send(lpszStr, lstrlen(lpszStr) * 2);

			}
		}

		
	}else{ tuple(lpszStr); }	//키가 채팅이 아닐때 키에맞는 함수를 찾아 실행
	
	return 0;
}


afx_msg LRESULT CMFCServerDlg::OnClientClose(WPARAM wParam, LPARAM lParam)
{
	CclientSocket* pClient = (CclientSocket*)lParam;
	CString str;
	CString playerName;
	UINT idx = 0;
	POSITION pos = m_ptrClientSocketList.Find(pClient);

	if (pos != NULL)
	{
		//m_list_client에서 해당 Client 삭제
		str.Format(_T("Client (%d)"), (int)pClient);
		idx = m_list_client.SelectString(-1, (LPCTSTR)str);
		m_list_client.DeleteString(idx);
		playerName.Format(_T("Player%d"), (int)m_ptrClientSocketList.GetCount()); // player뜨게 하기
		idx = m_list_client.SelectString(-1, (LPCTSTR)playerName);
		m_list_client.DeleteString(idx);
		playerName.Format(_T("Player접속종료"));
		senddata(playerName);
		m_pServerSocket->downclients();
		//CPtrList에서도 해당 Client 삭제
		m_ptrClientSocketList.RemoveAt(pos);
	}

	return 0;
}
void CMFCServerDlg::dicestate(CString state_string) {
	CString state;
	for (int i = 0; i < 5; i++)
	{
		state = state_string.Mid(i, 1);
			if (state==L"1") {
				dice_state[i] = TRUE;
			}else { dice_state[i] = FALSE; }
	}
	
}
void CMFCServerDlg::dice_roll(CString state_string) {
	dicestate(state_string.Mid(5));
	CString data=L"D|";
	srand((unsigned int)time(NULL));
	for (int i = 0; i < 5; i++)
	{
		if (!dice_state[i]) {
			dice_num[i] = rand() % 6 + 1;
		}
		
		
	}
	data.Format(_T("%s%d/%d/%d/%d/%d"), data, dice_num[0], dice_num[1], dice_num[2], dice_num[3], dice_num[4]);
	Sleep(50);
	senddata(data);

	dice_score();
}
void CMFCServerDlg::dice_score() {
	//0 ACE, 1 DEUCES, 2 THREES, 3 FOURS, 4 FIVES, 5 SIXS, 6 BONUS(SUBTOTAL)
	//7 CHOICE,8 4OF A KIND, 9 FULL HOUSE, 10 S.STRAIGHT
	//11 L.STRAIGHT,12 YACHT, 13 TOTAL
	//dice_iscore초기화 문제 및 s.Straight 계산오류  
	CString scorename[12] = {L"ACE",L"DEUCES", L"THREES", L"FOURS", L"FIVES", L"SIXS", L"CHOICE", L"4OF A KIND", L"FULL HOUSE", L"S.STRAIGHT", L"L.STRAIGHT", L"YACHT"};
	int dice_scale[6] = { 0, };
	int count ,sum = 0;
	CString data;
	for (int i = 0; i < 12; i++)
	{
		dice_iscore[i] = 0;
	}
	for (int i = 0; i <5; i++) {
		dice_scale[dice_num[i] - 1]++;//주사위 각 눈 개수 ex 5가 2개 6이1개 1이3개
	}

	for (int i = 0; i < 6; i++) {//ACE, DEUCES, THREES, FOURS, FIVES, SIXS
		dice_iscore[i] = dice_scale[i] * (i + 1);
	}


	for (int i = 0; i < 5; i++)//Choice
	{
		dice_iscore[6] += dice_num[i];
	}

	for (int i = 0; i <= 5; i++) {//4 of a kind
		if (dice_scale[i] >= 4)  dice_iscore[7] = 29;
	}

	for (int i = 0; i <= 5; i++) {//full house
		for (int j = 0; j <= 5; j++) {
			if (dice_scale[i] == 3)
				if (dice_scale[j] == 2) dice_iscore[8] = 28;
		}
	}

	count = 0;
	for (int i = 0; i <= 2; i++) {//S,Straignt
		for (int j = i; j <= i + 3; j++) {
			if (dice_scale[j] <= 2 && dice_scale[j] != 0) { count++; }

		}
		if (count == 4) { dice_iscore[9] = 15; break; }
		count = 0;
	}

	count = 0;
	for (int i = 0; i <= 1; i++) {//L.Straight
		for (int j = i; j <= i + 4; j++) {
			if (dice_scale[j] == 1) { count++; }
			if (count == 5) {
				dice_iscore[10] = 30;
			}
		}
		count = 0;
	}

	for (int i = 0; i <= 5; i++) { //Yacht
		if (dice_scale[i] == 5) { dice_iscore[11] = 50; break; }
	}
	for (int i = 0; i < 12; i++) {
		Sleep(60);
		data.Format(_T("C|%s:%d\n"),scorename[i], dice_iscore[i]);
		senddata(data);

	}
}
void CMFCServerDlg::tuple(CString recvdata) {
	CString key[3] = { L"T|",L"S|",L"R|" };
	if (recvdata.Find(key[0])+1){turn(recvdata);}
	//else if (recvdata.Find(key[1]) + 1) { selectscore(recvdata); }
	else if (recvdata.Find(key[2]) + 1) { dice_roll(recvdata); }
		
}

void CMFCServerDlg::turn(CString str) {//P1|T|scoreend
	
	
	
		if ((turnset[0] == 12) && (turnset[1] == 2)) {
		//게임끝
			turnset[0] = 1;
			turnset[1] = 1;
			if (player_score[0][13] > player_score[1][13]) { senddata(L"C|Player1이 승리하였습니다"); }
			else if (player_score[0][13] < player_score[1][13]) { senddata(L"C|Player2가 승리하였습니다"); }
			else if (player_score[0][13] < player_score[1][13]) { senddata(L"C|Player2가 승리하였습니다"); }
			game_reset();
		}
		else {
			
			if (str.Find(L"P1|")+1) { turnset[1] = 02; }
			if (str.Find(L"P2|")+1) { 
				turnset[1] = 01; 
				turnset[0]++; }
			selectscore(str);
		}
		
	

}
void CMFCServerDlg::senddata(CString data) {
	POSITION pos = m_ptrClientSocketList.GetHeadPosition();
	 
	while (pos != NULL)
	{
		
		CclientSocket* pClient = (CclientSocket*)m_ptrClientSocketList.GetNext(pos);
		if (pClient != NULL)
		{
			//UNICODE 사용하면 기존 메모리크기 *2 해야함
			//Client에 Broadcasting하기
			pClient->Send(data, lstrlen(data) * 2);
		}
	}
	m_list_msg.InsertString(-1, data);
	m_list_msg.SetCurSel(m_list_msg.GetCount() - 1);
}
void CMFCServerDlg::selectscore(CString score) {

	CString data;
	CString scorename[13] = { L"Ace",L"Deuces", L"Threes", L"Fours", L"Fives", L"Sixes", L"Choice", L"4kind", L"Fullhouse", L"Sstraight", L"Lstraight", L"Yacht" ,L"timeout"};
	int player_num;
	if (score.Find(L"P1")+1) {player_num = 0;}
	else if (score.Find(L"P2")+1) { player_num = 1; }
	for (int i = 0; i < 12; i++)
	{
		
		if (score.Find(scorename[i]) + 1) {
			
			if(i>5){ 
				player_score[player_num][i+1] = dice_iscore[i]; 
				total();
				data.Format(_T("T|%d/%d/%d/%d/%d/%d"), turnset[0], i+1, player_score[player_num][i + 1], player_score[player_num][6], player_score[player_num][13],player_num);
			}
			else{ 
				player_score[player_num][i] = dice_iscore[i]; 
				total();
				data.Format(_T("T|%d/%d/%d/%d/%d/%d"), turnset[0], i, player_score[player_num][i], player_score[player_num][6], player_score[player_num][13],player_num);
			}
			Sleep(50);
			senddata(data);
		}
		
	}
	for (int i = 0; i < 12; i++)
	{
		dice_iscore[i] = 0;
	}
	if (score.Find(scorename[12]) + 1) {
		data. Format(_T("T|timeout/%d/%d"), turnset[0],player_num);
		senddata(data);
	
	}
	
}
void CMFCServerDlg::total() {
	int sum = 0;
	for (int j = 0; j < 2; j++)
	{
		for (int i = 0; i < 6; i++) {
			sum += player_score[j][i];
		}
		if (sum == 63) { player_score[j][6] = sum + 30; }
		else { player_score[j][6] = sum; }//subtotal 보너스 까지 계산 해줌
		sum = 0;
		for (int i = 6; i < 13; i++) {
			sum += player_score[j][i];
		}
		player_score[j][13] = sum;//total score 계산
		sum = 0;
	}
}
void CMFCServerDlg::game_reset(){
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 14; j++)
		{
			player_score[i][j] = 0;
		}
	}
	senddata(L"reset|");
}