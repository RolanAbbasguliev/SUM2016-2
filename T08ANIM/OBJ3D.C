/* FILE NAME: OBJ3D.C
 * PROGRAMMER: VG4
 * DATE: 18.06.2016
 * PURPOSE: Object handle functions.
 */

#include "anim.h"

/* Object create function.
 * ARGUMENTS:
 *   - object structure pointer:
 *       vg4OBJ *Obj;
 * RETURNS: None.
 */
VOID VG4_RndObjCreate( vg4OBJ *Obj )
{
  memset(Obj, 0, sizeof(vg4OBJ));
  Obj->M = MatrIdentity();
} /* End of 'VG4_RndObjCreate' function */

/* Object draw function.
 * ARGUMENTS:
 *   - object structure pointer:
 *       vg4OBJ *Obj;
 * RETURNS: None.
 */
VOID VG4_RndObjDraw( vg4OBJ *Obj )
{
  INT i;
  MATR MSave;

  MSave = VG4_RndMatrWorld;
  VG4_RndMatrWorld = MatrMulMatr(VG4_RndMatrWorld, Obj->M);
  for (i = 0; i < Obj->NumOfPrims; i++)
    VG4_RndPrimDraw(&Obj->Prims[i]);
  VG4_RndMatrWorld = MSave;
} /* End of 'VG4_RndObjDraw' function */

/* Object free function.
 * ARGUMENTS:
 *   - object structure pointer:
 *       vg4OBJ *Obj;
 * RETURNS: None.
 */
VOID VG4_RndObjFree( vg4OBJ *Obj )
{
  INT i;

  for (i = 0; i < Obj->NumOfPrims; i++)
    VG4_RndPrimFree(&Obj->Prims[i]);
  free(Obj->Prims);
  memset(Obj, 0, sizeof(vg4OBJ));
} /* End of 'VG4_RndObjFree' function */

/* END OF 'OBJ3D.C' FILE */
