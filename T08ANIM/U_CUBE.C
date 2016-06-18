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
  DBL TimerShift;  /* Timer shift phase value*/
  DBL TimerSpeed;  /* Timer speed value*/
  vg4OBJ Cow;      /* Cow primitive */
  vg4PRIM Pr;
  vg4PRIM Land;
  VEC Pos;         /* Model position */
  FLT DirAngle;    /* Model oriantation */
  FLT Speed;       /* Model movement speed */
  FLT Twist;       /* Model twist */
  VEC CamPos;      /* Camera position */
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
  Uni->CamPos = VecSet(30, 15, 8);
  Uni->Pos = VecSet(0, 3, 0);
  Uni->DirAngle = 0;
  /*
  VG4_RndObjLoadMatrix = MatrMulMatr(MatrScale(VecSet1(0.01)), MatrRotateY(90));
  VG4_RndObjLoad(&Uni->Cow, "models\\mi.g3d");
  */
  VG4_RndObjLoadMatrix = MatrMulMatr(MatrMulMatr(MatrTranslate(VecSet(0, 1, -5)), MatrScale(VecSet1(0.1))), MatrRotateY(90));
  VG4_RndObjLoad(&Uni->Cow, "models\\sova30.g3d");

  /*
  vg4IMG Im;

  VG4_RndPrimCreateSphere(&Uni->Pr, 1, 30, 30);
  VG4_ImageLoad(&Im, "sky.bmp");
  Uni->Pr.MtlNo = VG4_RndMtlFromImage(&Im, "sky_sphere");
  VG4_ImageFree(&Im);

  VG4_RndPrimCreateLandscape(&Uni->Land, "HF1.BMP");
  VG4_ImageLoad(&Im, "HF1TEX.BMP");
  Uni->Land.MtlNo = VG4_RndMtlFromImage(&Im, "land");
  VG4_ImageFree(&Im);
  */

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
  VG4_RndPrimFree(&Uni->Pr);
  VG4_RndPrimFree(&Uni->Land);
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
  VEC V, Shift, POI;

  Uni->Speed +=
    (Ani->Keys[VK_UP] - Ani->Keys[VK_DOWN] - Ani->JY * 1) * Ani->DeltaTime;
  Uni->DirAngle +=
    30 * (Ani->Keys[VK_LEFT] - Ani->Keys[VK_RIGHT] + Ani->Mdx * 1 - Ani->JX * 1) * Ani->DeltaTime;
  Uni->Twist = -30 * (Ani->Keys[VK_LEFT] - Ani->Keys[VK_RIGHT] + Ani->Mdx * 1 - Ani->JX * 1);

  V = VecMulMatr43(VecSet(1, 0, 0), MatrRotateY(Uni->DirAngle));
  Shift = VecMulMatr43(VecSet(-3, 3, 0), MatrRotateY(Uni->DirAngle));
  Uni->Pos = VecAddVec(Uni->Pos, VecMulNum(V, Uni->Speed * Ani->DeltaTime));

  POI = VecAddVec(Uni->Pos, Shift);
  Uni->CamPos =
    VecAddVec(Uni->CamPos,
      VecMulNum(VecSubVec(POI, Uni->CamPos), Ani->DeltaTime));
  VG4_RndMatrView = MatrView(Uni->CamPos, Uni->Pos, VecSet(0, 1, 0));
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
  VG4_RndMatrWorld =
    MatrMulMatr(MatrMulMatr(MatrRotateX(Uni->Twist), MatrRotateY(Uni->DirAngle)), MatrTranslate(Uni->Pos));
  VG4_RndObjDraw(&Uni->Cow);

  VG4_RndMatrWorld = MatrScale(VecSet(30, 30, 30));
  VG4_RndPrimDraw(&Uni->Pr);
  VG4_RndMatrWorld = MatrScale(VecSet(27, 3, 27));
  VG4_RndPrimDraw(&Uni->Land);
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
