/* FILE NAME: MAIN.C
 * PROGRAMMER: VG4
 * DATE: 08.06.2016
 * PURPOSE: Animation project.
 *          Main startup module.
 */

#include <windows.h>

#include "vec.h"

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
  MATR m;

  m = MatrIdentity();
  m = MatrTranslate(VecSet(1, 2, 3));
  m = MatrScale(VecSet(1, 2, 3));
  m = MatrRotateX(30);
  m = MatrRotateY(30);
  m = MatrRotateZ(30);
  m = MatrRotate(30, VecSet(1, 2, 3));
  m = MatrMulMatr(MatrRotateX(90), MatrScale(VecSet(2, 2, 2)));
  m = MatrTranspose(m);
  m = MatrInverse(m);
  m = MatrView(VecSet(5, 5, 5), VecSet(0, 0, 0), VecSet(0, 1, 0));
  m = MatrOrtho(-1, 1, -1, 1, -1, 1);
  m = MatrFrustum(-1, 1, -1, 1, 1, 1000);

} /* End of 'MinMain' function */

/* END OF 'MAIN.C' FILE */

