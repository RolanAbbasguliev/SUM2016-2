/* FILE NAME: RENDER.C
 * PROGRAMMER: VG4
 * DATE: 13.06.2016
 * PURPOSE: Render handle functions.
 */

#include "anim.h"

/* Global transformation matrices */
MATR
  VG4_RndMatrWorld, /* World (object) transformation matrix */
  VG4_RndMatrView,  /* Camera view transform */
  VG4_RndMatrProj;  /* Projection transform */

/* Projection parameters */
DBL
  VG4_RndProjDist, /* Near clip plane */
  VG4_RndFarClip,  /* Far clip plane */
  VG4_RndProjSize; /* Project plane size */

/* Setup projection function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID VG4_RndSetProj( VOID )
{
  DBL ratio_x = 1, ratio_y = 1;

  if (VG4_Anim.W >= VG4_Anim.H)
    ratio_x = (DBL)VG4_Anim.W / VG4_Anim.H;
  else
    ratio_y = (DBL)VG4_Anim.H / VG4_Anim.W;

  VG4_RndMatrProj = MatrFrustum(-ratio_x * VG4_RndProjSize / 2,
                                 ratio_x * VG4_RndProjSize / 2,
                                -ratio_y * VG4_RndProjSize / 2,
                                 ratio_y * VG4_RndProjSize / 2,
                                 VG4_RndProjDist, VG4_RndFarClip);
} /* End of 'VG4_RndSetProj' function */

/* Primitive draw function.
 * ARGUMENTS:
 *   - primtive to draw:
 *       vg4PRIM *Pr;
 * RETURNS: None.
 */
VOID VG4_RndPrimDraw( vg4PRIM *Pr )
{
  INT i;
  MATR M;

  /* Build transform matrix */
  M = MatrMulMatr(VG4_RndMatrWorld,
    MatrMulMatr(VG4_RndMatrView, VG4_RndMatrProj));
  glLoadMatrixf(M.A[0]);

  /* Draw all lines */
  glBegin(GL_TRIANGLES);
  for (i = 0; i < Pr->NumOfI; i++)
  {
    glColor3fv(&Pr->V[Pr->I[i]].C.X);
    glVertex3fv(&Pr->V[Pr->I[i]].P.X);
  }
  glEnd();
} /* End of 'VG4_RndPrimDraw' function */

/* Primitive free function.
 * ARGUMENTS:
 *   - primtive to free:
 *       vg4PRIM *Pr;
 * RETURNS: None.
 */
VOID VG4_RndPrimFree( vg4PRIM *Pr )
{
  if (Pr->V != NULL)
    free(Pr->V);
  if (Pr->I != NULL)
    free(Pr->I);
  memset(Pr, 0, sizeof(vg4PRIM));
} /* End of 'VG4_RndPrimFree' function */

/* END OF 'RENDER.C' FILE */
