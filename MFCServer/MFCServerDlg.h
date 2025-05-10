
// MFCServerDlg.h: 헤더 파일
//

#pragma once

#include "CServerSocket.h"

// CMFCServerDlg 대화 상자
class CMFCServerDlg : public CDialogEx
{
// 생성입니다.
public:
	CMFCServerDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCSERVER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

public:
	CListBox m_list_client;			//IDC_LIST_CLIENT
	CListBox m_list_msg;			//IDC_LIST_MSG
	CPtrList m_ptrClientSocketList;	//For manage Client Sockets
// 구현입니다.
protected:
	HICON m_hIcon;
	CclientSocket* m_pClientSocket;
	CServerSocket* m_pServerSocket;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnAcceptSocket(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCleintMsgRecv(WPARAM wParam, LPARAM lParam);
	
	afx_msg LRESULT OnClientClose(WPARAM wParam, LPARAM lParam);
public:
	void dice_roll(CString state_string);//랜덤 주사위 돌리기
	void dicestate(CString state_string);//키값에따른 락 상태값 저장
	bool dice_state[5]={0,};//주사위 락 상태값
	int dice_num[5];// 주사위 숫자
	int dice_iscore[12];//주사위 족보에 따른 스코어 값
	int player_score[2][14];//[플레이어 원,투],[족보]
	//0 ACE, 1 DEUCES, 2 THREES, 3 FOURS, 4 FIVES, 5 SIXS, 6 BONUS(SUBTOTAL), 7 CHOICE,8 4OF A KIND, 9 FULL HOUSE, 10 S.STRAIGHT, 11 L.STRAIGHT,12 YACHT, 13 TOTAL
	//ACE, DEUCES, THREES, FOURS, FIVES, SIXS, BONUS(SUBTOTAL), CHOICE, 4OF A KIND, FULL HOUSE, S.STRAIGHT, L.STRAIGHT,YACHT, TOTAL
	void dice_score();//족보에 따른 점수 계산
	void tuple(CString recvdata);
	void turn(CString str);
	bool player_turn[12][2];
	int turnset[2]={1,1};
	void senddata(CString data);
	void selectscore(CString score);
	void total();
	void game_reset();
};
