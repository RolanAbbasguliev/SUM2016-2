/* FILE NAME: U_CONTRL.C
 * PROGRAMMER: VG4
 * DATE: 14.06.2016
 * PURPOSE: Control unit sample.
 */

#include <time.h>

#include "anim.h"
#include "units.h"

/* Clock unit representation type */
typedef struct
{
  vg4UNIT; /* Base unit fields */
  VEC Pos;
} vg4UNIT_CONTROL;

/* Unit clock initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       vg4UNIT_CONTROL *Uni;
 *   - animation context:
 *       vg4ANIM *Ani;
 * RETURNS: None.
 */
static VOID VG4_UnitInit( vg4UNIT_CONTROL *Uni, vg4ANIM *Ani )
{
  Uni->Pos = VecSet(0, 0, 8);
} /* End of 'VG4_UnitInit' function */

/* Unit clock inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       vg4UNIT_CONTROL *Uni;
 *   - animation context:
 *       vg4ANIM *Ani;
 * RETURNS: None.
 */
static VOID VG4_UnitResponse( vg4UNIT_CONTROL *Uni, vg4ANIM *Ani )
{
  DBL r;

  if (Ani->Keys['T'])
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  if (Ani->Keys['Y'])
    glPolygonMode(GL_FRONT, GL_LINE);
  if (Ani->Keys['U'])
    glPolygonMode(GL_BACK, GL_LINE);
  if (Ani->Keys[VK_SPACE])
    VG4_AnimAddUnit(VG4_UnitCreateBall());
  if (Ani->KeysClick['C'])
    VG4_AnimAddUnit(VG4_UnitCreateCube());
  if (Ani->KeysClick[VK_RETURN] && Ani->Keys[VK_MENU])
    VG4_AnimFlipFullScreen();
  if (Ani->KeysClick[VK_ESCAPE])
    VG4_AnimDoExit();
  if (Ani->KeysClick['P'])
    Ani->IsPause = !Ani->IsPause;

  /* Uni->Pos.Y += Ani->JY * Ani->GlobalDeltaTime; */
  Uni->Pos = VecMulMatr43(Uni->Pos, MatrRotateX(59 * Ani->JY * Ani->GlobalDeltaTime));
  Uni->Pos = VecMulMatr43(Uni->Pos, MatrRotateY(59 * Ani->JX * Ani->GlobalDeltaTime));

  if (Ani->Keys[VK_LBUTTON])
  {
    Uni->Pos = VecMulMatr43(Uni->Pos, MatrRotateY(59 * Ani->Mdx * Ani->GlobalDeltaTime));
    Uni->Pos = VecMulMatr43(Uni->Pos, MatrRotateX(59 * Ani->Mdy * Ani->GlobalDeltaTime));
  }

  Uni->Pos = VecMulMatr43(Uni->Pos, MatrRotateY(59 * Ani->Keys[VK_RIGHT] * Ani->GlobalDeltaTime));
  Uni->Pos = VecMulMatr43(Uni->Pos, MatrRotateY(-59 * Ani->Keys[VK_LEFT] * Ani->GlobalDeltaTime));

  r = VecLen(Uni->Pos);
  Uni->Pos = VecMulNum(Uni->Pos, (r + Ani->Mdz * Ani->GlobalDeltaTime * 0.1) / r);

  VG4_RndMatrView = MatrView(Uni->Pos, VecSet(0, 0, 0), VecSet(0, 1, 0));
} /* End of 'VG4_UnitResponse' function */

/* Unit clock render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       vg4UNIT_CONTROL *Uni;
 *   - animation context:
 *       vg4ANIM *Ani;
 * RETURNS: None.
 */
static VOID VG4_UnitRender( vg4UNIT_CONTROL *Uni, vg4ANIM *Ani )
{
} /* End of 'VG4_UnitRender' function */

/* Unit clock creation function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (vg4UNIT *) pointer to created unit.
 */
vg4UNIT * VG4_UnitCreateClock( VOID )
{
  vg4UNIT_CONTROL *Uni;

  if ((Uni = (vg4UNIT_CONTROL *)VG4_AnimUnitCreate(sizeof(vg4UNIT_CONTROL))) == NULL)
    return NULL;
  /* Setup unit methods */
  Uni->Init = (VOID *)VG4_UnitInit;
  Uni->Response = (VOID *)VG4_UnitResponse;
  Uni->Render = (VOID *)VG4_UnitRender;
  return (vg4UNIT *)Uni;
} /* End of 'VG4_UnitCreateClock' function */

/* END OF 'U_CONTRL.C' FILE */
