/* FILE NAME: LOADPRIM.C
 * PROGRAMMER: VG4
 * DATE: 13.06.2016
 * PURPOSE: Render handle functions.
 */

#include <stdio.h>
#include <string.h>

#include "anim.h"

/* Load primitive from '*.g3d' file function.
 * ARGUMENTS:
 *   - primitive structure pointer:
 *       vg4PRIM *Pr;
 *   - file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE is success, FALSE otherwise.
 */
BOOL VG4_RndPrimLoad( vg4PRIM *Pr, CHAR *FileName )
{
  FILE *F;
  DWORD Sign;
  INT NumOfPrimitives;
  CHAR MtlFile[300];
  INT NumOfP;
  INT NumOfI;
  CHAR Mtl[300];
  INT p;

  memset(Pr, 0, sizeof(vg4PRIM));

  F = fopen(FileName, "rb");
  if (F == NULL)
    return FALSE;

  /* File structure:
   *   4b Signature: "G3D\0"    CHAR Sign[4];
   *   4b NumOfPrimitives       INT NumOfPrimitives;
   *   300b material file name: CHAR MtlFile[300];
   *   repeated NumOfPrimitives times:
   *     4b INT NumOfP; - vertex count
   *     4b INT NumOfI; - index (triangles * 3) count
   *     300b material name: CHAR Mtl[300];
   *     repeat NumOfP times - vertices:
   *         !!! float point -> FLT
   *       typedef struct
   *       {
   *         VEC  P;  - Vertex position
   *         VEC2 T;  - Vertex texture coordinates
   *         VEC  N;  - Normal at vertex
   *         VEC4 C;  - Vertex color
   *       } VERTEX;
   *     repeat (NumOfF / 3) times - facets (triangles):
   *       INT N0, N1, N2; - for every triangle (N* - vertex number)
   */
  fread(&Sign, 4, 1, F);
  if (Sign != *(DWORD *)"G3D")
  {
    fclose(F);
    return FALSE;
  }
  fread(&NumOfPrimitives, 4, 1, F);
  fread(MtlFile, 1, 300, F);
  for (p = 0; p < NumOfPrimitives; p++)
  {
    /* Read primitive info */
    fread(&NumOfP, 4, 1, F);
    fread(&NumOfI, 4, 1, F);
    fread(Mtl, 1, 300, F);

    /* Allocate memory for primitive */
    if ((Pr->V = malloc(sizeof(vg4VERTEX) * NumOfP)) == NULL)
    {
      fclose(F);
      return FALSE;
    }
    if ((Pr->I = malloc(sizeof(INT) * NumOfI)) == NULL)
    {
      free(Pr->V);
      Pr->V = NULL;
      fclose(F);
      return FALSE;
    }
    Pr->NumOfV = NumOfP;
    Pr->NumOfI = NumOfI;
    fread(Pr->V, sizeof(vg4VERTEX), NumOfP, F);
    fread(Pr->I, sizeof(INT), NumOfI, F);
    if (Pr->NumOfV > 0)
    {
      INT i;

      for (i = 0; i < Pr->NumOfV; i++)
        Pr->V[i].C = Vec4Set(Pr->V[i].N.X / 2 + 0.5,
                             Pr->V[i].N.Y / 2 + 0.5,
                             Pr->V[i].N.Z / 2 + 0.5, 1); /* Vec4Set(Rnd0(), Rnd0(), Rnd0(), 1); */
    }
    break;
  }
  fclose(F);
  return TRUE;
} /* End of 'VG4_RndPrimLoad' function */

/* END OF 'LOADPRIM.C' FILE */
