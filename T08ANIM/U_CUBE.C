/* FILE NAME: U_CUBE.C
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
  VEC Pos;         /* Cube position */
  DBL TimerShift;  /* Timer shift phase value*/
  DBL TimerSpeed;  /* Timer speed value*/
  vg4PRIM Cow;     /* Cow primitive */
} vg4UNIT_CUBE;

/* Unit cube initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       vg4UNIT_CUBE *Uni;
 *   - animation context:
 *       vg4ANIM *Ani;
 * RETURNS: None.
 */
static VOID VG4_UnitInit( vg4UNIT_CUBE *Uni, vg4ANIM *Ani )
{
  Uni->Pos = VecSet(Rnd1() * 5, Rnd1() * 5, Rnd1() * 5);
  Uni->TimerShift = Rnd1() * 59;
  Uni->TimerSpeed = Rnd1() * 8;

  switch (rand() % 4)
  {
  case 0:
    VG4_RndPrimLoad(&Uni->Cow, "models\\cow.g3d");
    break;
  case 1:
    VG4_RndPrimLoad(&Uni->Cow, "models\\piggy.g3d");
    break;
  case 2:
    VG4_RndPrimLoad(&Uni->Cow, "models\\sova30.g3d");
    break;
  case 3:
    VG4_RndPrimLoad(&Uni->Cow, "models\\bw.g3d");
    break;
  }
} /* End of 'VG4_UnitInit' function */

/* Unit cube deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       vg4UNIT_CUBE *Uni;
 *   - animation context:
 *       vg4ANIM *Ani;
 * RETURNS: None.
 */
static VOID VG4_UnitClose( vg4UNIT_CUBE *Uni, vg4ANIM *Ani )
{
  VG4_RndPrimFree(&Uni->Cow);
} /* End of 'VG4_UnitClose' function */

/* Unit cube inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       vg4UNIT_CUBE *Uni;
 *   - animation context:
 *       vg4ANIM *Ani;
 * RETURNS: None.
 */
static VOID VG4_UnitResponse( vg4UNIT_CUBE *Uni, vg4ANIM *Ani )
{
} /* End of 'VG4_UnitResponse' function */

/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       vg4UNIT_CUBE *Uni;
 *   - animation context:
 *       vg4ANIM *Ani;
 * RETURNS: None.
 */
static VOID VG4_UnitRender( vg4UNIT_CUBE *Uni, vg4ANIM *Ani )
{
  VG4_RndMatrWorld = MatrMulMatr(MatrScale(VecSet(0.030, 0.030, 0.030)),
                     MatrMulMatr(MatrRotateY((Uni->TimerSpeed * Ani->Time) * 30 + Uni->TimerShift),
                                 MatrTranslate(VecAddVec(Uni->Pos,
                                                         VecMulNum(VecSet(/*Ani->JX, Ani->JY, Ani->JZ*/0, 0, 0), 3)))));
  VG4_RndPrimDraw(&Uni->Cow);
} /* End of 'VG4_UnitRender' function */

/* Unit cube creation function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (vg4UNIT *) pointer to created unit.
 */
vg4UNIT * VG4_UnitCreateCube( VOID )
{
  vg4UNIT_CUBE *Uni;

  if ((Uni = (vg4UNIT_CUBE *)VG4_AnimUnitCreate(sizeof(vg4UNIT_CUBE))) == NULL)
    return NULL;
  /* Setup unit methods */
  Uni->Init = (VOID *)VG4_UnitInit;
  Uni->Close = (VOID *)VG4_UnitClose;
  Uni->Response = (VOID *)VG4_UnitResponse;
  Uni->Render = (VOID *)VG4_UnitRender;
  return (vg4UNIT *)Uni;
} /* End of 'VG4_UnitCreateCube' function */

/* END OF 'U_CUBE.C' FILE */
