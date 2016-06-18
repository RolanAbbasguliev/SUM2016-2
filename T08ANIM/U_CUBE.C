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
  vg4OBJ Cow;     /* Cow primitive */
  UINT tx;
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
  /*
  FLT tex[2][2] =
  {
    {1, 0},
    {0, 1}
  };
  vg4IMG Im;

  VG4_ImageLoad(&Im, "models/a.bmp");

  glGenTextures(1, &Uni->tx);
  glBindTexture(GL_TEXTURE_2D, Uni->tx);
  / * glTexImage2D(GL_TEXTURE_2D, 0, 1, 2, 2, 0, GL_LUMINANCE, GL_FLOAT, tex); * /
  glTexImage2D(GL_TEXTURE_2D, 0, 4, Im.W, Im.H, 0, GL_BGRA, GL_UNSIGNED_BYTE, Im.Bits);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glBindTexture(GL_TEXTURE_2D, 0);
  VG4_ImageFree(&Im);
  */
  Uni->Pos = VecSet(Rnd1() * 5, Rnd1() * 5, Rnd1() * 5);
  Uni->TimerShift = Rnd1() * 59;
  Uni->TimerSpeed = Rnd1() * 8;
  Uni->Pos = VecSet(0, 0, 0);

  VG4_RndObjLoad(&Uni->Cow, "models\\x6.g3d");

  /*
  switch (rand() % 5)
  {
  case 0:
    VG4_RndObjLoad(&Uni->Cow, "models\\cow.g3d");
    break;
  case 1:
    VG4_RndObjLoad(&Uni->Cow, "models\\piggy.g3d");
    break;
  case 2:
    VG4_RndObjLoad(&Uni->Cow, "models\\sova30.g3d");
    break;
  case 3:
    VG4_RndObjLoad(&Uni->Cow, "models\\bw.g3d");
    break;
  case 4:
    VG4_RndObjLoad(&Uni->Cow, "models\\x6.g3d");
    break;
  }
  */
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
  VG4_RndObjFree(&Uni->Cow);
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
  INT i;

  for (i = 0; i <= 0; i++)
  {
    VG4_RndMatrWorld = MatrIdentity();
    VG4_RndMatrWorld = MatrMulMatr(VG4_RndMatrWorld,
      MatrScale(VecSet(0.30, 0.30, 0.30)));
    VG4_RndMatrWorld = MatrMulMatr(VG4_RndMatrWorld,
      MatrRotateY((Uni->TimerSpeed * Ani->Time) * 30 * 0 + 0 * Uni->TimerShift));
    VG4_RndMatrWorld = MatrMulMatr(VG4_RndMatrWorld,
      MatrTranslate(VecAddVec(Uni->Pos, VecSet(i * 1, 0, 0))));

    VG4_RndObjDraw(&Uni->Cow);
  }
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
