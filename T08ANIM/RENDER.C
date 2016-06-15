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

/* Primitive draw function.
 * ARGUMENTS:
 *   - primtive to draw:
 *       vg4PRIM *Pr;
 * RETURNS: None.
 */
VOID VG4_RndPrimDraw( vg4PRIM *Pr )
{
  INT loc;
  MATR M;

  /* Build transform matrix */
  M = MatrMulMatr(VG4_RndMatrWorld,
    MatrMulMatr(VG4_RndMatrView, VG4_RndMatrProj));
  glLoadMatrixf(M.A[0]);

  glUseProgram(VG4_RndPrg);

  /* Setup global variables */
  if ((loc = glGetUniformLocation(VG4_RndPrg, "MatrWorld")) != -1)
    glUniformMatrix4fv(loc, 1, FALSE, VG4_RndMatrWorld.A[0]);
  if ((loc = glGetUniformLocation(VG4_RndPrg, "MatrView")) != -1)
    glUniformMatrix4fv(loc, 1, FALSE, VG4_RndMatrView.A[0]);
  if ((loc = glGetUniformLocation(VG4_RndPrg, "MatrProj")) != -1)
    glUniformMatrix4fv(loc, 1, FALSE, VG4_RndMatrProj.A[0]);
  if ((loc = glGetUniformLocation(VG4_RndPrg, "Time")) != -1)
    glUniform1f(loc, VG4_Anim.Time);


  /* Activete primitive vertex array */
  glBindVertexArray(Pr->VA);
  /* Activete primitive index buffer */
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Pr->IBuf);
  /* Draw primitive */
  glDrawElements(GL_TRIANGLES, Pr->NumOfI, GL_UNSIGNED_INT, NULL);
  glUseProgram(0);
} /* End of 'VG4_RndPrimDraw' function */

/* Primitive free function.
 * ARGUMENTS:
 *   - primtive to free:
 *       vg4PRIM *Pr;
 * RETURNS: None.
 */
VOID VG4_RndPrimFree( vg4PRIM *Pr )
{
  glBindVertexArray(Pr->VA);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glDeleteBuffers(1, &Pr->VBuf);
  glBindVertexArray(0);
  glDeleteVertexArrays(1, &Pr->VA);
  glDeleteBuffers(1, &Pr->IBuf);
  memset(Pr, 0, sizeof(vg4PRIM));
} /* End of 'VG4_RndPrimFree' function */

/* END OF 'RENDER.C' FILE */
