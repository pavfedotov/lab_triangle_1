
//------------------------------------------------------- 
//      GL_2D.cpp
//       =========
//  ��������� ������� �����  � OpenGL
//--------------------------------------------------------
#define STRICT

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <gl\gl.h>
#include <gl\\glu.h>
// #include <gl\\glaux.h>
#pragma comment (linker, "/defaultlib:opengl32.lib")
#pragma comment (linker, "/defaultlib:glu32.lib")


#define MIN(a,b) ((a)>=(b) ? (b) : (a))
#define MAX(a,b) ((a)>=(b) ? (a) : (b))
#define MsgBox(str) MessageBox(hwndMain, str, aczAppName, 0L)

char aczAppName[512] = "GlBaseSamp";
LPCSTR pczMainWndClass = "WC__FrameWnd";

HINSTANCE hiApp;
HWND hwndMain;
HDC hdcMain;
HGLRC hGlRc;
// ������� ������ � ������� �����������
RECT rcViewport;

//  ������ ������� ������ � ��������� (���������� ���)
# define DIM_SCENE 2.0 


void InitOpenGL();
void UninitOpenGL();
void InitViewport(const int cx, const int cy);
void Draw();
void DrawModel(int numVertices);
LRESULT WINAPI MainWndProc(HWND, UINT, WPARAM, LPARAM);
void MainOnPaint(HWND);
int RegisterWndClasses(void);


// ������������� ��������� ��������������� OpenGL
void InitOpenGL()
{
	hdcMain = GetDC(hwndMain);
	int iPixelFormat;
	// ������������� ��������� ������ ��������
	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),  // ������ ���������
		1,                              // ����� ������ OpenGL
		PFD_DRAW_TO_WINDOW |          // ��������� ������ � ����
		PFD_SUPPORT_OPENGL |          // ��������� OpenGL
		PFD_DOUBLEBUFFER,		// ������� �����������
		PFD_TYPE_RGBA,          // ����� � ������ RGBA
		24,                     // 24 ������� �� ����
		0, 0, 0, 0, 0, 0,       // ���� ����� ������������
		0,                      // �� ������������ �����-��������
		0,                      // �������� ������ ������������
		0,                      // ����� ������������ �� ������������
		0, 0, 0, 0,             // ���� ������������ ������������
		32,                     // 32-��������� ����� �������
		0,                      // ����� ��������� �� ������������
		0,                      // ��������������� ����� �� ������������
		PFD_MAIN_PLANE,         // �������� ����
		0,                      // ��������������
		0, 0, 0                 // ����� ���� ������������
	};
	// ������ pix-�������
	iPixelFormat = ChoosePixelFormat(hdcMain, &pfd);
	if (!iPixelFormat)
		MsgBox("�������� ������ ��������");
	// ��������� pix-�������
	if (!SetPixelFormat(hdcMain, iPixelFormat, &pfd))
		MsgBox("������ �������� �� ����������");
	// �������� OpenGL-���������
	hGlRc = wglCreateContext(hdcMain);
	if (!hGlRc)
		MsgBox("�������� OpenGL �� ������");
	// ��������� �������� OpenGL-���������
	if (!wglMakeCurrent(hdcMain, hGlRc))
		MsgBox("�������� OpenGL �� ����������");
}  // func InitOpenGL

 // ������������ ��������� ���������������
void UninitOpenGL()
{
	// �������� ������� �������� ��������������� OpenGL
	wglMakeCurrent(NULL, NULL);
	// ��������� ��������� ��������������� OpenGL
	if (hGlRc)
		wglDeleteContext(hGlRc);
	// ������������ ��������� ������� �������
	ReleaseDC(hwndMain, hdcMain);
}  //func UninitOpenGL

 // ������������� ������� ������
void InitViewport(const int cx,
	const int cy)
{
	// ������ ������� ������ OpenGL �� ����������� ������� ����
	rcViewport.left = 10;
	rcViewport.top = 10;
	int viewSize = MIN(cx, cy) - 20;
	rcViewport.right = rcViewport.left + viewSize;
	rcViewport.bottom = rcViewport.top + viewSize;
	glMatrixMode(GL_PROJECTION);
	glViewport(rcViewport.left,
		cy - rcViewport.bottom,
		viewSize, viewSize);
	glLoadIdentity();
	glOrtho(-DIM_SCENE, DIM_SCENE, -DIM_SCENE, DIM_SCENE, -DIM_SCENE, DIM_SCENE);
	glMatrixMode(GL_MODELVIEW);
}  //func InitViewport

 // ����� ������
void DrawModel(int numVertices)
{
	// ���������� ������ ���� ����
	glClearColor(0, 0, 0, 1.0);

	// ���������� �����.
	double k_gab = 0.95;
	double razmer_figuri = 1.5; // 

	// �������� ����� �����
	glClear(GL_COLOR_BUFFER_BIT);


   // ���������� ������� �������  ������
	glColor3d(1, 1, 1);
	glBegin(GL_LINE_LOOP);
	glVertex2d(-DIM_SCENE * k_gab, -DIM_SCENE * k_gab);
	glVertex2d(DIM_SCENE * k_gab, -DIM_SCENE * k_gab);
	glVertex2d(DIM_SCENE * k_gab, DIM_SCENE * k_gab);
	glVertex2d(-DIM_SCENE * k_gab, DIM_SCENE * k_gab);
	glVertex2d(-DIM_SCENE * k_gab, -DIM_SCENE * k_gab);
	glEnd();


	double x0 = -DIM_SCENE * k_gab, y0= (DIM_SCENE * k_gab) / 2, incr = 0.3;


	glBegin(GL_LINE_STRIP);
	glColor3d(1, 1, 1);
	y0 += 0.3;
	for (int i = 0; i < numVertices/2; i++) {
		double x = x0 + (incr*2)*i;

		glVertex2d(x, y0);
		glVertex2d(x+incr, y0+incr);

		glVertex2d(x + incr, y0 + incr);
		glVertex2d(x+2*incr, y0);
		
	}
	glEnd();
	
	
	
	glBegin(GL_TRIANGLES);
	glColor3d(0, 1, 0);

	for (int i = 0; i < numVertices; i++) {
		y0 = (DIM_SCENE * k_gab) / 2;
		double x = x0 + incr * i;
		glVertex2d(x, y0);
		glVertex2d(x, y0 + incr);
		glVertex2d(x + incr, y0 + incr);
		glVertex2d(x + incr, y0 + incr);
		glVertex2d(x, y0);
		glVertex2d(x + incr, y0);
		
		y0 -=0.3;
		x = x0 + incr * i;
		glVertex2d(x, y0);
		glVertex2d(x, y0 + incr);
		glVertex2d(x + incr, y0 + incr);
		glVertex2d(x + incr, y0 + incr);
		glVertex2d(x, y0);
		glVertex2d(x + incr, y0);
	}
	glEnd();
	

	glBegin(GL_LINE_STRIP);
	glColor3d(1, 0, 0);
	for (int i = 0; i < numVertices; i++) {
		y0 = (DIM_SCENE * k_gab) / 2;
		double x = x0 + incr * i;
		glVertex2d(x, y0);
		glVertex2d(x, y0 + incr);
		glVertex2d(x + incr, y0 + incr);
		glVertex2d(x + incr, y0 + incr);
		glVertex2d(x, y0);
		glVertex2d(x + incr, y0);

	}
	glEnd();

	y0 = (DIM_SCENE * k_gab) / 2;
	glBegin(GL_LINE_STRIP);
	glColor3d(1, 0, 0);

	for (int i = 0; i < numVertices; i++) {

		double x = x0 + incr * i;
		glVertex2d(x, y0);
		glVertex2d(x, y0 + incr);
		glVertex2d(x + incr, y0 + incr);
		glVertex2d(x + incr, y0 + incr);
		glVertex2d(x, y0);
		glVertex2d(x + incr, y0);
		glVertex2d(x + incr, y0 + incr);
	}
	glEnd();

	y0 -= 0.3;
	glBegin(GL_LINE_STRIP);
	glColor3d(1, 0, 0);

	for (int i = 0; i < numVertices; i++) {

		double x = x0 + incr * i;

		glVertex2d(x, y0);
		glVertex2d(x + incr, y0);

		glVertex2d(x, y0);
		glVertex2d(x, y0 + incr);

		glVertex2d(x, y0 + incr);
		glVertex2d(x + incr, y0 + incr);

		glVertex2d(x + incr, y0);
		glVertex2d(x + incr, y0 + incr);

		glVertex2d(x, y0 + incr);
		glVertex2d(x + incr, y0);


	}

	glEnd();

	glBegin(GL_QUADS);
	glColor3d(1, 0, 0);
	y0 = ((DIM_SCENE * k_gab) / 2)+0.3;

	for (int i = 0; i < numVertices / 2; i++) {
		double x = x0 + (incr * 2) * i;
		double m = 0.05;

		glVertex2d(x + m, y0 - m);
		glVertex2d(x + m, y0 + m);
		glVertex2d(x - m, y0 + m);
		glVertex2d(x - m, y0 - m);


		glVertex2d(x + incr +m, y0 + incr-m);
		glVertex2d(x + incr+m, y0 + incr+m);
		glVertex2d(x + incr-m, y0 + incr+m);
		glVertex2d(x + incr-m, y0 + incr-m);

		glVertex2d(x + 2*incr + m, y0  - m);
		glVertex2d(x + 2 * incr + m, y0  + m);
		glVertex2d(x + 2 * incr - m, y0  + m);
		glVertex2d(x + 2 * incr - m, y0 - m);

		
		

	}
	glEnd();




	glFinish();
	// ����������� ����� �� ���������� ������ � ��������
	SwapBuffers(hdcMain);
}  // func DrawModel

// ������� ��� ��������� n ��������������� � �������� radius � ������� � ����� (x, y)
void DrawRectangles(double x, double y, double radius, int n)
{
	// �������� ��������� �����
	glBegin(GL_LINE_LOOP);

	// ��������� ��������� �����
	glEnd();
}
// ��������� �������� ����
LRESULT WINAPI MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {
	case WM_SIZE:
		InitViewport(LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_PAINT:
		MainOnPaint(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0L;
}  // func MainWndProc

 // ���������� WM_PAINT
void MainOnPaint(HWND hwnd)
{
	PAINTSTRUCT ps;
	BeginPaint(hwnd, &ps);

	DrawModel(8);
	// ������ ��������������


	EndPaint(hwnd, &ps);
}  //func MainOnPaint

 // ����������� ������� ����
int RegisterWndClasses()
{
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(wc));
	wc.hInstance = hiApp;
	wc.style = CS_VREDRAW | CS_HREDRAW | CS_OWNDC;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpfnWndProc = (WNDPROC)MainWndProc;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	// wc.hbrBackground = CreateSolidBrush(GetSysColor(COLOR_3DFACE));
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	//wc.lpszMenuName  = MAKEINTRESOURCE(IDR_MENU);
	wc.lpszClassName = pczMainWndClass;
	if (!RegisterClassEx(&wc))
		return 0;
	return 1;
}  // func RegisterWndClasses

 // ������������� ����������
void InitializeApp()
{
	// ���������������� ����� � ������� ������� ����
	RegisterWndClasses();
	hwndMain = CreateWindow(pczMainWndClass, NULL,
		WS_CAPTION | WS_VISIBLE | WS_SYSMENU | WS_THICKFRAME |
		WS_OVERLAPPED | WS_MAXIMIZEBOX | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
		0, 0, 800, 600, NULL,
		NULL, hiApp, NULL);
	if (!hwndMain)
		return;
	// ���������������� OpenGL
	InitOpenGL();
	// ������� ����� � ���� ��� ������� ������� ������
	SelectObject(hdcMain, GetStockObject(NULL_BRUSH));
	SelectObject(hdcMain, GetStockObject(WHITE_PEN));
	//
	RECT r;
	GetClientRect(hwndMain, &r);
	InitViewport(r.right, r.bottom);
}  //func InitializeApp

 // ������������ �������� ����������
void UninitializeApp()
{
	UnregisterClass(pczMainWndClass, hiApp);
}  //func UninitializeApp

 // ����� ����� ����������
int APIENTRY WinMain(HINSTANCE hi, HINSTANCE hi_void, LPSTR pc_cl, int wsm)
{
	hiApp = hi;
	InitializeApp();
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
		DispatchMessage(&msg);
	UninitializeApp();
	return 0;
}  //func WinMain


 // --- main.cpp ---
