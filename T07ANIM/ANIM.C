/* FILE NAME: ANIM.C
 * PROGRAMMER: VG4
 * DATE: 11.06.2016
 * PURPOSE: Animation system.
 */

#include <stdio.h>

#include "anim.h"
#include <mmsystem.h>

#pragma comment(lib, "winmm")

/* Joystic axis value obtain macro function */
#define VG4_GET_JOYSTIC_AXIS(A) (2.0 * (ji.dw##A##pos - jc.w##A##min) / (jc.w##A##max - jc.w##A##min - 1) - 1)

/* Global mouse wheel delta value */
INT VG4_MouseWheel;

/* Global animation context */
vg4ANIM VG4_Anim;

/* Timer local data */
static UINT64
  VG4_StartTime,    /* Start program time */
  VG4_OldTime,      /* Time from program start to previous frame */
  VG4_OldTimeFPS,   /* Old time FPS measurement */
  VG4_PauseTime,    /* Time during pause period */
  VG4_TimePerSec,   /* Timer resolution */
  VG4_FrameCounter; /* Frames counter */

/* Animation system initialization function.
 * ARGUMENTS:
 *   - work window:
 *       HWND hWnd;
 * RETURNS: None.
 */
VOID VG4_AnimInit( HWND hWnd )
{
  HDC hDC;
  LARGE_INTEGER t;

  memset(&VG4_Anim, 0, sizeof(vg4ANIM));

  /* Store window and create memory device context */
  VG4_Anim.hWnd = hWnd;
  hDC = GetDC(hWnd);
  VG4_Anim.hDC = CreateCompatibleDC(hDC);
  ReleaseDC(hWnd, hDC);

  /* Timer initialization */
  QueryPerformanceFrequency(&t);
  VG4_TimePerSec = t.QuadPart;
  QueryPerformanceCounter(&t);
  VG4_StartTime = VG4_OldTime = VG4_OldTimeFPS = t.QuadPart;
  VG4_PauseTime = 0;

  /* Render setup */
  VG4_RndMatrWorld = MatrIdentity();
  VG4_RndMatrView = MatrView(VecSet(5, 5, 5), VecSet(0, 0, 0), VecSet(0, 1, 0));
  VG4_RndMatrProj = MatrFrustum(-1, 1, -1, 1, 1, 100);

  VG4_RndProjDist = 1;
  VG4_RndFarClip = 500;
  VG4_RndProjSize = 1;
} /* End of 'VG4_AnimInit' function */

/* Animation system deinitialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID VG4_AnimClose( VOID )
{
  INT i;

  /* Destroy all units */
  for (i = 0; i < VG4_Anim.NumOfUnits; i++)
  {
    VG4_Anim.Units[i]->Close(VG4_Anim.Units[i], &VG4_Anim);
    free(VG4_Anim.Units[i]);
  }
  VG4_Anim.NumOfUnits = 0;

  /* Delete GDI data */
  DeleteDC(VG4_Anim.hDC);
  DeleteObject(VG4_Anim.hFrame);

  memset(&VG4_Anim, 0, sizeof(vg4ANIM));
} /* End of 'VG4_AnimClose' function */

/* Animation system resize window handle function.
 * ARGUMENTS:
 *   - new frame size:
 *       INT W, H;
 * RETURNS: None.
 */
VOID VG4_AnimResize( INT W, INT H )
{
  HDC hDC;

  /* Store new window frame size */
  VG4_Anim.W = W;
  VG4_Anim.H = H;

  /* Create double buffer image */
  if (VG4_Anim.hFrame != NULL)
    DeleteObject(VG4_Anim.hFrame);
  hDC = GetDC(VG4_Anim.hWnd);
  VG4_Anim.hFrame = CreateCompatibleBitmap(hDC, W, H);
  ReleaseDC(VG4_Anim.hWnd, hDC);

  SelectObject(VG4_Anim.hDC, VG4_Anim.hFrame);

  VG4_RndSetProj();
} /* End of 'VG4_AnimClose' function */

/* Animation system resize window handle function.
 * ARGUMENTS:
 *   - device context to be copy to:
 *       HDC hDC;
 * RETURNS: None.
 */
VOID VG4_AnimCopyFrame( HDC hDC )
{
  BitBlt(hDC, 0, 0, VG4_Anim.W, VG4_Anim.H, VG4_Anim.hDC, 0, 0, SRCCOPY);
} /* End of 'VG4_AnimCopyFrame' function */

/* Animation system render frame function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID VG4_AnimRender( VOID )
{
  INT i;
  POINT pt;
  LARGE_INTEGER t;

  /*** Handle timer ***/
  VG4_FrameCounter++;
  QueryPerformanceCounter(&t);
  /* Global time */
  VG4_Anim.GlobalTime = (DBL)(t.QuadPart - VG4_StartTime) / VG4_TimePerSec;
  VG4_Anim.GlobalDeltaTime = (DBL)(t.QuadPart - VG4_OldTime) / VG4_TimePerSec;
  /* Time with pause */
  if (VG4_Anim.IsPause)
  {
    VG4_Anim.DeltaTime = 0;
    VG4_PauseTime += t.QuadPart - VG4_OldTime;
  }
  else
  {
    VG4_Anim.DeltaTime = VG4_Anim.GlobalDeltaTime;
    VG4_Anim.Time = (DBL)(t.QuadPart - VG4_PauseTime - VG4_StartTime) / VG4_TimePerSec;
  }
  /* FPS */
  if (t.QuadPart - VG4_OldTimeFPS > VG4_TimePerSec)
  {
    CHAR str[100];

    VG4_Anim.FPS = VG4_FrameCounter * VG4_TimePerSec / (DBL)(t.QuadPart - VG4_OldTimeFPS);
    VG4_OldTimeFPS = t.QuadPart;
    sprintf(str, "FPS: %.5f", VG4_Anim.FPS);
    SetWindowText(VG4_Anim.hWnd, str);
    VG4_FrameCounter = 0;
  }
  VG4_OldTime = t.QuadPart;

  /*** Obtain input system state ***/

  /* Keyboard */
  GetKeyboardState(VG4_Anim.Keys);
  for (i = 0; i < 256; i++)
  {
    VG4_Anim.Keys[i] >>= 7;
    if (!VG4_Anim.OldKeys[i] && VG4_Anim.Keys[i])
      VG4_Anim.KeysClick[i] = TRUE;
    else
      VG4_Anim.KeysClick[i] = FALSE;
  }
  memcpy(VG4_Anim.OldKeys, VG4_Anim.Keys, 256);

  /* Mouse */
  GetCursorPos(&pt);
  ScreenToClient(VG4_Anim.hWnd, &pt);
  VG4_Anim.Mdx = pt.x - VG4_Anim.Mx;
  VG4_Anim.Mdy = pt.y - VG4_Anim.My;
  VG4_Anim.Mx = pt.x;
  VG4_Anim.My = pt.y;

  /* Joystick */
  if (joyGetNumDevs() > 0)
  {
    JOYCAPS jc;

    /* Get joystick info */
    if (joyGetDevCaps(JOYSTICKID1, &jc, sizeof(jc)) == JOYERR_NOERROR)
    {
      JOYINFOEX ji;

      ji.dwSize = sizeof(JOYINFOEX);
      ji.dwFlags = JOY_RETURNALL;
      if (joyGetPosEx(JOYSTICKID1, &ji) == JOYERR_NOERROR)
      {
        /* Buttons */
        for (i = 0; i < 32; i++)
          VG4_Anim.JBut[i] = (ji.dwButtons >> i) & 1;

        /* Axes */
        VG4_Anim.JX = VG4_GET_JOYSTIC_AXIS(X);
        VG4_Anim.JY = VG4_GET_JOYSTIC_AXIS(Y);
        VG4_Anim.JZ = VG4_GET_JOYSTIC_AXIS(Z);
        VG4_Anim.JR = VG4_GET_JOYSTIC_AXIS(R);

        /* Point of view */
        VG4_Anim.JPov = ji.dwPOV == 0xFFFF ? 0 : ji.dwPOV / 4500 + 1;
      }
    }
  }

  /*** Send response to all units ***/
  for (i = 0; i < VG4_Anim.NumOfUnits; i++)
    VG4_Anim.Units[i]->Response(VG4_Anim.Units[i], &VG4_Anim);

  /*** Clear frame ***/
  /* Clear background */
  SelectObject(VG4_Anim.hDC, GetStockObject(NULL_PEN));
  SelectObject(VG4_Anim.hDC, GetStockObject(DC_BRUSH));
  SetDCBrushColor(VG4_Anim.hDC, RGB(100, 155, 220));
  Rectangle(VG4_Anim.hDC, 0, 0, VG4_Anim.W + 1, VG4_Anim.H + 1);

  /*** Render all units ***/
  for (i = 0; i < VG4_Anim.NumOfUnits; i++)
  {
    SelectObject(VG4_Anim.hDC, GetStockObject(DC_PEN));
    SelectObject(VG4_Anim.hDC, GetStockObject(DC_BRUSH));
    SetDCPenColor(VG4_Anim.hDC, RGB(0, 0, 0));
    SetDCBrushColor(VG4_Anim.hDC, RGB(255, 255, 255));

    VG4_Anim.Units[i]->Render(VG4_Anim.Units[i], &VG4_Anim);
  }
} /* End of 'VG4_AnimRender' function */

/* Add new unit to animation system function.
 * ARGUMENTS:
 *   - pointer to new unit:
 *       vg4UNIT *Uni;
 * RETURNS: None.
 */
VOID VG4_AnimAddUnit( vg4UNIT *Uni )
{
  if (VG4_Anim.NumOfUnits < VG4_MAX_UNITS)
  {
    VG4_Anim.Units[VG4_Anim.NumOfUnits] = Uni;
    VG4_Anim.Units[VG4_Anim.NumOfUnits]->Init(VG4_Anim.Units[VG4_Anim.NumOfUnits], &VG4_Anim);
    VG4_Anim.NumOfUnits++;
    /* other version:
    VG4_Anim.Units[VG4_Anim.NumOfUnits++] = Uni;
    Uni->Init(Uni, &VG4_Anim);
    */
  }
} /* End of 'VG4_AnimAddUnit' function */

/* End animation function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID VG4_AnimDoExit( VOID )
{
  static BOOL IsExit = FALSE;

  if (IsExit)
    return;
  IsExit = TRUE;
  /* DestroyWindow(VG4_Anim.hWnd); */
  PostMessage(VG4_Anim.hWnd, WM_CLOSE, 0, 0);
} /* End of 'VG4_AnimDoExit' function */

/* Set/reset full screen mode function.
 * ARGUMENTS: None.
 * RETUNRS: None.
 */
VOID VG4_AnimFlipFullScreen( VOID )
{
  static BOOL IsFullScreen = FALSE;
  static RECT SaveRect;

  if (IsFullScreen)
  {
    /* restore window size */
    SetWindowPos(VG4_Anim.hWnd, HWND_TOP,
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
    GetWindowRect(VG4_Anim.hWnd, &SaveRect);

    /* Get nearest monitor */
    hmon = MonitorFromWindow(VG4_Anim.hWnd, MONITOR_DEFAULTTONEAREST);

    /* Obtain monitor info */
    moninfo.cbSize = sizeof(moninfo);
    GetMonitorInfo(hmon, (MONITORINFO *)&moninfo);

    /* Set window new size */
    rc = moninfo.rcMonitor;
    AdjustWindowRect(&rc, GetWindowLong(VG4_Anim.hWnd, GWL_STYLE), FALSE);

    SetWindowPos(VG4_Anim.hWnd, HWND_TOPMOST,
      rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top,
      SWP_NOOWNERZORDER);
  }
  IsFullScreen = !IsFullScreen;
} /* End of 'VG4_AnimFlipFullScreen' function */

/* END OF 'ANIM.C' FILE */
