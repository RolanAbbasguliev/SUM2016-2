#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <windows.h>

#include "clockres.h"

#pragma warning(disable: 4244)

#define WND_CLASS_NAME "My window class"

LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );

/* The main program function */
INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, CHAR *CmdLine, INT ShowCmd )
{
   WNDCLASS wc;
   HWND hWnd;
   MSG msg;

   /* Create window class */
   wc.style = CS_VREDRAW | CS_HREDRAW;
   wc.cbClsExtra = 0;
   wc.cbWndExtra = 0;
   wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
   wc.hCursor = LoadCursor(NULL, IDC_ARROW);
   wc.hIcon = LoadIcon(NULL,IDI_ERROR);
   wc.lpszMenuName = NULL;
   wc.hInstance = hInstance;
   wc.lpfnWndProc = MyWindowFunc;
   wc.lpszClassName = WND_CLASS_NAME;

   if (!RegisterClass(&wc))
   {
     MessageBox(NULL, "Error register window class","ERROR", MB_OK | MB_ICONERROR);
     return 0;
   }

   /* Create window */
   hWnd =
     CreateWindow(WND_CLASS_NAME,
     "MyLovelyClocks <3", 
     WS_OVERLAPPEDWINDOW,
     CW_USEDEFAULT, CW_USEDEFAULT,
     CW_USEDEFAULT, CW_USEDEFAULT,
     NULL,
     NULL,
     hInstance,
     NULL);
   ShowWindow(hWnd, SW_SHOWNORMAL);
   UpdateWindow(hWnd);

   /* Run message loop */
   while (GetMessage(&msg, NULL, 0, 0))
     DispatchMessage(&msg);
   return msg.wParam;
} /* End of 'MinMain' function */

/* Draw figure function */
VOID Draw( HDC hDC, INT X, INT Y, DOUBLE A )
{
  INT i;
  DOUBLE rad = A * 3.14159265358979323 / 180, si = sin(rad), co = cos(rad);
  static POINT pt[] =
  {
    {-10, 0}, {-10, 50}, {0, 59 * 3}, {10, 50}, {10, 0}
  };
  POINT pt1[sizeof(pt) / sizeof(pt[0])];

  for (i = 0; i < sizeof(pt) / sizeof(pt[0]); i++)
  {
    pt1[i].x = X + pt[i].x * co - pt[i].y * si;
    pt1[i].y = Y - (pt[i].x * si + pt[i].y * co);
  }
  SelectObject(hDC, GetStockObject(DC_PEN));
  SelectObject(hDC, GetStockObject(DC_BRUSH));
  SetDCPenColor(hDC, RGB(255, 0, 0));
  SetDCBrushColor(hDC, RGB(255, 220, 220));
  Polygon(hDC, pt1, sizeof(pt) / sizeof(pt[0]));
} /* End of 'Draw' function */

/* Set/reset full screen mode function */
VOID FlipFullScreen( HWND hWnd )
{
  static BOOL IsFullScreen = FALSE;
  static RECT SaveRect;

  if (IsFullScreen)
  {
    /* restore window size */
    SetWindowPos(hWnd, HWND_TOP,
      SaveRect.left, SaveRect.top,
      SaveRect.right - SaveRect.left, SaveRect.bottom - SaveRect.top,
      SWP_NOOWNERZORDER);
  }
  else
  {
    /* Set full screen size to window */
    HMONITOR hmon;
    MONITORINFOEX moninfo;
    RECT rc;

    /* Store window old size */
    GetWindowRect(hWnd, &SaveRect);

    /* Get nearest monitor */
    hmon = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);

    /* Obtain monitor info */
    moninfo.cbSize = sizeof(moninfo);
    GetMonitorInfo(hmon, (MONITORINFO *)&moninfo);

    /* Set window new size */
    rc = moninfo.rcMonitor;
    AdjustWindowRect(&rc, GetWindowLong(hWnd, GWL_STYLE), FALSE);

    SetWindowPos(hWnd, HWND_TOPMOST,
      rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top,
      SWP_NOOWNERZORDER);
  }
  IsFullScreen = !IsFullScreen;
} /* End of 'FlipFullScreen' function */

/* Window message handle function */
LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
  HDC hDC;
  PAINTSTRUCT ps;
  DOUBLE a, r;
  CHAR s[300];
  SYSTEMTIME t;
  HPEN hPen, hOldPen;
  HBRUSH hBr, hOldBr;
  CREATESTRUCT *cs;
  MINMAXINFO *MinMax;
  static POINT pts[] =
  {
    {0, 0}, {30, 300}, {200, 20}
  };
  static HFONT hFnt;
  static INT w, h;
  static BITMAP bm, bmPig;
  static HBITMAP hFrame, hFace, hPigXOR, hPigAND;
  static HDC hMemDCFrame, hMemDCFace, hMemDCPig;

  switch (Msg)
  {
  case WM_GETMINMAXINFO:
    MinMax = (MINMAXINFO *)lParam;
    MinMax->ptMaxTrackSize.y =
      GetSystemMetrics(SM_CYMAXTRACK) +
      GetSystemMetrics(SM_CYCAPTION) +
      GetSystemMetrics(SM_CYMENU) +
      GetSystemMetrics(SM_CYBORDER) * 2;
    return 0;
  case WM_CREATE:
    cs = (CREATESTRUCT *)lParam;
    SetTimer(hWnd, 11, 2, NULL);

    hFace = LoadBitmap(cs->hInstance, (CHAR *)IDB_CLOCKFACE);
    GetObject(hFace, sizeof(bm), &bm);

    hPigXOR = LoadImage(cs->hInstance, (CHAR *)IDB_XOR, IMAGE_BITMAP, 0, 0, 0);
    GetObject(hPigXOR, sizeof(bmPig), &bmPig);
    hPigAND = LoadImage(cs->hInstance, (CHAR *)IDB_AND, IMAGE_BITMAP, 0, 0, 0);

    hDC = GetDC(hWnd);
    hMemDCFace = CreateCompatibleDC(hDC);
    hMemDCFrame = CreateCompatibleDC(hDC);
    hMemDCPig = CreateCompatibleDC(hDC);
    ReleaseDC(hWnd, hDC);

    SelectObject(hMemDCFace, hFace);

    hFnt = CreateFont(59, 0, 0, 9000, FW_BOLD, FALSE, FALSE, FALSE,
      RUSSIAN_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
      DEFAULT_QUALITY, VARIABLE_PITCH | FF_SCRIPT, "Bookman Old Style");
    SelectObject(hMemDCFrame, hFnt);
    return 0;
  case WM_SIZE:
    w = LOWORD(lParam);
    h = HIWORD(lParam);

    if (hFrame != NULL)
      DeleteObject(hFrame);
    hDC = GetDC(hWnd);
    hFrame = CreateCompatibleBitmap(hDC, w, h);
    ReleaseDC(hWnd, hDC);

    SelectObject(hMemDCFrame, hFrame);
    SendMessage(hWnd, WM_TIMER, 0, 0);
    return 0;
  case WM_KEYDOWN:
    if (LOWORD(wParam) == 'F')
      FlipFullScreen(hWnd);
    else if (LOWORD(wParam) == VK_ESCAPE)
      DestroyWindow(hWnd);
    return 0;
  case WM_ERASEBKGND:
    return 0;
  case WM_TIMER:
    SelectObject(hMemDCFrame, GetStockObject(NULL_PEN));
    Rectangle(hMemDCFrame, 0, 0, w + 1, h + 1);
    BitBlt(hMemDCFrame, (w - bm.bmWidth) / 2, (h - bm.bmHeight) / 2,
      bm.bmWidth, bm.bmHeight, hMemDCFace, 0, 0, SRCCOPY);

    GetLocalTime(&t);

    a = (t.wSecond) * 2 * 3.1415926535 / 60;
    r = bm.bmWidth / 2.2;
    
    hPen = CreatePen(PS_SOLID, 6, RGB(0xFF, 0, 0xFF));
    hOldPen = SelectObject(hMemDCFrame, hPen);
    MoveToEx(hMemDCFrame, w / 2, h / 2, NULL);
    LineTo(hMemDCFrame, w / 2 + sin(a) * r, h / 2 - cos(a) * r);
    SelectObject(hMemDCFrame, hOldPen);
    DeleteObject(hPen);

    a = (t.wMinute + t.wSecond / 60.0) * 2 * 3.1415926535 / 60;
    r = bm.bmWidth / 3.2;
    hPen = CreatePen(PS_SOLID, 10, RGB(0, 0, 255));
    hOldPen = SelectObject(hMemDCFrame, hPen);
    MoveToEx(hMemDCFrame, w / 2, h / 2, NULL);
    LineTo(hMemDCFrame, w / 2 + sin(a) * r, h / 2 - cos(a) * r);
    SelectObject(hMemDCFrame, hOldPen);
    DeleteObject(hPen);

    a = (t.wHour % 12 + t.wMinute / 60.0) * 2 * 3.1415926535 / 12;
    r = bm.bmWidth / 4.0;
    MoveToEx(hMemDCFrame, w / 2, h / 2, NULL);
    LineTo(hMemDCFrame, w / 2 + sin(a) * r, h / 2 - cos(a) * r);

    /* SetBkMode(hMemDCFrame, TRANSPARENT); */
    SetTextColor(hMemDCFrame, RGB(130, 90, 30));
    TextOut(hMemDCFrame, 0, h / 2 + h / 2 * sin(clock() / 2000.0), s,
      sprintf(s, "Current date: %02i.%02i.%i", t.wDay, t.wMonth, t.wYear));
    /* VOID WINAPI GetLocalTime(lpSystemTime); */

    SelectObject(hMemDCPig, hPigAND);
    BitBlt(hMemDCFrame, 200 + 150 * sin(clock() / 200.0), 100,
      bmPig.bmWidth, bmPig.bmHeight, hMemDCPig, 0, 0, SRCAND);
    SelectObject(hMemDCPig, hPigXOR);
    BitBlt(hMemDCFrame, 200 + 150 * sin(clock() / 200.0), 100,
      bmPig.bmWidth, bmPig.bmHeight, hMemDCPig, 0, 0, SRCINVERT);


    hBr = CreateHatchBrush(HS_DIAGCROSS, RGB(0, 0, 255));
    SetBkMode(hMemDCFrame, TRANSPARENT);
    hOldBr = SelectObject(hMemDCFrame, hBr);
    Polygon(hMemDCFrame, pts, 3);
    SelectObject(hMemDCFrame, hOldBr);
    DeleteObject(hBr);

    a = clock() / (DOUBLE)CLOCKS_PER_SEC;
    Draw(hMemDCFrame, 100, 100, 30 * sin(a * 5));
    Draw(hMemDCFrame, 100, 100, 59 * sin(-a * 8));
    Draw(hMemDCFrame, 100, 100, 36 * a);
    Draw(hMemDCFrame, 100, 100, -36 * a);

    InvalidateRect(hWnd, NULL, FALSE);
    return 0;
  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    BitBlt(hDC, 0, 0, w, h, hMemDCFrame, 0, 0, SRCCOPY);
    EndPaint(hWnd, &ps);
    return 0;
  case WM_DESTROY:
    KillTimer(hWnd, 11);
    DeleteDC(hMemDCFrame);
    DeleteDC(hMemDCFace);
    DeleteObject(hFrame);
    DeleteObject(hFace);
    DeleteObject(hFnt);
    PostQuitMessage(0);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'MyWindowFunc' function */

/* END OF 'T02CLOCK.C' FILE */
