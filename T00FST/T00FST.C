#include <stdlib.h>
#include <math.h>
#include <windows.h>

LRESULT CALLBACK MyWinFunc( HWND hWnd, UINT Msg,
                            WPARAM wParam, LPARAM lParam );

/* ������� ������� ���������.
 * ���������:
 *   - ���������� ���������� ����������:
 *       HINSTANCE hInstance;
 *   - ���������� ����������� ���������� ����������
 *     (�� ������������ � ������ ���� NULL):
 *       HINSTANCE hPrevInstance;
 *   - ��������� ������:
 *       CHAR *CmdLine;
 */
INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    CHAR *CmdLine, INT ShowCmd )
{
  WNDCLASS wc;
  MSG msg;
  HWND hWnd;

  /* �������� ������ ���� */
  wc.style = CS_HREDRAW | CS_VREDRAW;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hCursor = LoadCursor(NULL, IDC_HAND);
  wc.hIcon = LoadIcon(NULL, IDI_EXCLAMATION);
  wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
  wc.hInstance = hInstance;
  wc.lpszClassName = "My Window Class";
  wc.lpszMenuName = NULL;
  wc.lpfnWndProc = MyWinFunc;

  RegisterClass(&wc);

  /* �������� ���� */
  hWnd = CreateWindow("My Window Class",
    "30!",
    WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT, CW_USEDEFAULT,
    CW_USEDEFAULT, CW_USEDEFAULT,
    NULL, NULL, hInstance, NULL);

  ShowWindow(hWnd, ShowCmd);

  /* ������ ����� ��������� ��������� ���� */
  while (GetMessage(&msg, NULL, 0, 0))
    DispatchMessage(&msg);

  return 30;
} /* End of 'WinMain' function */

/* ������� ��������� �����.
 * ���������:
 *   - ���������� ����:
 *       HWND hWnd;
 *   - �������� ����������:
 *       HDC hDC;
 *   - ���������� ������:
 *       INT X, Y;
 *   - �������:
 *       INT R, R1;
 * ������������ ��������: ���.
 */
VOID DrawEye( HWND hWnd, HDC hDC, INT X, INT Y, INT R, INT R1 )
{
  INT px, py, len2;
  POINT pt;

  GetCursorPos(&pt);
  ScreenToClient(hWnd, &pt);

  SelectObject(hDC, GetStockObject(DC_PEN));
  SetDCPenColor(hDC, RGB(255, 0, 0));
  SelectObject(hDC, GetStockObject(DC_BRUSH));
  SetDCBrushColor(hDC, RGB(255, 255, 0));

  Ellipse(hDC, X - R, Y - R, X + R, Y + R);
  px = pt.x;
  py = pt.y;
  SetDCPenColor(hDC, RGB(255, 0, 255));
  SetDCBrushColor(hDC, RGB(0, 0, 0));

  len2 = sqrt((px - X) * (px - X) + (py - Y) * (py - Y));
  if (len2 < (R - R1))
    len2 = (R - R1);
  px = (px - X) * (R - R1) / len2 + X;
  py = (py - Y) * (R - R1) / len2 + Y;

  Ellipse(hDC, px - R1, py - R1, px + R1, py + R1);
} /* End of 'DrawEye' function */

/* ������� ��������� ��������� ����.
 * ���������:
 *   - ���������� ����:
 *       HWND hWnd;
 *   - ����� ���������:
 *       UINT Msg;
 *   - �������� ��������� ('word parameter'):
 *       WPARAM wParam;
 *   - �������� ��������� ('long parameter'):
 *       LPARAM lParam;
 * ������������ ��������:
 *   (LRESULT) - � ����������� �� ���������.
 */
LRESULT CALLBACK MyWinFunc( HWND hWnd, UINT Msg,
                            WPARAM wParam, LPARAM lParam )
{
  INT i;
  HDC hDC;
  PAINTSTRUCT ps;
  static INT w, h;

  switch (Msg)
  {
  case WM_SIZE:
    w = LOWORD(lParam);
    h = HIWORD(lParam);
    break;
  case WM_MOUSEMOVE:
    InvalidateRect(hWnd, NULL, TRUE);
    return 0;
  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);

    srand(59);
    for (i = 0; i < 30; i++)
      DrawEye(hWnd, hDC, rand() % w, rand() % h, 50, 15);

    EndPaint(hWnd, &ps);
    return 0;
  case WM_DESTROY:
    PostMessage(hWnd, WM_QUIT, 0, 0);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'MyWinFunc' function */
