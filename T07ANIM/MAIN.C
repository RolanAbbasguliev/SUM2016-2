/* FILE NAME: MAIN.C
 * PROGRAMMER: VG4
 * DATE: 11.06.2016
 * PURPOSE: Animation project.
 *          Main startup module.
 */

#include "anim.h"
#include "units.h"

/* Window class name */
#define VG4_WND_CLASS_NAME "My window class"

/* Forward references */
LRESULT CALLBACK VG4_MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );

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
  /* INT i; */
  WNDCLASS wc;
  HWND hWnd;
  MSG msg;

  SetDbgMemHooks();

  /* Create window class */
  wc.style = CS_VREDRAW | CS_HREDRAW;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);
  wc.hIcon = LoadIcon(NULL,IDI_ERROR);
  wc.lpszMenuName = NULL;
  wc.hInstance = hInstance;
  wc.lpfnWndProc = VG4_MyWindowFunc;
  wc.lpszClassName = VG4_WND_CLASS_NAME;

  if (!RegisterClass(&wc))
  {
    MessageBox(NULL, "Error register window class", "ERROR", MB_OK | MB_ICONERROR);
    return 0;
  }
  /* Create window */
  hWnd =
    CreateWindow(VG4_WND_CLASS_NAME,
    "Animation System <3", 
    WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT, CW_USEDEFAULT,
    CW_USEDEFAULT, CW_USEDEFAULT,
    NULL, NULL, hInstance, NULL);
  ShowWindow(hWnd, SW_SHOWNORMAL);
  UpdateWindow(hWnd);

  /*** Add unit samples ***/
  /*
  for (i = 0; i < 3000; i++)
    VG4_AnimAddUnit(VG4_UnitCreateBall());
  */
  VG4_AnimAddUnit(VG4_UnitCreateClock());

  /* Run message loop */
  while (GetMessage(&msg, NULL, 0, 0))
    DispatchMessage(&msg);
  return 30;
} /* End of 'WinMain' function */

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
LRESULT CALLBACK VG4_MyWindowFunc( HWND hWnd, UINT Msg,
                                   WPARAM wParam, LPARAM lParam )
{
  HDC hDC;
  PAINTSTRUCT ps;
  MINMAXINFO *MinMax;

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

    VG4_AnimInit(hWnd);
    return 0;
  case WM_SIZE:
    VG4_AnimResize(LOWORD(lParam), HIWORD(lParam));

    /* Send redraw timer message */
    SendMessage(hWnd, WM_TIMER, 0, 0);
    return 0;
  case WM_MOUSEWHEEL:
    VG4_MouseWheel += (SHORT)HIWORD(wParam);
    return 0;
  case WM_ERASEBKGND:
    /* Cancel draw background */
    return 0;
  case WM_TIMER:
    VG4_AnimRender();
    /* Post repaint window message */
    InvalidateRect(hWnd, NULL, FALSE);
    return 0;
  case WM_PAINT:
    /* Redraw window frame */
    hDC = BeginPaint(hWnd, &ps);
    VG4_AnimCopyFrame(hDC);
    EndPaint(hWnd, &ps);
    return 0;
  case WM_DESTROY:
    VG4_AnimClose();
    KillTimer(hWnd, 11);
    PostQuitMessage(0);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'MyWindowFunc' function */

/* END OF 'MAIN.C' FILE */

