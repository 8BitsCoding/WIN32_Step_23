// Project1.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "Project1.h"

#define MAX_COUNT 30

struct TalkWnd
{
	HWND h_talk_wnd, h_edit_wnd;
	wchar_t title[256];
};

TalkWnd g_talk_list[MAX_COUNT];
int g_talk_count = 0;

HWND gh_chat_edit, gh_send_btn;
HWND gh_list_box;
HFONT gh_font;

void FindTalkWindow()
{
	TalkWnd * p = g_talk_list;
	HWND h_find_wnd = NULL;
	wchar_t str[256];
	while (h_find_wnd = FindWindowEx(NULL, h_find_wnd, L"#32770", NULL)) {
		p->h_talk_wnd = FindWindowEx(p->h_talk_wnd, NULL, L"RichEdit20W", NULL);
		if (p->h_talk_wnd != NULL) {
			p->h_talk_wnd = h_find_wnd;
			GetWindowText(p->h_talk_wnd, p->title, 256);
			swprintf_s(str, 256, L"카톡방 찾음 : %s", p->title);
			SendMessage(gh_list_box, LB_INSERTSTRING, g_talk_count, (LPARAM)str);
			p++;
			g_talk_count++;
		}
	}
}

void SendChatData()
{
	if (g_talk_count == 0) return;

	wchar_t str[128];
	GetWindowText(gh_chat_edit, str, 128);

	int index = SendMessage(gh_list_box, LB_INSERTSTRING, -1, (LPARAM)str);
	SendMessage(gh_list_box, LB_SETCURSEL, index, 0);

	TalkWnd * p = g_talk_list;
	for (int i = 0; i < g_talk_count; i++) {
		SendMessage(p->h_edit_wnd, WM_SETTEXT, 0, (LPARAM)str);
		// SendMessage는 동기식으로 메시지를 즉각 처리
		p++;
	}

	Sleep(200);		// 너무빨리 엔터를 치면 딜레이를 넣음

	p = g_talk_list;
	for (int i = 0; i < g_talk_count; i++) {
		PostMessage(p->h_edit_wnd, WM_KEYDOWN, 0x0000000D, 0x001C001);	// 엔터키를 누름
		PostMessage(p->h_edit_wnd, WM_KEYUP, 0x0000000D, 0xC01C001);	// 엔터키를 누름
		// PostMessage는 메시지 큐에 메시지를 넣는다.(안전)
		p++;
	}

	SetWindowText(gh_chat_edit, L"");
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_CREATE) {
		gh_font = CreateFont(12, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0,
			DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"굴림체");

		gh_chat_edit = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", NULL,
			WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
			5, 303, 532, 28, hWnd, (HMENU)25002, NULL, NULL);

		SendMessage(gh_chat_edit, WM_SETFONT, (WPARAM)gh_font, 1);

		gh_send_btn = CreateWindow(L"Button", L"전송",
			WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
			540, 301, 80, 31, hWnd, (HMENU)25003, NULL, NULL);

		SendMessage(gh_send_btn, WM_SETFONT, (WPARAM)gh_font, 1);

		gh_list_box = CreateWindowEx(WS_EX_CLIENTEDGE, L"ListBox", NULL,
			WS_CHILD | WS_VISIBLE | LBS_STANDARD,
			5, 5, 615, 300, hWnd, (HMENU)25001, NULL, NULL);

		SendMessage(gh_list_box, WM_SETFONT, (WPARAM)gh_font, 1);
		
		FindTalkWindow();

		return 0;
	}
	else if (uMsg == WM_COMMAND) {
		// 컨트롤을 클릭할시 발생하는 메시지
		if (LOWORD(wParam) == 25003) {
			SendChatData();
		}
	}
	else if (uMsg == WM_DESTROY) {
		DestroyWindow(gh_chat_edit);
		DestroyWindow(gh_send_btn);
		DestroyWindow(gh_list_box);
		DeleteObject(gh_font);
		PostQuitMessage(0);
	}
	else if (uMsg == WM_LBUTTONDOWN) {
		//FindTalkWindow();
		//SendChatData();
	}
		
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASS wc;

	wchar_t my_class_name[] = L"tipssoft";

	wc.cbClsExtra = NULL;
	wc.cbWndExtra = NULL;
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance = hInstance;
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = my_class_name;
	wc.lpszMenuName = NULL;
	wc.style = CS_HREDRAW | CS_VREDRAW;

	RegisterClass(&wc);

	HWND hWnd = CreateWindow(my_class_name, L"www.tipssoft.com",
		//WS_OVERLAPPEDWINDOW, 50, 50, 640, 374, NULL, NULL, hInstance, NULL);
		// Resize를 막자
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, 50, 50, 640, 374, NULL, NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}