/* FILE NAME: T02CLOCK.C
 * PROGRAMMER: VG4
 * DATE: 02.06.2016
 * PURPOSE: WinAPI windowed clock drawing application sample.
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

/* Window message handle function */
LRESULT CALLBACK MyWinFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
  HDC hDC;
  PAINTSTRUCT ps;
  static INT w, h;
  static HDC hDC1;
  static HBITMAP hbm;
  static BITMAP bm;

  switch (Msg)
  {
  case WM_CREATE:
    SetTimer(hWnd, 30, 10, NULL);
    hbm = LoadImage(NULL, "M.BMP", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    GetObject(hbm, sizeof(bm), &bm);
    hDC = GetDC(hWnd);
    hDC1 = CreateCompatibleDC(hDC);
    ReleaseDC(hWnd, hDC);
    SelectObject(hDC1, hbm);
    Ellipse(hDC1, 0, 0, 30, 30);
    return 0;
  case WM_SIZE:
    w = LOWORD(lParam);
    h = HIWORD(lParam);
    return 0;
  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);


    /* hDC <- hbm??? */
    BitBlt(hDC, 0, 0, bm.bmWidth, bm.bmHeight, hDC1, 0, 0, SRCCOPY);
    StretchBlt(hDC, 0, 0, w, h, hDC1, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
    BitBlt(hDC, 0, 200, bm.bmWidth, bm.bmHeight, hDC, 0, 0, SRCINVERT);


    EndPaint(hWnd, &ps);
    return 0;
  case WM_DESTROY:
    KillTimer(hWnd, 30);
    DeleteObject(hbm);
    DeleteDC(hDC1);
    PostQuitMessage(0);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'MyWinFunc' function */

/* END OF 'T02CLOCK.C' FILE */


