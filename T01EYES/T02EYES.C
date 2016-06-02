/* FILE NAME: T02EYES.C
 * PROGRAMMER: VG4
 * DATE: 02.06.2016
 * PURPOSE: WinAPI windowed application sample.
 */

#include <stdlib.h>
#include <math.h>

#include <windows.h>

/* Window class name */
#define WND_CLASS_NAME "My Window Class"

/* Forward references */
LRESULT CALLBACK MyWinFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );

/* The main program function */
INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    CHAR *CmdLine, INT CmdShow )
{
  WNDCLASS wc;
  HWND hWnd;
  MSG msg;

  /* Register window class */
  wc.style = CS_HREDRAW | CS_VREDRAW;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hCursor = LoadCursor(NULL, IDC_HAND);
  wc.hIcon = LoadIcon(NULL, IDI_EXCLAMATION);
  wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
  wc.hInstance = hInstance;
  wc.lpszClassName = WND_CLASS_NAME;
  wc.lpszMenuName = NULL;
  wc.lpfnWndProc = MyWinFunc;

  if (!RegisterClass(&wc))
  {
    MessageBox(NULL, "Error register window class", "ERROR", MB_OK | MB_ICONERROR);
    return 0;
  }

  /* Create window */
  hWnd = CreateWindow(WND_CLASS_NAME,
    "30!",
    WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT, CW_USEDEFAULT,
    CW_USEDEFAULT, CW_USEDEFAULT,
    NULL, NULL, hInstance, NULL);
  if (hWnd == NULL)
  {
    MessageBox(NULL, "Create window erroe", "ERROR", MB_OK | MB_ICONERROR);
    return 0;
  }

  /* Show window */
  ShowWindow(hWnd, CmdShow);
  UpdateWindow(hWnd);

  /* Run message loop */
  while (GetMessage(&msg, NULL, 0, 0))
    DispatchMessage(&msg);

  return 30;
} /* End of 'WinMain' function */

/* Draw eye function */
VOID DrawEye( HWND hWnd, HDC hDC, INT X, INT Y, INT R, INT R1 )
{
  POINT pt;
  INT dx, dy;
  DOUBLE t;

  GetCursorPos(&pt);
  ScreenToClient(hWnd, &pt);

  dx = pt.x - X;
  dy = pt.y - Y;
  t = (R - R1) / sqrt(dx * dx + dy * dy);
  if (t < 1)
  {
    dx *= t;
    dy *= t;
  }

  Ellipse(hDC, X - R, Y - R, X + R, Y + R);
  Ellipse(hDC, X + dx - R1, Y + dy - R1, X + dx + R1, Y + dy + R1);
} /* End of 'DrawEye' function */

/* Window message handle function */
LRESULT CALLBACK MyWinFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
  INT i;
  HDC hDC;
  PAINTSTRUCT ps;
  static INT w, h;
  static BITMAP bm;
  static HBITMAP hBm, hBmLogo;
  static HDC hMemDC, hMemDCLogo;

  switch (Msg)
  {
  case WM_CREATE:
    SetTimer(hWnd, 30, 10, NULL);
    hBmLogo = LoadImage(NULL, "GLOBE.BMP", IMAGE_BITMAP, 0, 0,
        LR_LOADFROMFILE);
    GetObject(hBmLogo, sizeof(bm), &bm);
    hDC = GetDC(hWnd);
    hMemDC = CreateCompatibleDC(hDC);
    hMemDCLogo = CreateCompatibleDC(hDC);
    SelectObject(hMemDCLogo, hBmLogo);
    ReleaseDC(hWnd, hDC);
    return 0;
  case WM_SIZE:
    w = LOWORD(lParam);
    h = HIWORD(lParam);
    if (hBm != NULL)
      DeleteObject(hBm);
    hDC = GetDC(hWnd);
    hBm = CreateCompatibleBitmap(hDC, w, h);
    ReleaseDC(hWnd, hDC);
    SelectObject(hMemDC, hBm);
    SendMessage(hWnd, WM_TIMER, 0, 0);
    return 0;
  case WM_TIMER:
    Rectangle(hMemDC, 0, 0, w + 1, h + 1);
    /*
    BitBlt(hMemDC, 0, 0, bm.bmWidth, bm.bmHeight,
      hMemDCLogo, 0, 0, SRCCOPY);
    */
    SetStretchBltMode(hMemDC, COLORONCOLOR);
    StretchBlt(hMemDC, 0, 0, w, h,
      hMemDCLogo, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
    DrawEye(hWnd, hMemDC, 100, 100, 50, 15);
    DrawEye(hWnd, hMemDC, 300, 200, 150, 30);
    DrawEye(hWnd, hMemDC, 100, 200, 50, 30);
    DrawEye(hWnd, hMemDC, 400, 200, 130, 30);
    DrawEye(hWnd, hMemDC, w / 2, h / 2, 80, 15);

    srand(59);
    for (i = 0; i < 30; i++)
      DrawEye(hWnd, hMemDC, rand() % 800, rand() % 700, 30, 8);

    SetBkMode(hMemDC, TRANSPARENT);
    SetTextColor(hMemDC, RGB(255, 0, 0));
    TextOut(hMemDC, 30, 30, "30!", 3);
    InvalidateRect(hWnd, NULL, FALSE);
    return 0;
  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    BitBlt(hDC, 0, 0, w, h, hMemDC, 0, 0, SRCCOPY);
    EndPaint(hWnd, &ps);
    return 0;
  case WM_DESTROY:
    KillTimer(hWnd, 30);
    DeleteDC(hMemDC);
    DeleteObject(hBm);
    PostQuitMessage(0);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'MyWinFunc' function */

/* END OF 'T02EYES.C' FILE */
