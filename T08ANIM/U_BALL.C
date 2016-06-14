/* FILE NAME: U_BALL.C
 * PROGRAMMER: VG4
 * DATE: 14.06.2016
 * PURPOSE: Ball unit sample.
 */

#include <time.h>

#include "anim.h"

/* Ball unit representation type */
typedef struct
{
  vg4UNIT;         /* Base unit fields */
  VEC Pos;         /* Ball position */
  VEC Shift;       /* Ball shift position */
  DBL TimerShift;  /* Timer shift phase value*/
  DBL TimerSpeed;  /* Timer speed value*/
  COLORREF Color;  /* Color */
} vg4UNIT_BALL;

/* Unit ball initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       vg4UNIT_BALL *Uni;
 *   - animation context:
 *       vg4ANIM *Ani;
 * RETURNS: None.
 */
static VOID VG4_UnitInit( vg4UNIT_BALL *Uni, vg4ANIM *Ani )
{
  Uni->Pos = VecSet(Rnd0() * 1024, Rnd0() * 768, 0);
  Uni->TimerShift = Rnd1() * 59;
  Uni->TimerSpeed = Rnd1() * 8;
  Uni->Color = RGB(Rnd0() * 255, Rnd0() * 255, Rnd0() * 255);
} /* End of 'VG4_UnitInit' function */

/* Unit ball deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       vg4UNIT_BALL *Uni;
 *   - animation context:
 *       vg4ANIM *Ani;
 * RETURNS: None.
 */
static VOID VG4_UnitClose( vg4UNIT_BALL *Uni, vg4ANIM *Ani )
{
} /* End of 'VG4_UnitClose' function */

/* Unit ball inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       vg4UNIT_BALL *Uni;
 *   - animation context:
 *       vg4ANIM *Ani;
 * RETURNS: None.
 */
static VOID VG4_UnitResponse( vg4UNIT_BALL *Uni, vg4ANIM *Ani )
{
  DBL t = Uni->TimerSpeed * Ani->GlobalTime + Uni->TimerShift;

  Uni->Shift = VecSet(30 * sin(t), 30 * cos(t), 0);
} /* End of 'VG4_UnitResponse' function */

/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       vg4UNIT_BALL *Uni;
 *   - animation context:
 *       vg4ANIM *Ani;
 * RETURNS: None.
 */
static VOID VG4_UnitRender( vg4UNIT_BALL *Uni, vg4ANIM *Ani )
{
  VEC p = VecAddVec(Uni->Pos, Uni->Shift);

  p = p;
} /* End of 'VG4_UnitRender' function */

/* Unit ball creation function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (vg4UNIT *) pointer to created unit.
 */
vg4UNIT * VG4_UnitCreateBall( VOID )
{
  vg4UNIT_BALL *Uni;

  if ((Uni = (vg4UNIT_BALL *)VG4_AnimUnitCreate(sizeof(vg4UNIT_BALL))) == NULL)
    return NULL;
  /* Setup unit methods */
  Uni->Init = (VOID *)VG4_UnitInit;
  Uni->Close = (VOID *)VG4_UnitClose;
  Uni->Response = (VOID *)VG4_UnitResponse;
  Uni->Render = (VOID *)VG4_UnitRender;
  return (vg4UNIT *)Uni;
} /* End of 'VG4_UnitCreateBall' function */

/* END OF 'U_BALL.C' FILE */
