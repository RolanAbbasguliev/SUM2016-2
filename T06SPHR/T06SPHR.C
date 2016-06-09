/* FILE NAME: T06SPHR.C
 * PROGRAMMER: VG4
 * DATE: 07.06.2016
 * PURPOSE: Sphere drawing project.
 *          Main startup module.
 */

#include <time.h>
#include <windows.h>

#include "sphere.h"

/* Window class name */
#define WND_CLASS_NAME "My window class"

/* Forward references */
LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );

/* The main program function.
 * ARGUMENTS:
 *   - handle of application instance:
 *       HINSTANCE hInstance;
 *   - dummy handle of previous application instance (not used):
 *       HINSTANCE hPrevInstance;
 *   - command line string:
 *       CHAR *CmdLine;
 *   - show window command parameter (see SW_***):
 *       INT CmdShow;
 * RETURNS:
 *   (INT) Error level for operation system (0 for success).
 */
INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    CHAR *CmdLine, INT ShowCmd )
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
     MessageBox(NULL, "Error register window class", "ERROR", MB_OK | MB_ICONERROR);
     return 0;
   }

   /* Create window */
   hWnd =
     CreateWindow(WND_CLASS_NAME,
     "MyLovelyClocks <3", 
     WS_OVERLAPPEDWINDOW,
     CW_USEDEFAULT, CW_USEDEFAULT,
     CW_USEDEFAULT, CW_USEDEFAULT,
     NULL, NULL, hInstance, NULL);
   ShowWindow(hWnd, SW_SHOWNORMAL);
   UpdateWindow(hWnd);

   /* Run message loop */
   while (GetMessage(&msg, NULL, 0, 0))
     DispatchMessage(&msg);
   return msg.wParam;
} /* End of 'MinMain' function */

/* Set/reset full screen mode function.
 * ARGUMENTS:
 *   - window to be flip:
 *       HWND hWnd;
 * RETUNRS: None.
 */
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

/* Window message handle function (CALLBACK version).
 *   - window handle:
 *       HWND hWnd;
 *   - message identifier:
 *       UINT Msg;
 *   - message first parameter ('word' param):
 *       WPARAM wParam;
 *   - message second parameter ('long' param):
 *       LPARAM lParam;
 * RETURNS:
 *   (LRESULT) message return code (depended to Msg type).
 */
LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg,
                               WPARAM wParam, LPARAM lParam )
{
  HDC hDC;
  PAINTSTRUCT ps;
  MINMAXINFO *MinMax;
  static INT W, H;
  static HBITMAP hFrame;
  static HDC hMemDCFrame;

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
    /* Setup timer */
    SetTimer(hWnd, 11, 2, NULL);

    LoadSphere();

    /* Create double buffer hDC */
    hDC = GetDC(hWnd);
    hMemDCFrame = CreateCompatibleDC(hDC);
    ReleaseDC(hWnd, hDC);
    return 0;
  case WM_SIZE:
    W = LOWORD(lParam);
    H = HIWORD(lParam);

    /* Create double buffer image */
    if (hFrame != NULL)
      DeleteObject(hFrame);
    hDC = GetDC(hWnd);
    hFrame = CreateCompatibleBitmap(hDC, W, H);
    ReleaseDC(hWnd, hDC);
    SelectObject(hMemDCFrame, hFrame);
    /* Send redraw timer message */
    SendMessage(hWnd, WM_TIMER, 0, 0);
    return 0;
  case WM_KEYDOWN:
    if (LOWORD(wParam) == 'F')
      FlipFullScreen(hWnd);
    else if (LOWORD(wParam) == VK_ESCAPE)
      DestroyWindow(hWnd);
    return 0;
  case WM_ERASEBKGND:
    /* Cancel draw background */
    return 0;
  case WM_TIMER:
    /* Clear background */
    SelectObject(hMemDCFrame, GetStockObject(NULL_PEN));
    SelectObject(hMemDCFrame, GetStockObject(DC_BRUSH));
    SetDCBrushColor(hMemDCFrame, RGB(100, 155, 220));
    Rectangle(hMemDCFrame, 0, 0, W + 1, H + 1);


    /* Draw sphere */
    DrawSphereFacets(hMemDCFrame, W / 2, H / 2, (W + H) / 3 / 2);

    /* Post repaint window message */
    InvalidateRect(hWnd, NULL, FALSE);
    return 0;
  case WM_PAINT:
    /* Redraw window frame */
    hDC = BeginPaint(hWnd, &ps);
    BitBlt(hDC, 0, 0, W, H, hMemDCFrame, 0, 0, SRCCOPY);
    EndPaint(hWnd, &ps);
    return 0;
  case WM_DESTROY:
    KillTimer(hWnd, 11);
    DeleteDC(hMemDCFrame);
    DeleteObject(hFrame);
    PostQuitMessage(0);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'MyWindowFunc' function */

/* END OF 'T06SPHR.C' FILE */
