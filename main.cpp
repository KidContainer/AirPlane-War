//����ͷ�ļ�
#include <windows.h>
#include <time.h>
#include "resource.h"
#include <stdlib.h>
#include <stdio.h>

//�������ļ�
#pragma comment (lib,"winmm.lib")  //�������� ����lib

typedef struct Node
{
	int x;
	int y;
	struct Node *pnext;

}DiRen,FeiJi,ZiDan;
//TODO��ȫ�ֳ�ʼ������
DiRen *pDiRen = NULL;
ZiDan *pZiDan = NULL;
FeiJi *pFeiJi = NULL;
static int score = 0;
static wchar_t nScore[20];
static int var = 0;	//�ж��Լ������Ĵ���
int Direnmovevar = 10;
//TODO:�����ڴ�����
void CreateDiRen();
void ShowDiRen(struct Node *pHead, HWND hWnd);
void CreateZiJi(HWND hWnd);
void CreateZiDan();
void ShowZiDan(ZiDan *pHead, HWND hWnd);
void DiRenMove(DiRen *pHead);
void ZiDanMove(ZiDan *pHead);
void Shoot(HWND hWnd, FeiJi *ziji, DiRen **diren, ZiDan **zidan);
void ReleaseDiRen(DiRen **pHead);
void ReleaseZiDan(ZiDan **pHead);
void DirenMoveVar();
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
//�궨�� define
#define TIME_DIREN         101
#define TIME_DIRENMOVE     102  
#define TIME_ZIDAN         103
#define TIME_DIRENRELEASE  104
#define TIME_ZIDANMOVE     105
#define TIME_ZIDANRELEASE  107
//WinMain()����
//
//
//WindowsӦ�ó������
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,int nCmdShow)
{
	//�������� 
	WNDCLASSEX wndClass = { 0 };
	wndClass.style=CS_HREDRAW|CS_VREDRAW|CS_OWNDC;
	wndClass.cbClsExtra=0;
	wndClass.cbWndExtra=0;
	wndClass.hbrBackground=(HBRUSH)5;
	wndClass.hInstance=hInstance;
	wndClass.hCursor=NULL;
	wndClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wndClass.lpfnWndProc=WndProc;
	wndClass.lpszClassName=L"AirPlane";
	wndClass.lpszMenuName=NULL;
	wndClass.hIconSm = NULL;
	wndClass.cbSize = sizeof(WNDCLASSEX);
	//ע�ᴰ��
	if (!RegisterClassEx(&wndClass))
	{
		return -1;
	}
	//��������
	HWND hWnd = CreateWindow(L"AirPlane", L"����ս��", WS_OVERLAPPEDWINDOW,100,100,600,600,NULL,NULL,hInstance,NULL);
	//�ƶ� ��ʾ ���´���
	ShowWindow(hWnd,nCmdShow);
	UpdateWindow(hWnd);

	//PlaySound(L"music.wav",NULL,SND_FILENAME|SND_ASYNC|SND_LOOP); //ѭ�����ű�������
	//��Ϣѭ��
	MSG msg = { 0 };
	while (GetMessage(&msg,NULL,0,0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int i=1;
	int jscore;
	HDC hDC;
	HDC memDC;
	HBITMAP hBitmap;
	BITMAP bBitmap;
	switch (message)
	{
	case WM_TIMER:
		hDC = GetDC(hWnd); //�õ��豸���
		hBitmap = LoadBitmap(GetModuleHandle(NULL),MAKEINTRESOURCE(IDB_BITMAP1));  //���뱳��
		GetObject(hBitmap,sizeof(hBitmap),&bBitmap);
		memDC = CreateCompatibleDC(hDC);
		SelectObject(memDC,hBitmap);
		BitBlt(hDC,0,0,600,600,memDC,0,0,SRCCOPY);
		wsprintf(nScore, L"%d", score);
		jscore = score;
		while ((jscore = jscore / 10) > 0)
		{
			i++;
		}
		TextOut(hDC, 0, 0, L"����:", 4);
		TextOut(hDC, 30, 0, nScore, i);
		DeleteDC(memDC);
		ReleaseDC(hWnd, hDC);
		DeleteObject(hBitmap);
		CreateZiJi(hWnd);
		if (TIME_ZIDAN == wParam)
		{
			ZiDanMove(pZiDan);
			ReleaseZiDan(&pZiDan);
		}
		else if (TIME_DIREN == wParam)
		{
			CreateDiRen();
		}
		else if (TIME_DIRENRELEASE == wParam)
		{
			ReleaseDiRen(&pDiRen);
		}
		ShowDiRen(pDiRen, hWnd);
		DiRenMove(pDiRen);
		ShowZiDan(pZiDan,hWnd);
		ZiDanMove(pZiDan);
		Shoot(hWnd, pFeiJi, &pDiRen,&pZiDan);
		break;
	case WM_CREATE:
		srand(time(NULL));
		SetTimer(hWnd, TIME_DIREN, 1000, NULL);
		pFeiJi = (struct Node*)malloc(sizeof(struct Node));
		pFeiJi->x = 200;
		pFeiJi->y = 500;
		SetTimer(hWnd, TIME_DIREN, 1500, NULL);
		SetTimer(hWnd, TIME_DIRENMOVE, 200, NULL);
		SetTimer(hWnd, TIME_ZIDAN, 100, NULL);
		SetTimer(hWnd, TIME_ZIDANMOVE, 50, NULL);
		SetTimer(hWnd,TIME_DIRENRELEASE,300,NULL);
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_LEFT:
			if (pFeiJi->x > 0)
				pFeiJi->x -= 20;
			break;
		case VK_RIGHT:
			if (pFeiJi->x < 550)
				pFeiJi->x += 20;
			break;
		case VK_UP:
			if (pFeiJi->y >0)
				pFeiJi->y -= 20;
			break;
		case VK_DOWN:
			if (pFeiJi->y < 550)
				pFeiJi->y += 20;
			break;
		case VK_SPACE:
			CreateZiDan();
			break;
		case VK_RETURN:
			int n = 0;
				if (n = 0)
				{
					KillTimer(hWnd, TIME_DIREN);
					break;
				}
			  if (n = 1)
			  {
				SetTimer(hWnd, TIME_DIREN, 1500, NULL);
				n = 0;
				break;
			  }
			break;
		}
		break;
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
		return DefWindowProc(hWnd, message, wParam, lParam);
}
//��������
void CreateDiRen()
{
	DiRen *u;
	u = (struct Node*)malloc(sizeof(struct Node));
	u->x = rand() % 550;  //����������ֵ�x����
	u->y = -10;
	u->pnext = NULL;
	if (NULL == pDiRen)
	{
		pDiRen = u;
	}
	else
	{
		u->pnext = pDiRen;
		pDiRen = u;
	}
}
//��ʾ����
void ShowDiRen(struct Node *pHead,HWND hWnd)
{
	HDC hDC;
	HDC memDC;
	HBITMAP hBitmap;
	BITMAP bBitmap;
	hDC = GetDC(hWnd); //�õ��豸���
	hBitmap = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP2));  //�������
	GetObject(hBitmap, sizeof(hBitmap), &bBitmap);
	memDC = CreateCompatibleDC(hDC);
	SelectObject(memDC, hBitmap);
	while (pHead!=NULL)
	{
		//͸�����ַ�
		//���� ��Ϸ����
		BitBlt(hDC, pHead->x, pHead->y, 40, 40, memDC, 40, 0, SRCAND);
		BitBlt(hDC, pHead->x, pHead->y, 40, 40, memDC, 0, 0, SRCPAINT);
		pHead = pHead->pnext;
	}
	DeleteDC(memDC);
	ReleaseDC(hWnd, hDC);
	DeleteObject(hBitmap);
}
//�����Լ�
void CreateZiJi(HWND hWnd)
{
	HDC hDC;
	HDC memDC;
	HBITMAP hBitmap;
	BITMAP bBitmap;
	hDC = GetDC(hWnd); //�õ��豸���
	hBitmap = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP3));  //�����Լ�
	GetObject(hBitmap, sizeof(hBitmap), &bBitmap);
	memDC = CreateCompatibleDC(hDC);
	SelectObject(memDC, hBitmap);
		//͸�����ַ�
	BitBlt(hDC, pFeiJi->x, pFeiJi->y, 40, 40, memDC, 40, 0, SRCAND);
	BitBlt(hDC, pFeiJi->x, pFeiJi->y, 40, 40, memDC, 0, 0, SRCPAINT);
	DeleteDC(memDC);
	ReleaseDC(hWnd, hDC);
	DeleteObject(hBitmap);
}
void CreateZiDan()
{
	ZiDan *u;
	u = (ZiDan*)malloc(sizeof(ZiDan));
	u->x = pFeiJi->x+20;  //�ӵ�����
	u->y = pFeiJi->y+10;
	u->pnext = NULL;
	if (NULL == pZiDan)
	{
		pZiDan = u;
	}
	else
	{
		u->pnext = pZiDan;
		pZiDan = u;
	}
}
//��ʾ����
void ShowZiDan(ZiDan *pHead,HWND hWnd)
{
	HDC hDC;
	HDC memDC;
	HBITMAP hBitmap;
	BITMAP bBitmap;
	hDC = GetDC(hWnd); //�õ��豸���
	hBitmap = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP4));  //�����ӵ�
	GetObject(hBitmap, sizeof(hBitmap), &bBitmap);
	memDC = CreateCompatibleDC(hDC);
	SelectObject(memDC, hBitmap);
	while (pHead != NULL)
	{
		//͸�����ַ�
		//���� ��Ϸ����
		BitBlt(hDC, pHead->x, pHead->y, 15, 25, memDC, 15, 0, SRCAND);
		BitBlt(hDC, pHead->x, pHead->y, 15, 25, memDC, 0, 0, SRCPAINT);
		pHead = pHead->pnext;
	}
	DeleteDC(memDC);
	ReleaseDC(hWnd, hDC);
	DeleteObject(hBitmap);
}
//�����ƶ�
void DiRenMove(DiRen *pHead)
{
	while (pHead!=NULL)
	{
		pHead->y += Direnmovevar;
		pHead = pHead->pnext;
	}
}
//�ӵ��ƶ�  ����
void ZiDanMove(ZiDan *pHead)
{
	while (pHead != NULL)
	{
		pHead->y -= 20;
		pHead = pHead->pnext;
	}
}
void Shoot(HWND hWnd, FeiJi *ziji, DiRen **diren, ZiDan **zidan)
{
	DiRen *js1 = *diren;
	ZiDan *js2 = *zidan;
	int n = 1;
	//�ж��Լ��Ƿ�ײ���Է�
	while (js1 != NULL)
	{
		if ((ziji->x - js1->x<30 && ziji->x - js1->x>-38) && (ziji->y-js1->y<25&&ziji->y-js1->y>-38))
		{
			KillTimer(hWnd, TIME_DIREN);
			KillTimer(hWnd, TIME_ZIDAN);
			KillTimer(hWnd, TIME_DIRENMOVE);
			KillTimer(hWnd, TIME_ZIDANMOVE);
			KillTimer(hWnd, TIME_DIRENRELEASE);
			MessageBox(hWnd, L"��Ϸ���������������ж����㱻�����ˣ�  ���ߣ�������", L"����ս����Ϣ", MB_OK);
			PostQuitMessage(0);
			break;
		}
		else
		{
			js1 = js1->pnext;
		}
	}
	//�жϵл��е�
	js1 = *diren;
	js2 = *zidan;
	while ((js1 = *diren) != NULL)
	{
		zidan = &pZiDan;
		n = 0;
		while ((js2 = *zidan) != NULL)
		{
			//�ж��Ƿ���е���
			if ((js2->x - js1->x <= 40 && js2->x - js1->x >= -5) && (js2->y - js1->y <= 40 && js2->y - js1->y >= -5))
			{
				n = 1;
				score += 100;
				*zidan = js2->pnext;
				if (js1->pnext != NULL)
				{
					*diren = js1->pnext;
					diren = &pDiRen;
					free(js1);
					free(js2);
				}
				else
					*diren = NULL;	
				break;
			}
			else
				zidan = &js2->pnext; //���û�� �ӵ��ж���һ��
		}
		if (n != 1)
			diren = &js1->pnext; //�����ж���һ��
	}
}
//�ͷ�
void ReleaseZiDan(ZiDan **pHead)
{
	ZiDan *js = *pHead;
	while ((js = *pHead) != NULL)
	{
		if (js->y < 0)
		{
			*pHead = js->pnext;
			free(js);
		}
		else
			pHead = &js->pnext;
	}
}
void ReleaseDiRen(DiRen **pHead)
{
	ZiDan *js = *pHead;
	while ((js = *pHead) != NULL)
	{
		if (js->y >600)
		{
			*pHead = js->pnext;
			free(js);
		}
		else
			pHead = &js->pnext;
	}
}
void DirenMoveVar()
{
	if (var = 20)
		Direnmovevar = 15;

}