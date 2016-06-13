/* FILE NAME: U_CLOCK.C
 * PROGRAMMER: VG4
 * DATE: 13.06.2016
 * PURPOSE: Clock unit sample.
 */

#include <time.h>

#include "anim.h"
#include "units.h"

/* Ball unit representation type */
typedef struct
{
  vg4UNIT; /* Base unit fields */
} vg4UNIT_CLOCK;

/* Unit clock initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       vg4UNIT_CLOCK *Uni;
 *   - animation context:
 *       vg4ANIM *Ani;
 * RETURNS: None.
 */
static VOID VG4_UnitInit( vg4UNIT_CLOCK *Uni, vg4ANIM *Ani )
{
} /* End of 'VG4_UnitInit' function */

/* Unit ball inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       vg4UNIT_CLOCK *Uni;
 *   - animation context:
 *       vg4ANIM *Ani;
 * RETURNS: None.
 */
static VOID VG4_UnitResponse( vg4UNIT_CLOCK *Uni, vg4ANIM *Ani )
{
  if (Ani->Keys[VK_SPACE])
    VG4_AnimAddUnit(VG4_UnitCreateBall());
  if (Ani->Keys['C'])
    VG4_AnimAddUnit(VG4_UnitCreateCube());
  if (Ani->KeysClick[VK_RETURN] && Ani->Keys[VK_MENU])
    VG4_AnimFlipFullScreen();
  if (Ani->KeysClick[VK_ESCAPE])
    VG4_AnimDoExit();
} /* End of 'VG4_UnitResponse' function */

/* Unit clock render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       vg4UNIT_CLOCK *Uni;
 *   - animation context:
 *       vg4ANIM *Ani;
 * RETURNS: None.
 */
static VOID VG4_UnitRender( vg4UNIT_CLOCK *Uni, vg4ANIM *Ani )
{
  DBL a, r;
  SYSTEMTIME t;

  GetLocalTime(&t);

  a = (t.wSecond) * 2 * 3.1415926535 / 60;
  r = Ani->W / 2.2;

  if (Ani->JBut[0])
    SetDCPenColor(Ani->hDC, RGB(255, 0, 0));
  else
    SetDCPenColor(Ani->hDC, RGB(0, 0, 0));

  MoveToEx(Ani->hDC, Ani->W / 2 + Ani->JX * 30, Ani->H / 2 + Ani->JY * 30, NULL);
  LineTo(Ani->hDC, Ani->W / 2 + Ani->JX * 30 + sin(a) * r, Ani->H / 2 + Ani->JY * 30 - cos(a) * r);
} /* End of 'VG4_UnitRender' function */

/* Unit clock creation function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (vg4UNIT *) pointer to created unit.
 */
vg4UNIT * VG4_UnitCreateClock( VOID )
{
  vg4UNIT_CLOCK *Uni;

  if ((Uni = (vg4UNIT_CLOCK *)VG4_AnimUnitCreate(sizeof(vg4UNIT_CLOCK))) == NULL)
    return NULL;
  /* Setup unit methods */
  Uni->Init = (VOID *)VG4_UnitInit;
  Uni->Response = (VOID *)VG4_UnitResponse;
  Uni->Render = (VOID *)VG4_UnitRender;
  return (vg4UNIT *)Uni;
} /* End of 'VG4_UnitCreateClock' function */

/* END OF 'U_CLOCK.C' FILE */
