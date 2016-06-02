#include <stdlib.h>
#include <math.h>
#include <windows.h>

LRESULT CALLBACK MyWinFunc( HWND hWnd, UINT Msg,
                            WPARAM wParam, LPARAM lParam );

/* Главная функция программы.
 * АРГУМЕНТЫ:
 *   - дескриптор экземпляра приложения:
 *       HINSTANCE hInstance;
 *   - дескриптор предыдущего экземпляра приложения
 *     (не используется и должно быть NULL):
 *       HINSTANCE hPrevInstance;
 *   - командная строка:
 *       CHAR *CmdLine;
 */
INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    CHAR *CmdLine, INT ShowCmd )
{
  WNDCLASS wc;
  MSG msg;
  HWND hWnd;

  /* Создание класса окна */
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

  /* Создание окна */
  hWnd = CreateWindow("My Window Class",
    "30!",
    WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT, CW_USEDEFAULT,
    CW_USEDEFAULT, CW_USEDEFAULT,
    NULL, NULL, hInstance, NULL);

  ShowWindow(hWnd, ShowCmd);

  /* Запуск цикла обработки сообщений окна */
  while (GetMessage(&msg, NULL, 0, 0))
    DispatchMessage(&msg);

  return 30;
} /* End of 'WinMain' function */

/* Функция рисования глаза.
 * АРГУМЕНТЫ:
 *   - дескриптор окна:
 *       HWND hWnd;
 *   - контекст устройства:
 *       HDC hDC;
 *   - координаты центра:
 *       INT X, Y;
 *   - радиусы:
 *       INT R, R1;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
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

/* Функция обработки сообщения окна.
 * АРГУМЕНТЫ:
 *   - дескриптор окна:
 *       HWND hWnd;
 *   - номер сообщения:
 *       UINT Msg;
 *   - параметр сообшения ('word parameter'):
 *       WPARAM wParam;
 *   - параметр сообшения ('long parameter'):
 *       LPARAM lParam;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (LRESULT) - в зависимости от сообщения.
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
