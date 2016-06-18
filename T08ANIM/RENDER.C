/* FILE NAME: RENDER.C
 * PROGRAMMER: VG4
 * DATE: 13.06.2016
 * PURPOSE: Render handle functions.
 */

#include <stdio.h>
#include <string.h>

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

/* Shader support */
UINT VG4_RndPrg;

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

/* END OF 'RENDER.C' FILE */
