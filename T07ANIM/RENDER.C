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
  POINT *pts;

  /* Build transform matrix */
  M = MatrMulMatr(VG4_RndMatrWorld,
    MatrMulMatr(VG4_RndMatrView, VG4_RndMatrProj));

  /* Transform all points */
  pts = malloc(sizeof(POINT) * Pr->NumOfP);
  if (pts == NULL)
    return;
  for (i = 0; i < Pr->NumOfP; i++)
  {
    /* NDC */
    VEC p = VecMulMatr(Pr->P[i], M);

    pts[i].x = (p.X + 1) * VG4_Anim.W / 2;
    pts[i].y = (-p.Y + 1) * VG4_Anim.H / 2;
  }

  /* Draw all lines */
  for (i = 0; i < Pr->NumOfE; i++)
  {
    INT n0 = Pr->Edges[i][0], n1 = Pr->Edges[i][1];

    MoveToEx(VG4_Anim.hDC, pts[n0].x, pts[n0].y, NULL);
    LineTo(VG4_Anim.hDC, pts[n1].x, pts[n1].y);
  }
  free(pts);
} /* End of 'VG4_RndPrimDraw' function */

/* END OF 'RENDER.C' FILE */
